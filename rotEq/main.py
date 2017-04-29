def rot(x,n,b):
    return ((x<<n)&((1<<b)-1))+((x<<n)/(1<<b))

def comp(x,y,b):
    for i in range(b):
        if x==rot(y,i,b):
            return True
    return False

def getSize(b):
    return 1<<b;

def getList(b):
    result=[]
    for i in range(getSize(b)):
        insert=True
        for j in result:
            if comp(i,j,b):
                insert=False
                break
        if insert:
            result+=[i]
    return result

def getList2(b):
    result=set()
    for i in range(getSize(b)):
        smallest=getSize(b)
        for j in range(b):
            if smallest>rot(i,j,b):
                smallest=rot(i,j,b)
        result.add(smallest)
    return list(result)


#for i in range(1,11):
#    print len(getList(i))

for i in range(1,33):
    print i,len(getList2(i))

