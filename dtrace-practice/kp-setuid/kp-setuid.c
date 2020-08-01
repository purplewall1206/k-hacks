#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>

#include <linux/ktime.h>
#include <linux/limits.h>

#define MAX_SYMBOL_LEN	64
static char symbol[MAX_SYMBOL_LEN] = "sys_setuid";
module_param_string(symbol, symbol, sizeof(symbol), 0644);

static struct kprobe kp = {
	.symbol_name = symbol,
};

struct my_data {
	ktime_t entry_stamp;
};

static int entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	struct my_data *data;

	if (!current->mm) return 1;

	data = (struct my_data *)
}

/* kprobe pre_handler: called just before the probed instruction is executed */
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
	pr_info("<%s> pre_handler: p->addr = 0x%p, ip = %lx, flags = 0x%lx\n",
		p->symbol_name, p->addr, regs->ip, regs->flags);
	return 0;
}

/* kprobe post_handler: called after the probed instruction is executed */
static void handler_post(struct kprobe *p, struct pt_regs *regs,
				unsigned long flags)
{
	pr_info("<%s> post_handler: p->addr = 0x%p, flags = 0x%lx\n",
		p->symbol_name, p->addr, regs->flags);
}

/*
 * fault_handler: this is called if an exception is generated for any
 * instruction within the pre- or post-handler, or when Kprobes
 * single-steps the probed instruction.
 */
static int handler_fault(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
	pr_info("fault_handler: p->addr = 0x%p, trap #%dn", p->addr, trapnr);
	/* Return 0 because we don't handle the fault. */
	return 0;
}

// static asmlinkage int kp_setuid(uid_t uid) 
// {
// 	pr_info("process %s [%d] attempted setuid to %d \n", current->comm, current->cred->uid, uid);
// 	jprobe_return();
// 	return 0;
// }

static int __init kprobe_init (void)
{
	int ret;

	kp.pre_handler = handler_pre;
	kp.post_handler = handler_post;
	kp.fault_handler = handler_fault;

	ret = register_kprobe(&kp);
	if (ret < 0) {
		pr_err("register_kprobe failed, returned %d\n", ret);
		return ret;
	}
	pr_info("Planted kprobe at %p\n", kp.addr);
	return 0;
}

static void __exit kprobe_exit(void)
{
	unregister_kprobe(&kp);
	pr_info("kprobe at %p unregistered\n", kp.addr);
}

module_init(kprobe_init);
module_exit(kprobe_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("PPW");
