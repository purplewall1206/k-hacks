#!/usr/bin/env python
from bcc import BPF

# sudo bpftrace -e 'kprobe:ext4_inode_csum { printf("trigger\n"); }'

BPF(text="""
#include <uapi/linux/ptrace.h>
int kprobe__ext4_inode_csum(struct pt_regs *ctx, struct inode *inode, struct ext4_inode *raw) {
    bpf_trace_printk("inode=%llx  raw=%llx  \\n", inode, raw);
    return 0;
}

int kretprobe__ext4_inode_csum(struct pt_regs *ctx) {
    unsigned long ret = PT_REGS_RC(ctx);
    bpf_trace_printk("ret: %llx\\n", ret);
    return 0;
}
""").trace_print()