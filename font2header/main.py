#!/usr/bin/python
import png

pf = png.Reader(open("font.png","rb")).read()
imgWidth  = pf[3]["size"][0]
imgHeight = pf[3]["size"][1]
nofCharacters = 95
pixels = list(pf[2])

pixs=[]
for row in reversed(pixels):
    pixs+=row[::3]


eights = zip(*[iter(pixs)]*8)
def eights2bits(ei):
    result = 0
    for i in range(8):
        if ei[i]>0:
            result+=1<<i
    return result

bits = map(eights2bits,eights)

print "#pragma once"
print "namespace ge{"
print "  namespace res{"
print "    namespace font{"
print "      const uint32_t width =",imgWidth,";"
print "      const uint32_t height =",imgHeight,";"
print "      const uint8_t data[] = {"
line = "        "
for i in bits:
    line += str(i)+","
    if len(line)>76:
        print line
        line = "        "
print line
print "      };"
print "    }"
print "  }"
print "}"
