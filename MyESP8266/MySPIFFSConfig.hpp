#ifndef __MYSPIFFSCONFIG_HPP__
#define __MYSPIFFSCONFIG_HPP__

#ifndef ESP8266
	#error This library is only working for an ESP8266.
#endif

#include <Arduino.h>
#include "MySPIFFS.hpp"

// path for WiFi settings
#define MYSPIFFSCONFIGCONSTANTS_PATH_WIFI "/config/wifi.json"

// path for update server settings
#define MYSPIFFSCONFIGCONSTANTS_PATH_UPDATESERVER "/config/updateServer.json"

// path for mqtt settings
#define MYSPIFFSCONFIGCONSTANTS_PATH_MQTT "/config/mqtt.json"


typedef struct {
	String ssid;
	String pw;
	String host;	
} config_wifi_t;

typedef struct {
	String ip;
	int port;
	String topics;
} config_mqtt_t;

typedef struct {
	String path;
	String user;
	String pw;
} config_updateServer_t;

typedef struct {
	config_wifi_t wifi;
	config_mqtt_t mqtt;
	config_updateServer_t updateServer;
} config_t;

class MySPIFFSConfig{
	public:
		MySPIFFSConfig();

		// get Configuration
		config_t getConfig();
		config_wifi_t getConfigWifi();
		config_mqtt_t getConfigMqtt();
		config_updateServer_t getConfigUpdateServer();

		// set Configuration (return true if success)
		boolean setConfig(config_t config);
		boolean setConfigWifi(config_wifi_t config);
		boolean setConfigMqtt(config_mqtt_t config);
		boolean setConfigUpdateServer(config_updateServer_t config);
		
	private:
		MySPIFFS wifi;
		MySPIFFS updateServer;
		MySPIFFS mqtt;
};

#endif /*__MYSPIFFSCONFIG_H__*/