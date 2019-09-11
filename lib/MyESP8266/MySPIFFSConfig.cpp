#include "MySPIFFSConfig.hpp"

MySPIFFSConfig::MySPIFFSConfig():wifi(MYSPIFFSCONFIGCONSTANTS_PATH_WIFI), updateServer(MYSPIFFSCONFIGCONSTANTS_PATH_UPDATESERVER), mqtt(MYSPIFFSCONFIGCONSTANTS_PATH_MQTT){
	SPIFFS.begin();
}

// --------------------------- get configuration ---------------------------

config_t MySPIFFSConfig::getConfig(){
	config_t config;
	config.wifi = getConfigWifi();
	config.mqtt = getConfigMqtt();
	config.updateServer = getConfigUpdateServer();
	return config;
}

config_wifi_t MySPIFFSConfig::getConfigWifi(){
	config_wifi_t config;
	config.ssid = wifi.get("SSID");
	config.pw = wifi.get("PW");
	config.host = wifi.get("HOST");
	return config;
}

config_mqtt_t MySPIFFSConfig::getConfigMqtt(){
	config_mqtt_t config;
	config.ip = mqtt.get("IP");
	config.port = atoi(mqtt.get("PORT").c_str());
	config.topics = mqtt.get("TOPICS");
	return config;
}

config_updateServer_t MySPIFFSConfig::getConfigUpdateServer(){
	config_updateServer_t config;
	config.path = updateServer.get("PATH");
	config.user = updateServer.get("USER");
	config.pw = updateServer.get("PW");
	return config;
}

// --------------------------- set configuration ---------------------------

boolean MySPIFFSConfig::setConfig(config_t config){
	boolean success = 1;
	success = success && setConfigWifi(config.wifi);
	success = success && setConfigMqtt(config.mqtt);
	success = success && setConfigUpdateServer(config.updateServer);
	return success;
}

boolean MySPIFFSConfig::setConfigWifi(config_wifi_t config){
	boolean success = 1;
	success = success && wifi.write("SSID",config.ssid);
	success = success && wifi.write("PW",config.pw);
	success = success && wifi.write("HOST",config.host);
	return success;
}

boolean MySPIFFSConfig::setConfigMqtt(config_mqtt_t config){
	boolean success = 1;
	success = success && mqtt.write("IP",config.ip);
	success = success && mqtt.write("PORT",config.port);
	success = success && mqtt.write("TOPICS",config.topics);
	return success;
}

boolean MySPIFFSConfig::setConfigUpdateServer(config_updateServer_t config){
	boolean success = 1;
	success = success && updateServer.write("PATH",config.path);
	success = success && updateServer.write("USER",config.user);
	success = success && updateServer.write("PW",config.pw);
	return success;
}
