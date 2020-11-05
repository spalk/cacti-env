![Cacti-Env Logo](devices/main-device/static/cacti-env-welcome-bitmap.bmp)

Cacti-Env 
=========
_Succulents and cacti indoor environment monitoring_

The main idea of project is to monitor environment around plants indoors and improve conditions where they live in three steps:

### First step: data collecting

The monitored parameters are:
- temperature
- light level
- relative humidity
- soil moisture

There are also other additional parameters taken into account:
- atmospheric pressure
- outside temperature
- position of sun 
- position of moon
   
All of this parameters a recorded every minute. 


### Second step: data analysing

For each parameters are defined theoretical ranges, when plants feel happy. This ranges can be adjusted according to visual conditions of plants or fresh theoretical information.

Ranges of parameters are also depend on season of the year and dormancy period of the plants. For instance winter temperature for succulents should be 10-15 degrees Celsius when they usually dormant, in summer more than 25 when they usually active grow. 

### Third step: action

In cases when some parameter is out of range actions taken to return it to normal value. 

For light deficiency compensation are used artificial grow lights. For temperature control can be used heaters. Watering can be adjusted manually according to current environment conditions and time of the year. 

------

## How it works    