# MyESP8266Config.h
Configurations are split in 3 different files:
## wifi:
* SSID
* PW
* HOST
>{"SSID":"your-ssid","PW":"your-password","HOST":"your-hostname"}

## mqtt:
* IP
* PORT
* TOPICS (written as a String, separated with a semi-column, spaces befor and after ; don't matter, if a topic contains \*host\* this will get replaced with the name of the host specified in the wifi configs 
> {"IP":"raspberrpi.local","PORT":1883,"TOPICS":"BROADCAST/#; \*host\*/#"}

## update Server
* PATH
* USER
* PW
> {"PATH":"/firmware","USER":"Stefan","PW":"password"}

## mqtt callback
TOPIC: \*host\*/config
payload get/set wif/mqtt/updateServer 
