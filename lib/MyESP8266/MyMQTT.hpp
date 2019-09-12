/***********************************************************
*
* MyMQTT.h - All basic functions for mqtt
* This library uses the PubSubClient
*
* Author: Stefan Deimel
* Date: 28.02.2019
*
***********************************************************/

#ifndef __MYMQTT_HPP__
#define __MYMQTT_HPP__

#ifndef ESP8266
	#error This library is only working for an ESP8266.
#endif

#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <functional>
#include <vector>
#include "MySPIFFSConfig.hpp"

typedef struct {
	String topic;
	String msg;
} payload_t;


class MyMQTT{
	public:
		MyMQTT();
		void setConfig(config_t config);
		boolean publish(payload_t payload);
		void addCallback(std::function<void(payload_t)> callback);
		void begin();
		void loop();
	private:
		void callback(char* topic, byte* payload, unsigned int length);
		void subscribe();
		config_t config;
		PubSubClient client;
		WiFiClient wificlient;
		std::vector<std::function<void(payload_t)>> callbacks;
		unsigned long nextReconnect;
		unsigned long lastTime;
		
		
};

#endif /*__MYMQTT_H__*/