#!/usr/bin/python3

import urllib.request
import sys

def getWebPage(urlName):
    return urllib.request.urlopen(urlName).read()

def getMarketHistory(coinPair,hours):
    url = "https://www.cryptopia.co.nz/api/GetMarketHistory"
    url += "/" + str(coinPair)
    url += "/" + str(hours)
    print(url)
    return getWebPage(url)

if len(sys.argv) != 3:
    print("expected two arguments coinpair and number of hours")
    exit(0)

coinPair = int(sys.argv[1])
hours    = int(sys.argv[2])

print(getMarketHistory(coinPair,hours))
