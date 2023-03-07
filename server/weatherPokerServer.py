#!/usr/bin/env python3

import sys
import requests
import json
import subprocess
import os
import asyncio
import websockets
import _thread

latitude = ""
longitude = ""
infoData = ""
forcastData = ""
ipData = ""

# Need to call InfoLink first to get info needed for forcastLink
InfoLink = 'https://api.weather.gov/points/{},{}' # {latitude},{longitude}
forcastLink = 'https://api.weather.gov/gridpoints/{}/{},{}/forecast' # {office}/{grid X},{grid Y}
ipInfoLink = 'http://ipinfo.io/json'

def getLocationFromIp():
    response = requests.get(ipInfoLink)
    resText = response.text
    return json.loads(resText)

def getInfoData():
    response = requests.get(InfoLink.format(latitude, longitude))
    resText = response.text
    return json.loads(resText)

def getForcastData(infoData):
    gridId = infoData["properties"]["gridId"]
    gridX  = infoData["properties"]["gridX"]
    gridY  = infoData["properties"]["gridY"]

    response = requests.get(forcastLink.format(gridId, gridX, gridY))
    resText = response.text
    return json.loads(resText)

def getLocal(infoData):
    location = infoData["properties"]["relativeLocation"]["properties"]
    return location["city"] + ", " + location["state"]

def getTemp(forcastData):
    return str(forcastData["properties"]["periods"][0]["temperature"]) + "°F"

def getDetail(forcastData):
    return str(forcastData["properties"]["periods"][0]["detailedForecast"]) 

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
    forcastData = getForcastData(infoData)
    # print("forcastData:" + str(forcastData))

    if action == "get-temp":
        response = getTemp(forcastData)
    elif action == "get-local":
        response = getLocal(infoData)
    elif action == "get-detail":
        response = getDetail(forcastData)
    elif action == "exit":
        response = "ok bye"
        websocket.close()

    await websocket.send(response)
    print(f"> {response}")

# def launchApp():
#     dir = os.path.dirname(__file__)
#     path = os.path.join(dir, 'WhatIsTheWeather.exe')
#     path = "C:\Users\\alexa\Desktop\BA-VerisonSubmission\WhatsTheWeather-exe\WhatIsTheWeather.exe"
#     print("path:" + path)
#     subprocess.run([path], shell=False)

# try:
#    _thread.start_new_thread( launchApp, ( ) )
# except:
#    print ("Error: unable to start thread")

start_server = websockets.serve(receiver, "localhost", 5000)
asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()