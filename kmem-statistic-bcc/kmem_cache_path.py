from bcc import BPF

prog = """
#include <uapi/linux/ptrace.h>




int do___kmalloc(struct pt_regs *ctx)
{
   unsigned long ret = PT_REGS_RC(ctx);
   bpf_trace_printk("__kmalloc:%llx\\n", ret);
   return 0;
}

int do_kmem_cache_alloc_trace(struct pt_regs *ctx) 
{
   unsigned long ret = PT_REGS_RC(ctx);
   bpf_trace_printk("kmem_cache_alloc_trace:%llx\\n", ret);
   return 0;
}

struct kmalloc_args {
   unsigned short common_type;   
   unsigned char common_flags;    
   unsigned char common_preempt_count;     
   int common_pid;  

   unsigned long call_site;
   const void * ptr; 
   size_t bytes_req; 
   size_t bytes_alloc;     
   gfp_t gfp_flags;
};

//int printarg(struct kmalloc_args *args) {
TRACEPOINT_PROBE(kmem, kmalloc) {
   bpf_trace_printk("kmalloc:%llx\\n", args->ptr);
   return 0;
}
"""

b = BPF(text=prog)
# b.attach_kretprobe(event='__kmalloc', fn_name="do___kmalloc")
b.attach_kretprobe(event='kmem_cache_alloc_trace', fn_name="do_kmem_cache_alloc_trace")
# b.attach_tracepoint("kmem:kmalloc", "printarg")
b.trace_print()
# while 1:
#     try:
#         (task, pid, cpu, flags, ts, msg) = b.trace_fields()
#     except ValueError:
#         continue
#     print("%-18.9f %-16s %-6d %s" % (ts, task, pid, msg))