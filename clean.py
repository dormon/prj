#!/usr/bin/python

import os

for fileName in os.listdir("."):
    if os.path.isdir(fileName):
        os.system("make -C "+fileName+" clean")

