def Mbn(b, n, mu, i, j,p):
    M = Decimal(b)/Decimal(10**n-mu)
    SCF = int((10**p)*M)
    RCF = (SCF%10**(i+j)-SCF%10**i)/10 ** i
    return RCF

from decimal import *

b = 1
n = 8
mu =  7
p = 200
i = 10
j = 128
mensaje = 53
getcontext().prec = p

print Mbn(b, n, mu, i, j,p)*mensaje
