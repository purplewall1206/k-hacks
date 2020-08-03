# k-hacks
kernel hack samples collection

## GDB direct debug

* compile kernel with debug info
* replace kernel & modules

`sudo gdb vmlinux /proc/kcore`

kcore相当于一个物理内存的备份
