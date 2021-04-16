from bcc import BPF

prog = """
#include <uapi/linux/ptrace.h>
#include <linux/sched.h>
#include <linux/mm.h>

#include <linux/slab.h>

struct kmem_cache;


int kprobe__kmem_cache_alloc(struct pt_regs *ctx, void *s, gfp_t gfpflags) {
    char comm[250];
    bpf_get_current_comm(comm, sizeof(comm));
    bpf_trace_printk("kmem_cache_alloc  rbx:%llx  comm:%s   \\n", ctx->bx, comm);
    bpf_probe_read_kernel(comm, 60, s);
    //bpf_trace_printk("  kmem_cache: %s\\n", comm);
    for (int i = 0; i < 60;i++) {
        bpf_trace_printk("%s\\n", comm[i]);
    }
    return 0;
}


"""

prog1 = """
TRACEPOINT_PROBE(kmem, kmem_cache_alloc) {
    bpf_trace_printk("%llx\\n %llx\\n", args->ptr, args->call_site);
}
"""


b = BPF(text=prog)
# b.attach_kprobe(event=b.get_kprobe_functions("kmem_cache_alloc"), fn_name="")

b.trace_print()

# while True:
#     try:
#         (task, pid, cpu, flags, ts, msg) = b.trace_fields()
#     except ValueError:
#         continue
#     print("%-18.9f %-16s %-6d %s" % (ts, task, pid, msg))