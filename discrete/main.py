#!/usr/bin/python

def sized(p):
    return p[0]*p[0]+p[1]*p[1]

def distance(s,p):
    return sized((s[0]-p[0],s[1]-p[1]))

def mind(s,p,d):
    #d=distance(s,p)
    comb=[(p[0]-1+(i%3),p[1]-1+(i/3),i) for i in range(9)]
    dist=map(lambda x:(abs(x[0]-d),x[1]),map(lambda x:(distance(s,(x[0],x[1])),x[2]),comb))
    #print comb
    #print dist
    mina=min(dist)
    dist.remove(mina)
    minb=min(dist)
    dist.remove(minb)
    minc=min(dist)
    me    = comb[mina[1]]
    left  = comb[minb[1]]
    right = comb[minc[1]]
    return [(me[0],me[1]),(left[0],left[1]),(right[0],right[1])]


def getCircle(s,p):
    d=distance(s,p)
    circ=set([p])
    new = mind(s,p,d)
    while (not circ.issuperset([new[0]])) or (not circ.issuperset([new[1]])) or (not circ.issuperset([new[2]])):
        go0 =True
        go1 =True;
        go2 =True;
        if(circ.issuperset([new[0]])):
            go0=False
        if(circ.issuperset([new[1]])):
            go1=False
        if(circ.issuperset([new[2]])):
            go2=False

        circ.add(new[0])
        circ.add(new[1])
        circ.add(new[2])

        if go0:
            new = mind(s,new[0],d)
        elif go1:
            new = mind(s,new[1],d)
        else:
            new = mind(s,new[2],d)
    return circ


#print mind((0,0),(11,3))
circ=getCircle((0,0),(11,3)))
