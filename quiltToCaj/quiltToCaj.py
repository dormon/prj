#!/usr/bin/env python3

from PIL import Image
from struct import pack

import argparse
import os
import ntpath

parser = argparse.ArgumentParser(description='This script converts quilt images into native holographics display image.')
parser.add_argument('--suffix'      , type=str  , default="_h"     , help="suffix of output file if the output file is not specified")
parser.add_argument('--input'       , type=str  , default=""       , help='input quilt image')
parser.add_argument('--output'      , type=str  , default=""       , help='output quilt image. if --input is: "/a/b/input.ext" then tje default output is "input${SUFFIX}.ext"')
parser.add_argument('--columns'     , type=int  , default=5        , help='number of quilt columns')
parser.add_argument('--rows'        , type=int  , default=9        , help='number of quilt rows')
parser.add_argument('--width'       , type=int  , default=2560     , help='width of holographics display in pixels')
parser.add_argument('--height'      , type=int  , default=1600     , help='height of holographic')
parser.add_argument('--tilt'        , type=float, default=-0.1153  , help='tilt of the holographics display')
parser.add_argument('--pitch'       , type=float, default=354.42108, help='pitch of the holographics display')
parser.add_argument('--center'      , type=float, default=0.04239  , help='center of the holographics display')
parser.add_argument('--viewPortion' , type=float, default=0.99976  , help='viewPortion of the holographics display')



args = parser.parse_args()

columns        = args.columns
rows           = args.rows   
inputFileName  = args.input  
outputFileName = args.output 
suffix         = args.suffix 
width          = args.width  
height         = args.height 
subp           = 1.0/float(width*3)
tilt           = args.tilt
pitch          = args.pitch
center         = args.center
viewPortion    = args.viewPortion

if inputFileName == "":
    parser.print_help()
    exit()

if outputFileName == "":
    name, ext = os.path.splitext(inputFileName)
    path = ntpath.dirname(name)
    baseName = ntpath.basename(name)
    #outputFileName = os.path.join(path, baseName + suffix + ext)
    outputFileName = baseName + suffix + ext
 
quilt = Image.open(inputFileName)
caj   = Image.new('RGB', (width, height))

def clamp(x,mmin,mmax):
    if x < mmin:
        return mmin
    if x > mmax:
        return mmax

def fract(x):
    return abs(x) - int(abs(x))

def floor(x):
    return int(x)

#
#def readPixel(img,cc):
#    x = cc[0]*img.width
#    y = cc[1]*img.height
#    x = clamp(x,0,img.width-1)
#    y = clamp(y,0,img.height-1)
#    xx = int(x)
#    yy = int(y)
#    xf = fract(x)
#    yf = fract(y)


def recomputeCoord(x,y,z):
    z      = int(z*columns*rows)
    row    = int(z/columns)
    column = z%columns
    x = ((column + x) / columns) * viewPortion
    y = ((row + y) / rows) * viewPortion
    return (min(int(x*quilt.width),quilt.width-1),quilt.height-1-min(int(y*quilt.height),quilt.height-1))

print ("ahoj")

for y in range(caj.height):
    print(y)
    for x in range(caj.width):
        xx = float(x)/float(caj.width) 
        yy = float(y)/float(caj.height)
        rgb = [0,0,0]
        for i in range(3):
            z = (xx + i*subp + yy*tilt)*pitch - center
            z = abs(z)-int(abs(z))
            z = 1-z
            rgb[i] = quilt.getpixel(recomputeCoord(xx,yy,z))[i]
        caj.putpixel((x,caj.height-1-y),(rgb[0],rgb[1],rgb[2]))



#TODO do the transformation

caj.save(outputFileName)

exit()

