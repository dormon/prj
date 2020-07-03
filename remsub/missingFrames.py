#!/usr/bin/python

import os
import sys
import argparse

parser = argparse.ArgumentParser(description='find missing frames')
parser.add_argument('--dir'         , type=str, default=""    ,  help='directory with frames')
parser.add_argument('--nofFrames'   , type=int, default=107558,  help='number of frames'     )


args = parser.parse_args()
dirname   = args.dir
nofFrames = args.nofFrames


alltxt = os.popen("ls "+dirname+" | grep \"jpg\" | sed \"s/.*_//g\" | sed \"s/\..*//g\"").read()
rendered = set(map(lambda x:int(x),filter(lambda x:len(x)>0,alltxt.split("\n"))))
all = set(range(nofFrames+1))
l = list(all-rendered)
l.sort()
print(l)

