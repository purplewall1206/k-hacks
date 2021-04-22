#!/usr/bin/env python
from bcc import BPF

BPF(text="""
#include <uapi/linux/ptrace.h>
int kprobe__ip_rcv(struct pt_regs *ctx, struct sk_buff *skb) {
    bpf_trace_printk("skb=%llx  %p!\\n", skb, skb);
    return 0;
}
""").trace_print()