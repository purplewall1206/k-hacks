#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>

void (*my_funptr)(void);

int null_pointer_write(struct file *file, const char *buf, unsigned long len)
{
    my_funptr(); // 未经验证直接调用
    return len;
}

static int __init null_pointer_init(void)
{
    printk(KERN_ALERT "null_pointer driver init!\n");
    create_proc_entry("null_pointer", 0666, 0)->write_proc = null_pointer_write;
    return 0;
}

static void __exit null_pointer_exit(void)
{
    printk(KERN_ALERT "null_pointer driver exit\n");
}

module_init(null_pointer_init);
module_exit(null_pointer_exit);