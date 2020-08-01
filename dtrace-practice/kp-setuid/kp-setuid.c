#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>

static struct jprobe setuid_jprobe;

static asmlinkage int kp_setuid(uid_t uid) 
{
	pr_info("process %s [%d] attempted setuid to %d \n", current->comm, current->cred->uid, uid);
	jprobe_return();
	return 0;
}

int init_module(void)
{
	int ret;

	setuid_jprobe.entry = (kprobe_opcode_t *) kp_setuid;
	setuid_jprobe.kp.symbol_name = "sys_setuid";
	if ((ret = register_jprobe(&set_jprobe)) < 0) {
		printk("register_jprobe failed, returned %d\n", ret);
		return -1;
	}
	return 0;
}

void cleanup_module(void)
{
	unregister_jprobe(&setuid_jprobe);
	printk("jprobe unregistered\n");
}

module_init(init_module);
module_exit(cleanup_module);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("PPW");
