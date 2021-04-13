# k-hacks
kernel hack samples collection

## GDB direct debug

* compile kernel with debug info
* replace kernel & modules

`sudo gdb vmlinux /proc/kcore`

kcore相当于一个物理内存的备份

## Compile kernel with debug info

kernel hacking  ->  compile-time checks and compiler options  -> Compile the kernel with debug info(GDB scripts may leads to compile failure)

**ebpf不能调用内核函数，只能调用bpf helper！！！！**


[Linux Extended BPF (eBPF) Tracing Tools](http://www.brendangregg.com/ebpf.html#bpftrace)

[bpftrace Reference Guide](https://github.com/iovisor/bpftrace/blob/master/docs/reference_guide.md)

[linux/samples/bpf/](https://github.com/torvalds/linux/tree/master/samples/bpf)

<table border="1">
<tbody><tr><th>Variable</th><th>Description</th></tr>
<tr><td><tt>pid</tt></td><td>Process ID</td></tr>
<tr><td><tt>tid</tt></td><td>Thread ID</td></tr>
<tr><td><tt>uid</tt></td><td>User ID</td></tr>
<tr><td><tt>username</tt></td><td>Username</td></tr>
<tr><td><tt>comm</tt></td><td>Process or command name</td></tr>
<tr><td><tt>curtask</tt></td><td>Current task_struct as a u64</td></tr>
<tr><td><tt>nsecs</tt></td><td>Current time in nanoseconds</td></tr>
<tr><td><tt>elapsed</tt></td><td>Time in nanoseconds since bpftrace start</td></tr>
<tr><td><tt>kstack</tt></td><td>Kernel stack trace</td></tr>
<tr><td><tt>ustack</tt></td><td>User-level stack trace</td></tr>
<tr><td><tt>arg0...argN</tt></td><td>Function arguments</td></tr>
<tr><td><tt>args</tt></td><td>Tracepoint arguments</td></tr>
<tr><td><tt>retval</tt></td><td>Function return value</td></tr>
<tr><td><tt>func</tt></td><td>Function name</td></tr>
<tr><td><tt>probe</tt></td><td>Full probe name</td></tr>
<tr><td><tt>$1...$N</tt></td><td>Positional parameters</td></tr>
<tr><td><tt>cgroup</tt></td><td>Default cgroup v2 ID</td></tr>
</tbody></table>

----------------

<table border="1">
<tbody><tr><th>Function</th><th>Description</th></tr>
<tr><td><tt>printf("...")</tt></td><td>Print formatted string</td></tr>
<tr><td><tt>time("...")</tt></td><td>Print formatted time</td></tr>
<tr><td><tt>join(char *arr[])</tt></td><td>Join array of strings with a space</td></tr>
<tr><td><tt>str(char *s [, int length])</tt></td><td>Return string from s pointer</td></tr>
<tr><td><tt>buf(void *p [, int length])</tt></td><td>Return a hexadecimal string from p pointer</td></tr>
<tr><td><tt>strncmp(char *s1, char *s2, int length)</tt></td><td>Compares two strings up to length</td></tr>
<tr><td><tt>sizeof(expression)</tt></td><td>Returns the size of the expression</td></tr>
<tr><td><tt>kstack([limit])</tt></td><td>Kernel stack trace up to limit frames</td></tr>
<tr><td><tt>ustack([limit])</tt></td><td>User-level stack trace up to limit frames</td></tr>
<tr><td><tt>ksym(void *p)</tt></td><td>Resolve kernel address to symbol</td></tr>
<tr><td><tt>usym(void *p)</tt></td><td>Resolve user-space address to symbol</td></tr>
<tr><td><tt>kaddr(char *name)</tt></td><td>Resolve kernel symbol name to address</td></tr>
<tr><td><tt>uaddr(char *name)</tt></td><td>Resolve user-space symbol name to address</td></tr>
<tr><td><tt>ntop([int af,]int|char[4:16] addr)</tt></td><td>Convert IP address data to text</td></tr>
<tr><td><tt>reg(char *name)</tt></td><td>Return register value</td></tr>
<tr><td><tt>cgroupid(char *path)</tt></td><td>Return cgroupid for /sys/fs/cgroup/... path</td></tr>
<tr><td><tt>time("...")</tt></td><td>Print formatted time</td></tr>
<tr><td><tt>system("...")</tt></td><td>Run shell command</td></tr>
<tr><td><tt>cat(char *filename)</tt></td><td>Print file content</td></tr>
<tr><td><tt>signal(char[] sig | int sig)</tt></td><td>Send a signal to the current task</td></tr>
<tr><td><tt>override(u64 rc)</tt></td><td>Override a kernel function return value</td></tr>
<tr><td><tt>exit()</tt></td><td>Exits bpftrace</td></tr>
<tr><td><tt>@ = count()</tt></td><td>Count events</td></tr>
<tr><td><tt>@ = sum(x)</tt></td><td>Sum the value</td></tr>
<tr><td><tt>@ = hist(x)</tt></td><td>Power-of-2 histogram for x</td></tr>
<tr><td><tt>@ = lhist(x, min, max, step)</tt></td><td>Linear histogram for x</td></tr>
<tr><td><tt>@ = min(x)</tt></td><td>Record the minimum value seen</td></tr>
<tr><td><tt>@ = max(x)</tt></td><td>Record the maximum value seen</td></tr>
<tr><td><tt>@ = stats(x)</tt></td><td>Return the count, average, and total for this value</td></tr>
<tr><td><tt>delete(@x[key])</tt></td><td>Delete the map element</td></tr>
<tr><td><tt>clear(@x)</tt></td><td>Delete all keys from the map</td></tr>
</tbody></table>