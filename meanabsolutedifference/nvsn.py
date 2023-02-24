from random import *

def mad(x):
    res = 0
    counter = 0
    for i in range(len(x)):
        for j in range(i,len(x)):
            counter+=1
            res += abs(x[i]-x[j])
    return float(res)/float(counter)

def approx1(x):
    res = 0
    counter = 0
    for i in range(len(x)-1):
        counter+=1
        res += abs(x[i]-x[i+1])
    return float(res)/float(counter)
    
def approx2(x):
    res = 0
    counter = 0
    for i in range(1,len(x)):
        counter+=1
        res += abs(x[0]-x[i])
    return float(res)/float(counter)

def approx3(x):
    res = 0
    counter = 0
    for i in range(1,len(x)):
        counter+=1
        res += abs(x[int(len(x)*random())]-x[int(len(x)*random())])
    return float(res)/float(counter)

def measureApprox(f,g,N):
    err = 0
    for i in range(N):
        x = [random() for i in range(64)]
        err = abs(f(x)-g(x))
    return err / float(N)


print("approx1 mean average difference error: "+str(measureApprox(mad,approx1,10000)))
print("approx2 mean average difference error: "+str(measureApprox(mad,approx2,10000)))
print("approx3 mean average difference error: "+str(measureApprox(mad,approx3,10000)))




