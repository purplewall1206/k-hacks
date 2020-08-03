# k-hacks
kernel hack samples collection

## GDB direct debug

* compile kernel with debug info
* replace kernel & modules

`sudo gdb vmlinux /proc/kcore`

kcore相当于一个物理内存的备份

## Compile kernel with debug info

kernel hacking  ->  compile-time checks and compiler options  -> Compile the kernel with debug info(GDB scripts may leads to compile failure)
