#!/usr/bin/python

# Don't know the source of this anymore, probably someone from #upstart on freenode. The idea is this: If your upstart job gets stuck, because your config file was somehow broken (an expect fork where none was needed, etc…), you run this script with the stuck pid, then, once it's done, kill it.

import os
import time
import sys

target=int(sys.argv[1])

print("Aiming for %d" % target)

MAX=2**16-1

def distance(target, pid):
    if pid > target:
       return MAX-pid+target
    else:
       return target-pid

tries=0
start=None
while(1):
    pid=os.fork()
    if not pid:
        time.sleep(100)
    if pid == -1:
        print("Fork Error!")
        sys.exit(1)
    if pid==target:
        print("Found %d" % target)
        sys.exit(0)
    os.kill(pid, 15)
    os.wait()
    if start is None:
        start = distance(target,pid)
    tries += 1
    if not tries % 100:
        pctdone = distance(target,pid) / start * 100
        print("[%d]%5.3f%%" % (pid,pctdone))
