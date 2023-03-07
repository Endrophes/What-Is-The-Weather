# What Is The Weather

Weather app in the Unreal Engine

## Overview

This project is done in two parts, Server and the App.

Server:

    - Python
    - Queries for weather data
    - receives calls from the app

App:

    - Unreal Engine
    - Front end
    - Updates the effects based on weather data

## To run

1. Run **weatherPokerServer.py** located in the server folder
2. Run the Unreal Engine project located in the app folder

## Install

Software required:

- Unreal engine 4.25.4
- Visual Studio 2017
  - Required for Unreal Engine 4.25.4 when running C++
- Python 3.8.9
- VsCode
  - Python Plugin

What I learned:

- Python
  - Used Linked in Learning course
    - Link: [https://www.linkedin.com/learning/python-essential-training-2018](https://www.linkedin.com/learning/python-essential-training-2018)
- NWS Web Service Api
  - Docs: [https://www.weather.gov/documentation/services-web-api](https://www.weather.gov/documentation/services-web-api)
  - takes two links to get the needed data

## Notes

### Weather API

Docs: [https://www.weather.gov/documentation/services-web-api](https://www.weather.gov/documentation/services-web-api)

Test link for Weather data
  
1. Link 1: [https://api.weather.gov/points/39.7456,-97.0892](https://api.weather.gov/points/39.7456,-97.0892)
   - Returns general data in regards to coordinates inputted
   - Used for link two

2. Link 2: [https://api.weather.gov/gridpoints/TOP/31,80/forecast](https://api.weather.gov/gridpoints/TOP/31,80/forecast)
   - Returns forecast on given location

Results:

InfoData.json

Target value(s):

- `data["properties"]["gridId"]`
- `data["properties"]["gridX"]`
- `data["properties"]["gridY"]`

Using these values I can now use the second link to provide all my data

See `forcastData.json`

primary target value: `data["properties"]["periods"][0]["temperature"]`

### Python

- [Python 3.9.5 documentation](https://docs.python.org/3.9/)
- [Threading](https://docs.python.org/3/library/threading.html)
