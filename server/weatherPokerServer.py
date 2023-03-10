#!/usr/bin/env python3

import sys
import requests
import json
import asyncio
import websockets

## Threading
# import _thread
# import subprocess
# import os

latitude = ""
longitude = ""
infoData = ""
forecastData = ""
ipData = ""

# Need to call InfoLink first to get info needed for forecastLink
InfoLink = 'https://api.weather.gov/points/{},{}' # {latitude},{longitude}
forecastLink = 'https://api.weather.gov/gridpoints/{}/{},{}/forecast' # {office}/{grid X},{grid Y}
ipInfoLink = 'http://ipinfo.io/json'

# Query for data

# see ipInfoData.json
def getLocationFromIp():
    response = requests.get(ipInfoLink)
    resText = response.text
    return json.loads(resText)

# see InfoData.json
def getInfoData():
    response = requests.get(InfoLink.format(latitude, longitude))
    resText = response.text
    return json.loads(resText)

# see forecastData.json
def getForecastData(infoData):
    gridId = infoData["properties"]["gridId"]
    gridX  = infoData["properties"]["gridX"]
    gridY  = infoData["properties"]["gridY"]

    response = requests.get(forecastLink.format(gridId, gridX, gridY))
    resText = response.text
    return json.loads(resText)

### Forecast Processing

def getInt(fullString, startSection, endSection):
    result = 0

    startIndex = fullString.find(startSection)
    if startIndex > -1:
        startIndex += len(startSection)
        endIndex    = fullString.find(endSection, startIndex)
        result      = int(fullString[startIndex:endIndex])

    return result

def getPrecipitation(detailedForecast):
    # print("getPrecipitation")
    precipitation = getInt(detailedForecast, "precipitation is ", "%")
    return precipitation

def getGustSpeed(detailedForecast):
    print("getPrecipitation")

    gustSpeed = getInt(detailedForecast, "gusts as high as", "mph")

    return gustSpeed

# This does some extra data siphoning over the forecast data
# this will allow apps to make direct calls to get more precise info with out having
# to resort to string interpretation.
def processForecastData(forecastData):
    # print("processForecastData")

    periods = forecastData["properties"]["periods"]
    for period in periods:
        period["speedUnit"]     = "mph"
        period["precipitation"] = getPrecipitation(period["detailedForecast"])
        period["gustSpeed"]     = getGustSpeed(period["detailedForecast"])

# Search through data

def getLocal(infoData):
    location = infoData["properties"]["relativeLocation"]["properties"]
    return location["city"] + ", " + location["state"]

def getTemp(forecastData):
    firstPeriod = forecastData["properties"]["periods"][0]
    return (
        str(firstPeriod["temperature"]) + "°" + firstPeriod["temperatureUnit"]
    )

def getDetail(forecastData):
    return str(forecastData["properties"]["periods"][0]["detailedForecast"]) 

# These need to come from command line
numArgs = len(sys.argv)
if numArgs >= 3:
    latitude = sys.argv[1]
    longitude = sys.argv[2]
else :
    ipData = getLocationFromIp()
    gps = ipData["loc"].split(",")
    latitude = gps[0]
    longitude = gps[1]

async def receiver(websocket, path):
    action = await websocket.recv()
    print(f"< {action}")
    infoData = getInfoData()
    # print("infoData:" + str(infoData))
    forecastData = getForecastData(infoData)
    # print("forecastData:" + str(forecastData))

    if action == "get-temp":
        response = getTemp(forecastData)
    elif action == "get-local":
        response = getLocal(infoData)
    elif action == "get-detail":
        response = getDetail(forecastData)
    elif action == "exit":
        response = "ok bye"
        websocket.close()

    await websocket.send(response)
    print(f"> {response}")

# def launchApp():
#     dir = os.path.dirname(__file__)
#     path = os.path.join(dir, 'WhatIsTheWeather.exe')
#     path = "WhatIsTheWeather.exe" # Path to exe file
#     print("path:" + path)
#     subprocess.run([path], shell=False)

# try:
#    _thread.start_new_thread( launchApp, ( ) )
# except:
#    print ("Error: unable to start thread")

# start_server = websockets.serve(receiver, "localhost", 5000)
# asyncio.get_event_loop().run_until_complete(start_server)
# asyncio.get_event_loop().run_forever()

# Test Server

infoData = getInfoData()
# print("infoData:" + str(infoData))
forecastData = getForecastData(infoData)
# print("forecastData:" + str(forecastData))

processForecastData(forecastData)