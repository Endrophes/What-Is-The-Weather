#!/usr/bin/env python3
# Copyright 2009-2017 BHG http://bw.org/

import sys
import requests
import json
import subprocess

latitude = ""
longitude = ""

def printInput():
    n = len(sys.argv)
    print("Total arguments passed:", n)

    # Arguments passed
    print("\nName of Python script:", sys.argv[0])

    print("\nArguments passed:", end = " ")
    for i in range(1, n):
        print(sys.argv[i], end = " ")

def getLocationFromIp():
        ipInfoLink = 'http://ipinfo.io/json'
        response = requests.get(ipInfoLink)
        resText = response.text
        # print("ipInfo data " + resText)
        data = json.loads(resText)
        return data

# printInput()

# These need to come from command line
numArgs = len(sys.argv)
if numArgs >= 3:
    latitude = sys.argv[1]
    longitude = sys.argv[2]

# If they are blank use default
if latitude == "" or longitude == "":
    data = getLocationFromIp()
    gps = data["loc"].split(",")
    latitude = gps[0]
    longitude = gps[1]
    
# Extra credit: default it based on IP location

# Need to call InfoLink first to get info needed for forcastLink
InfoLink = 'https://api.weather.gov/points/{},{}' # {latitude},{longitude}
forcastLink = 'https://api.weather.gov/gridpoints/{}/{},{}/forecast' # {office}/{grid X},{grid Y}

response = requests.get(InfoLink.format(latitude, longitude))
resText = response.text
infoData = json.loads(resText)
gridId = infoData["properties"]["gridId"]
gridX  = infoData["properties"]["gridX"]
gridY  = infoData["properties"]["gridY"]

response = requests.get(forcastLink.format(gridId, gridX, gridY))
resText = response.text
forcastData = json.loads(resText)
# print("Weather Data " + resText)
temperature = forcastData["properties"]["periods"][0]["temperature"]
location = infoData["properties"]["relativeLocation"]["properties"]
location = location["city"] + ", " + location["state"]

print("temperature: " + str(temperature))
print("location: " + str(location))

args = "-log " + " -tmp=" + str(temperature)
print("args: " + str(args))
subprocess.run(["H:\Repos\WhatIsTheWeather\Binaries\Win64\WhatIsTheWeather.exe"], shell=False)