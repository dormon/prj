#!/usr/bin/python

import os

for fileName in os.listdir("."):
    if os.path.isdir(fileName):
        os.system("make -C "+fileName+" clean")

os.system("`find | grep \"build\" | sed \"s/build.*/build\/*/g\" | sort -u | tr '\n' \" \" | sed \"s/\(.*\)/rm -rf \1/g\"`")
