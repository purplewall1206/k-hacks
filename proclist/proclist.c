#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched/task.h>
// #include <asm/current.h>
// #include <linux/list.h>

void DFS(struct task_struct *task, int *count)
{
    struct task_struct *child;
    struct list_head *list;
    pr_info("name:%s, pid:[%d], state: %li, stack: %lx\n", 
             task->comm, task->pid, task->state, task->stack);
    ++(*count);
    list_for_each(list, &task->children) {
        child = list_entry(list, struct task_struct, sibling);
        DFS(child, count);
    }
}

static int __init proclist_init(void)
{
    int ret = 0;
    pr_info("proclist loaded\n");
    int count = 0;
    DFS(&init_task, &count);
    pr_info("total iterate %d process\n", count);
    return ret;
}

static void __exit proclist_exit(void)
{
    pr_info("proclist exited\n");
}

module_init(proclist_init);
module_exit(proclist_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("PPW");
