#!/bin/sh
qemu-system-x86_64 \
 -initrd rootfs1.cpio \
 -kernel bzImage \
 -nographic \
 -append "root=/dev/ram rw console=ttyS0 rdinit=/sbin/init quite oops=panic panic=1" \
 -m 128M \
 -fsdev local,security_model=none,id=fsdev-fs0,path=/home/wangzc/Documents/k-hacks/null_pointer/share \
 -device virtio-9p-pci,fsdev=fsdev-fs0,mount_tag=rootme  \
 -gdb tcp::1234 \
 #-monitor /dev/null \