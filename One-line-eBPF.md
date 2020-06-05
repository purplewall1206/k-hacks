# one line eBPF tutorial

[lesson 1 one-line-eBPF](https://github.com/iovisor/bpftrace/blob/master/docs/tutorial_one_liners.md)
[sample - 20 tools](https://github.com/iovisor/bpftrace/tree/master/tools)
[reference guide](https://github.com/iovisor/bpftrace/blob/master/docs/reference_guide.md)

**Attension**
linux v5.4 会出现 error asm ')' 的情况，但是v5.3 没有这种情况的发生。


## 1. list Probes

```
bpftrace -l  #显示全部trace
bpftrace -l 'tracepoint:syscalls:sys_enter_*'
```

## 2. execution

```
bpftrace -e 'BEGIN { printf("hello world\n"); }'

bpftrace -e 'tracepoint:syscalls:sys_enter_openat { printf("%s %s\n", comm, str(args->filename)); }'

bpftrace -e 'tracepoint:raw_syscalls:sys_enter { @[comm] = count(); }'

bpftrace -e 'tracepoint:syscalls:sys_exit_read /pid == 18644/ { @bytes = hist(args->ret); }'

bpftrace -e 'kretprobe:vfs_read { @bytes = lhist(retval, 0, 2000, 200); }'

bpftrace -e 'kprobe:vfs_read { @start[tid] = nsecs; } kretprobe:vfs_read /@start[tid]/ { @ns[comm] = hist(nsecs - @start[tid]); delete(@start[tid]); }'

bpftrace -e 'profile:hz:99 { @[kstack] = count(); }'

bpftrace -e 'tracepoint:sched:sched_switch { @[kstack] = count(); }'

bpftrace --include linux/path.h --include linux/dcache.h \
    -e 'kprobe:vfs_open { printf("open path: %s\n", str(((path *)arg0)->dentry->d_name.name)); }'

bpftrace -e 'kprobe:vfs_read /arg2 < 16/ { printf("small read: %d byte buffer\n", arg2); }'

bpftrace -e 'tracepoint:syscalls:sys_exit_read { @error[args->ret < 0 ? - args->ret : 0] = count(); }'


bpftrace -e 'kprobe:do_nanosleep { $i = 1; unroll(5) { printf("i: %d\n", $i); $i = $i + 1; } }'

bpftrace -e 'tracepoint:syscalls:sys_enter_read { @reads = count();
    if (args->count > 1024) { @large = count(); } }'
```

* comm 是current 进程名
* args 在`bpftrace -vl` 里面找一下可用的参数
* @ 特殊变量类型 map
* [] 把map映射的key变成了set
* count() 统计函数调用次数
* /.../ 过滤器
* hist(): summarizes the argument as a power-of-2 histogram. 
* lhist(): this is a linear histogram, where the arguments are: value, min, max, step.
* sched: The sched probe category has high-level scheduler and process events, such as fork, exec, and context switch.
* kstack: Returns the kernel stack trace. This is used as a key for the map, so that it can be frequency counted. 
* 申请变量 $i 而不是i
* 

## 3. probes
* kprobe - kernel function start
* kretprobe - kernel function return
* uprobe - user-level function start
* uretprobe - user-level function return
* tracepoint - kernel static tracepoints
* usdt - user-level static tracepoints
* profile - timed sampling
* interval - timed output
* software - kernel software events
* hardware - processor-level events
