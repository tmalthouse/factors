#!/usr/bin/python3

import argparse, subprocess
from itertools import zip_longest

IPs=[]

parser = argparse.ArgumentParser()
parser.add_argument("IPs", help="A file containing the IP addresses of the nodes, separated by commas (eg. 1.1.1.1, 2.2.2.2, etc...)")
parser.add_argument("Numbers", help="A list of numbers to factorize. One per line, separated by newlines\neg. 1\n2\n3")
args = parser.parse_args()

with open(args.IPs, "r") as IPList:
    data=myfile.read().replace('\n', ',')
    IPs=data.split(',')

num_procs = len(IPs) + 1

def file_len(fname):
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i+1

num_lines = file_len(args.Numbers)

lines_per_file = int(num_lines/num_procs)+1

def grouper(n, interable, fillvalue=None):
# grouper(3, 'ABCDEFG', 'x') --> ABC DEF Gxx
    args = [iter(iterable)]*n
    return zip_longest(fillvalue=fillvalue, *args)

with open(args.Numbers) as nums:
    i=0
    for i, g in enumerate(grouper(lines_per_file, f, fillvalue=''), 1):
        with open("nums_"+str(i), "w") as out:
            out.writelines(g)
        i+=1

factor_procs = []

for index, val in enumerate(IPs):
#We send the file to each core, using blowfish encryption, since it appears to be faster than the default.
    subprocess.call("scp -c blowfish num_" + str(index+1) + " node@" + val + ":/home/node/factors")
#And run the factor script on each one.
    factor_procs.append(subprocess.Popen("ssh node@" + val + " factors/factor num*"))

# Now we can run our own job
factor_procs.append(subprocess.Popen("./factor num_0"))

# And wait for everything to finish.
for i in factor_procs:
    i.wait()

# Now we can copy the results back over
for index, val in enumerate(IPs):
    subprocess.call("scp -c blowfish node@" + val + ":/home/node/factors/num_" + str(index+1) + " .")

# And concat em all together.

subprocess.call("cat num* > " + args.Numbers)

# And we are done!
