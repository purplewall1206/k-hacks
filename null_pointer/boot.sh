#!/bin/sh
qemu-system-x86_64 \
 -initrd rootfs.cpio \
 -kernel bzImage \
 -nographic \
 -append "root=/dev/ram rw console=ttyS0 rdinit=/sbin/init quite oops=panic panic=1" \
 -m 128M \
 -monitor /dev/null \
 -gdb tcp::1234 \
