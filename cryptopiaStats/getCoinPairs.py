#!/usr/bin/python3

import urllib.request

def getWebPage(urlName):
    return urllib.request.urlopen(urlName).read()

print(getWebPage("https://www.cryptopia.co.nz/api/GetTradePairs"))
