/***********************************************************
* 
* MyESP8266.h - All basic functions for an esp8266 
* 
* This library uses the httpUpdateServer and mqtt
* 
* Author: Stefan Deimel
* Date: 28.02.2019
* 
***********************************************************/

#ifndef __MYESP8266_HPP__
#define __MYESP8266_HPP__

#ifndef ESP8266
	#error This library is only working for an ESP8266.
#endif

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <PubSubClient.h>
#include <functional>
#include "MyMQTT.hpp"
#include "MySPIFFSConfig.hpp"

class MyESP8266{	
	public:
		MyESP8266();
		MyESP8266(std::function<void(void)> alwaysRunning);
		void begin();
		void loop();
		
		void publish(payload_t payload);
		void addCallback(std::function<void(payload_t)> callback);

		config_t getConfig();

	private:
		MySPIFFSConfig mySPIFFSConfig;
		MyMQTT mqtt;
		ESP8266WebServer server;
		ESP8266HTTPUpdateServer updater;
		WiFiEventHandler onDissconnect;
		config_t config;
		void callback_config(payload_t payload);
		std::function<void(void)> alwaysRunning;
};

#endif /*__MYESP8266_H__*/
