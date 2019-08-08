#!/usr/bin/python

import os
import sys
import shutil

base = "fce"
fracturedDir = "fractured/"

def sourceFile(n):
    return base+str(n)+".cpp"

def headerFile(n):
    return base+str(n)+".h"

def include(n):
    return "#include \""+headerFile(n)+"\"\n"

def sourceSrc(n,C = 10):
    src  = ""
    src += "int fce"+str(n)+"(int a){\n"
    src += "  int z0 = a;\n"
    for i in range(1,C+1):
        src += "  int z"+str(i)+" = z" + str(i-1) + " + a;\n";
    src += "  return z"+str(C)+" + "+str(n)+";\n";
    src += "}\n"
    return src


def genSource(n,C = 10):
    f = open(fracturedDir+sourceFile(n),"w")
    f.write(include(n)+sourceSrc(n,C))
    f.close()

def genHeader(n):
    f = open(fracturedDir+headerFile(n),"w")
    f.write("#pragma once\n\nint fce"+str(n)+"(int a);\n")
    f.close()

def genFracturedMain(n):
    src = "";
    for i in range(n):
        src = src + include(i)
    src += "int main(int,char*[]){\n"
    src += "  int a = 0;\n"
    for i in range(n):
        src += "  a = fce"+str(i)+"(a);\n"
    src += "return a;\n"
    src += "}\n"
    f = open(fracturedDir+"main.cpp","w")
    f.write(src)
    f.close()

def genUnit(n,C=10):
    genSource(n,C)
    genHeader(n)

def genUnits(n,C=10):
    for i in range(n):
        genUnit(i,C)

def genFracturedMakefile(n):
    src  = "%.o: %.cpp\n"
    src += "\tg++ -o $@ -c $<\n"
    src += "\n"
    src += "OBJ = main.o "
    for i in range(n):
        src += " " + base + str(i) + ".o"
    src += "\n"
    src += "\n"
    src += "fractured: ${OBJ}\n"
    src += "\tg++ *.o -o fractured\n"
    src += "clean:\n"
    src += "\trm -f *.o"
    f = open(fracturedDir+"makefile","w")
    f.write(src)
    f.close()

def genFractured(n,C=10):
    if os.path.isdir(fracturedDir):
        shutil.rmtree(fracturedDir)
    os.mkdir(fracturedDir)
    genUnits(n,C)
    genFracturedMain(n)
    genFracturedMakefile(n)

if len(sys.argv) != 3:
    print ("./genFiles.py nofFiles complexityOfFiles")
    exit(0)

nofFiles = int(sys.argv[1])
complexity = int(sys.argv[2])



genFractured(nofFiles,complexity)
