#!/usr/bin/python

import sys
import subprocess
import time
import signal
import os

start_proc = int(sys.argv[1])
num_procs = int(sys.argv[2])

path = ""
for p in sys.argv[3:len(sys.argv)]:
    path += p + " "

#print "Executing %d copies of program: %s" % (num_procs, path)

procs = {}

# start
for i in range(start_proc, start_proc + num_procs):
    print "Starting process: %d" % i
    env = {}
    env["CARBON_PROCESS_INDEX"] = str(i)
    env["LD_LIBRARY_PATH"] = "/afs/csail/group/carbon/tools/boost_1_38_0/stage/lib"

    procs[i] = subprocess.Popen(path, shell=True, env=env)

# wait
active = True
returnCode = None

while active:
    time.sleep(0.1)
    for i in range(start_proc, start_proc + num_procs):
        returnCode = procs[i].poll()
        if returnCode != None:
            active = False
            break

# kill
for i in range(start_proc, start_proc + num_procs):
    returnCode2 = procs[i].poll()
    if returnCode2 == None:
        os.kill(procs[i].pid, signal.SIGKILL)
    elif returnCode == 0:
        returnCode = returnCode2

# exit
if returnCode != None:
    print >> sys.stderr, 'Exited with return code: %d' % returnCode

sys.exit(returnCode)
