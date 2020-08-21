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
bpftrace -lv 显示更详细的信息和参数
```

详见 `list.txt` 和 `vlist.txt` 两个文件

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

bpftrace -e 'kprobe:do_sys_open /comm == "bash"/ { @[ustack] = count(); }'

bpftrace -e 'BEGIN { printf("I got %d, %s (%d args)\n", $1, str($2), $#); }' 42 "hello"
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

## 4. variables

```
@global_name
@thread_local_variable_name[tid]
$scratch_name

bpftrace -e 'BEGIN { @start = nsecs; }
    kprobe:do_nanosleep /@start != 0/ { printf("at %d ms: sleep\n", (nsecs - @start) / 1000000); }'

bpftrace -e 'kprobe:do_nanosleep { @start[tid] = nsecs; }
    kretprobe:do_nanosleep /@start[tid] != 0/ {
        printf("slept for %d ms\n", (nsecs - @start[tid]) / 1000000); delete(@start[tid]); }'

bpftrace -e 'kprobe:do_nanosleep { @start[tid] = nsecs; }
    kretprobe:do_nanosleep /@start[tid] != 0/ { $delta = nsecs - @start[tid];
        printf("slept for %d ms\n", $delta / 1000000); delete(@start[tid]); }'
```

## 5. functions
    - [1. Builtins](#1-builtins-1)
    - [2. `printf()`: Print Formatted](#2-printf-Printing)
    - [3. `time()`: Time](#3-time-time)
    - [4. `join()`: Join](#4-join-join)
    - [5. `str()`: Strings](#5-str-strings)
    - [6. `ksym()`: Symbol Resolution, Kernel-Level](#6-ksym-symbol-resolution-kernel-level)
    - [7. `usym()`: Symbol Resolution, User-Level](#7-usym-symbol-resolution-user-level)
    - [8. `kaddr()`: Address Resolution, Kernel-Level](#8-kaddr-address-resolution-kernel-level)
    - [9. `uaddr()`: Address Resolution, User-Level](#9-uaddr-address-resolution-user-level)
    - [10. `reg()`: Registers](#10-reg-registers)
    - [11. `system()`: System](#11-system-system)
    - [12. `exit()`: Exit](#12-exit-exit)
    - [13. `cgroupid()`: Resolve cgroup ID](#13-cgroupid-resolve-cgroup-id)
    - [14. `ntop()`: Convert IP address data to text](#14-ntop-convert-ip-address-data-to-text)
    - [15. `kstack()`: Stack Traces, Kernel](#15-kstack-stack-traces-kernel)
    - [16. `ustack()`: Stack Traces, User](#16-ustack-stack-traces-user)
    - [17. `cat()`: Print file content](#17-cat-print-file-content)
    - [18. `signal()`: Send a signal to the current task](#18-signal-send-a-signal-to-current-task)
    - [19. `strncmp()`: Compare first n characters of two strings](#19-strncmp-compare-first-n-characters-of-two-strings)
    - [20. `override()`: Override return value](#20-override-override-return-value)
    - [21. `buf()`: Buffers](#21-buf-buffers)
    - [22. `sizeof()`: Size of type or expression](#22-sizeof-size-of-type-or-expression)


## 6. map functions
    - [1. Builtins](#1-builtins-2)
    - [2. `count()`: Count](#2-count-count)
    - [3. `sum()`: Sum](#3-sum-sum)
    - [4. `avg()`: Average](#4-avg-average)
    - [5. `min()`: Minimum](#5-min-minimum)
    - [6. `max()`: Maximum](#6-max-maximum)
    - [7. `stats()`: Stats](#7-stats-stats)
    - [8. `hist()`: Log2 Histogram](#8-hist-log2-histogram)
    - [9. `lhist()`: Linear Histogram](#9-lhist-linear-histogram)
    - [10. `print()`: Print Map](#10-print-print-map)


7. output 
    - [1. `printf()`: Per-Event Output](#1-printf-per-event-output)
    - [2. `interval`: Interval Output](#2-interval-interval-output)
    - [3. `hist()`, `printf()`: Histogram Printing](#3-hist-print-histogram-printing)