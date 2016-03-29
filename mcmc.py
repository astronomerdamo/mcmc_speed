'''

    Simple MCMC:
    Utilizes a Metropolis-Hastings Algorithm with a 
    standard Gibbs sampler.

'''

from __future__ import print_function 
from numpy import random, exp
import time 

'''
    Basic linear equation function
'''

def lnfnc(x, a, b): 
    return  [a*x + b for x in x]

'''
    Basic chi squared function
    Note: data set error is variance not stddev
'''

def chisqr(yo, ye, syo):
    csq = [(yo - ye)**2 / syo for yo, ye, syo in zip(yo, ye, syo)]
    return sum(csq)

'''
    Basic Metropolis-Hastings Algorithm with a 
    standard Gibbs sampler.
'''

def mcmc(a0, sa, b0, sb, x, y, dy):

    y0 = lnfnc(x, a0, b0)
    chi0 = chisqr(y0, y, dy)

    atrace = []
    btrace = []

    i = 0
    j = 0
    mcn = 500000
    brn = 1000

    while i < mcn: 
        at = random.normal(a0, sa)
        bt = random.normal(b0, sb)

        yt = lnfnc(x, at, bt)
        chit = chisqr(yt, y, dy)
        
        acal = exp((chi0 - chit)/2.0)
        aexp = min(1.0, acal)
        u = random.rand()

        if u <= aexp:
            a0 = at
            b0 = bt
            y0 = yt
            chi0 = chit
            atrace.append(at)
            btrace.append(bt)
            i += 1
        j += 1

    print("i: ", i, ", j: ", j)
    print("Acceptance ratio:", i/float(j))
    print("a: ", sum(atrace[brn:]) / len(atrace[brn:]), " b: ", sum(btrace[brn:]) / len(btrace[brn:]))

#
# MAIN
#

x = [203, 58, 210, 202,
     198, 158, 165, 201,
     157, 131, 166, 160,
     186, 125, 218, 146]

y = [495, 173, 479, 504,
     510, 416, 393, 442,
     317, 311, 400, 337,
     423, 334, 533, 344]

dy = [21, 15, 27, 14,
      30, 16, 14, 25,
      52, 16, 34, 31,
      42, 26, 16, 22]

a0 = 2.5
sa = 0.025
b0 = 28.82
sb = 2.5

cpubeg = time.time()
mcmc(a0, sa, b0, sb, x, y, dy)
print("CPU time: ", time.time() - cpubeg)

