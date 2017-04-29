#!/usr/bin/python

import sys
import re

if len(sys.argv)<2:
    sys.exit(0)

data0 = open(sys.argv[1]).read()

#cat glext.h | grep "^GLAPI" | grep -v "SUN\|NV\|ARB\|SGI\|EXT\|MESA\|INTEL\|ATI\|IBM\|HP\|APPLE\|AMD\|GREMEDY\|OES\|PGI\|3DFX\|INGR" | sed "s/ \*/*/g" | sed "s/\* /*/g" | sed "s/, /,/g" | sed "s/ ,/,/g" | sed "s/GLAPI \([a-zA-Z0-9]*\) APIENTRY \([a-zA-Z0-9]*\) (\(.*\));/\1,\2,\3/g"

data1         = re.compile(r"^GLAPI.*",flags=re.MULTILINE).findall(data0)
data2 = filter(lambda x:re.compile(r"SUN|NV|ARB|SGI|EXT|MESA|INTEL|ATI|IBM|HP|APPLE|AMD|GREMEDY|OES|PGI|3DFX|INGR").findall(x)==[] and  (x.find("Shader")>-1 or False),data1)
data3 = map(lambda x:re.sub(r" ,",r",",re.sub(r", ",r",",re.sub(r"\* ","*",re.sub(r" \*","*",x)))),data2)
data4 = map(lambda x:re.sub(r"GLAPI ([a-zA-Z0-9_]*) APIENTRY ([a-zA-Z0-9_]*) \((.*)\);",r"\1,\2,\3",x),data3)

def genClassName(string):
    return string[2].upper()+string[3:]

def genTypeName(string):
    spacepos=string.rfind(" ");
    starpos =string.rfind("*");
    if(spacepos>starpos):
        return (string[:string.rfind(" ")],string[string.rfind(" ")+1:])
    else:
        return (string[:string.rfind("*")+1],string[string.rfind("*")+1:])

def genClass(string):
    data0 = string.split(",")
    if(len(data0)>4):
        return ""
    typesNames=[];
    if(data0[0]!="void"):
        typesNames+=[("std::shared_ptr<"+data0[0]+">","result",data0[0],"","")]
    for i in data0[2:]:
        if(i=="void"):continue
        typesNames+=[(genTypeName(i)[1].upper(),genTypeName(i)[1],genTypeName(i)[0],"","")]

    typeMax = max(map(lambda x:len(x[0]),typesNames))
    nameMax = max(map(lambda x:len(x[1]),typesNames))

    tn=typesNames;
    typesNames=[];
    for i in tn:
        typesNames+=[(i[0],i[1]," "*(typeMax-len(i[0]))," "*(nameMax-len(i[1])),i[2])]

    result="template<"
    if(len(data0)>2):
        result+="typename "+genTypeName(data0[2])[1].upper()
    for i in data0[3:]:
        name = genTypeName(i)[1];
        result+=",typename "+name.upper()
    result+=">\n"

    className=genClassName(data0[1])
    result+="class "+className+": public Command{\n"
    result+="  protected:\n";
    for i in typesNames:
        result+="    "+i[0]+i[2]+"_"+i[1]+i[3]+";\n"

    result+="  public:\n"
    result+="    "+className+"("
    for i in typesNames:
        result+=i[0]+" "+i[1]+","*(int(i!=typesNames[-1]))
    result+="){\n"
    for i in typesNames:
        result+="      this->_"+i[1]+i[3]+" = "+i[1]+i[3]+";\n"
    result+="    }\n"
    result+="    void operator()();\n"
    for i in typesNames:
        result+="    inline "+i[0]+i[2]+"&get"+i[1][0].upper()+i[1][1:]+i[3]+"(){return this->_"+i[1]+i[3]+";}\n"
    for i in typesNames:
        result+="    inline void set"+i[1][0].upper()+i[1][1:]+i[3]+"("+i[0]+i[2]+" "+i[1]+i[3]+"){this->_"+i[1]+i[3]+" = "+i[1]+i[3]+";}\n"

    result+="};\n"
    nofArgs=len(data0)-2
    nofComb=2**nofArgs;
    for i in range(nofComb):
        result+="template<>\n"
        result+="void "+className+"<"
        for j in range(nofArgs):
            if((i>>j)&1):
                result+="std::shared_ptr<"+typesNames[j][4]+">"+","*(j<nofArgs-1)
            else:
                result+=typesNames[j][4]+","*(j<nofArgs-1)
        result+=">::operator()(){\n"
        if data0[0]=="void":
            result+="  "+data0[1]+"("
            for j in range(nofArgs):
                if((i>>j)&1):
                    result+="*this->_"+typesNames[j][1]+","*(j<nofArgs-1)
                else:
                    result+="this->_"+typesNames[j][1]+","*(j<nofArgs-1)
            result+=");\n"
        else:
            result+="  "
            result+="*this->_result = "
            result+=data0[1]+"("
            for j in range(nofArgs):
                if((i>>j)&1):
                    result+="*this->_"+typesNames[j][1]+","*(j<nofArgs-1)
                else:
                    result+="this->_"+typesNames[j][1]+","*(j<nofArgs-1)
            result+=");\n"
        result+="}\n"


    print result

map(lambda x:genClass(x),data4)





#print glext
