#!/usr/bin/env python3
from PIL import Image
from struct import pack

def pre(p):
    p = list(p)
    p[0] = p[0]*p[3]//255
    p[1] = p[1]*p[3]//255
    p[2] = p[2]*p[3]//255
    return p

def write(i, o, X, Y):
    for y in range(Y):
        for x in range(X):
            p = pre(i.getpixel((x, y)))
            o.write(pack('4B', p[2], p[1], p[0], p[3]))


i = Image.open('images/fish.png')
with open('fish.bin', 'wb') as o:
    write(i, o, 100, 59)

i = Image.open('images/window.png')
with open('window.bin', 'wb') as o:
    write(i, o, 320, 200)
