#!/usr/bin/python3

import argparse, subprocess
from itertools import zip_longest

IPs=[]

parser = argparse.ArgumentParser()
parser.add_argument("IPs", help="A file containing the IP addresses of the nodes, separated by commas (eg. 1.1.1.1, 2.2.2.2, etc...)")
parser.add_argument("Numbers", help="A list of numbers to factorize. One per line, separated by newlines\neg. 1\n2\n3")
args = parser.parse_args()

with open(args.IPs, "r") as IPList:
    data=IPList.read().replace('\n', ',')
    IPs=data.split(',')
IPs = [x for x in IPs if x]

num_procs = len(IPs) + 1

def file_len(fname):
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i+1

num_lines = file_len(args.Numbers)

lines_per_file = int(num_lines/num_procs)+1

def grouper(n, i, fillvalue=None):
# grouper(3, 'ABCDEFG', 'x') --> ABC DEF Gxx
    args = [iter(i)]*n
    return zip_longest(fillvalue=fillvalue, *args)

with open(args.Numbers) as nums:
    i=0
    for i, g in enumerate(grouper(lines_per_file, nums, fillvalue=''), 1):
        with open("nums_"+str(i), "w") as out:
            out.writelines(g)
        i+=1
print(IPs)
factor_procs = []
subprocess.call("pwd")
for index, val in enumerate(IPs):
#We send the file to each core
    subprocess.call("scp nums_" + str(index+2) + " node@" + val + ":/home/node/factors/", shell=True)
#And run the factor script on each one.
    print("IP literal is " + val)
    print("Command is: " + "echo \"factors/factor nums_" + str(index+2) + "\" | node@" + val)
    factor_procs.append(subprocess.Popen("ssh node@" + val + " \"cd factors && ./factor nums_" + str(index+2) + "\"", shell=True))

# Now we can run our own job
factor_procs.append(subprocess.Popen("./factor nums_1", shell=True))

# And wait for everything to finish.
for i in factor_procs:
    i.wait()

# Now we can copy the results back over
for index, val in enumerate(IPs):
    subprocess.call("scp node@" + val + ":/home/node/factors/nums_" + str(index+2) + " .", shell=True)

# And concat em all together.

subprocess.call("cat num* > " + args.Numbers, shell=True)

# And we are done!
