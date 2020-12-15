import sys

class stc:
    comm = ''
    pid = 0
    state = 0
    addr = 0
    def __init__(self, comm, pid, state, addr):
        self.comm = comm
        self.pid = pid
        self.state = state
        self.addr = addr

procs = []
with open('record.txt', 'r') as f:
    for line in f.readlines():
        # print(line)
        tmp = line[15:]
        tmps = tmp.split(',')
        procs.append(
            stc(comm=tmps[0][5:], pid=int(tmps[1][6:-1]), 
                state=int(tmps[2][7:]), addr=int(tmps[3][7:],16))
        )
start = 0xffffc90000034000
end   = 0xffffc90002800000
stacks = []
for p in procs:
    stacks.append(p.addr)

stacks.sort()
hex(stacks[0])
# '0xffffc90000034000'
hex(stacks[-1])
# '0xffffffff82200000'
hex(stacks[-2])
# '0xffffc9000270c000
hex(stacks[-2]-stacks[0])
# '0x26d8000'

interval = 0x8000 # 4k*8 = 2^15
buckets = [0]*1280
for x in stacks[:-1]:
    index = int((x-start)/interval)
    buckets[index] = buckets[index]+1

sum = 0
for b in buckets:
    if b > 0:
        sum = sum+1
# 所以每个内核栈都需要一个专门的tag页保存

import matplotlib.pyplot as plt
plt.bar(range(1280), buckets)
plt.savefig('./bucket.png')