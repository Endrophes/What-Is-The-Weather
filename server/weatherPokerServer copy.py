#!/usr/bin/env python3
# Copyright 2009-2017 BHG http://bw.org/

import sys
import requests
import json
import subprocess
import os
import asyncio
import websockets
import _thread
import time

latitude = ""
longitude = ""
infoData = ""
forcastData = ""
ipData = ""
run = True

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

def launchApp():
    path = "H:\Repos\WhatIsTheWeather\Content\Outputs\WindowsNoEditor\WhatIsTheWeather.exe"
    subprocess.run([path], shell=True)
    # os.system("path")

def startServer(loop):
    start_server = websockets.serve(receiver, "localhost", 5000)
    asyncio.set_event_loop(loop)
    asyncio.new_event_loop().run_until_complete(start_server)
    asyncio.new_event_loop().run_forever()

def print_time( threadName, delay):
   count = 0
   while count < 5:
      time.sleep(delay)
      count += 1
      print ("%s: %s" % ( threadName, time.ctime(time.time()) ))

print("HELLO!!!!")

# loop = asyncio.get_event_loop()

# Create two threads as follows

# _thread.start_new_thread( startServer, (loop,) )
# _thread.start_new_thread( launchApp, () )

# try:
#     _thread.start_new_thread( startServer, (loop) )
#     _thread.start_new_thread( launchApp, () )
#     # _thread.start_new_thread( print_time, ("Thread-1", 2 ) )
#     # _thread.start_new_thread( print_time, ("Thread-2", 4 ) )
# except:
#    print("Error: unable to start thread")

# while run:
#    pass

start_server = websockets.serve(receiver, "localhost", 5000)
asyncio.new_event_loop().run_until_complete(start_server)

# launchApp()

# asyncio.new_event_loop().run_forever()