#include "MyESP8266.hpp"

// ------------------------------------------------------ public functions ------------------------------------------------------

MyESP8266::MyESP8266(){
	MyESP8266(nullptr);
}

MyESP8266::MyESP8266(std::function<void(void)> alwaysRunning):mySPIFFSConfig(),mqtt(),server(),updater(){
	this->alwaysRunning = alwaysRunning;
}

void MyESP8266::begin(){
	config = mySPIFFSConfig.getConfig();
	WiFi.mode(WIFI_STA);
	WiFi.begin(config.wifi.ssid.c_str(),config.wifi.pw.c_str());
	while(WiFi.status() != WL_CONNECTED){
		if(alwaysRunning)
			alwaysRunning();
		yield(); // reset the Wachtdog
	}
	onDissconnect = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected & event) {
		ESP.restart();
	});
	Serial.println(WiFi.localIP());
	MDNS.begin(config.wifi.host.c_str());
	updater.setup(&server, config.updateServer.path.c_str(), config.updateServer.user.c_str(), config.updateServer.pw.c_str());
	server.begin();
	MDNS.addService("http", "tcp", 80);
	mqtt.setConfig(config);
	mqtt.begin(); // add always running funtion
	mqtt.addCallback([this](payload_t payload){this->callback_config(payload);});
}

void MyESP8266::loop(){
	mqtt.loop();
	server.handleClient();
	MDNS.update();
	if(alwaysRunning)
		alwaysRunning();
	// millis reset
	if(millis()>4294967196){
		ESP.restart();
	}
}


void MyESP8266::publish(payload_t payload){
	mqtt.publish(payload);
}

void MyESP8266::addCallback(std::function<void(payload_t)> callback){
	mqtt.addCallback(callback);
}

config_t MyESP8266::getConfig(){
	return config;
}

// ------------------------------------------------------ private functions -----------------------------------------------------

void MyESP8266::callback_config(payload_t payload){
	if((payload.topic.indexOf(config.wifi.host + "/config") + 1)){
		if(payload.topic.endsWith("/get") && payload.msg.equals("get")){
			payload.msg = "wifi:{ssid:" + config.wifi.ssid + ",pw:" + config.wifi.pw + ",host:" + config.wifi.host + "},";
			publish(payload);
			payload.msg = "mqtt:{ip:" + config.mqtt.ip + ",port:" + config.mqtt.port + ",topics:" + config.mqtt.topics + "},";
			publish(payload);
			payload.msg = "updateServer:{path:" + config.updateServer.path + ",user:" + config.updateServer.user + ",pw:" + config.updateServer.pw + "}";
			publish(payload);
		}
		else if(payload.topic.endsWith("/set")){
			if(payload.msg.startsWith("wifi:ssid:")){
				config.wifi.ssid = payload.msg.substring(payload.msg.indexOf(':',5)+1);
			}
			else if(payload.msg.startsWith("wifi:pw:")){
				config.wifi.pw = payload.msg.substring(payload.msg.indexOf(':',5)+1);
			}
			else if(payload.msg.startsWith("wifi:host:")){
				config.wifi.host = payload.msg.substring(payload.msg.indexOf(':',5)+1);
			}
			else if(payload.msg.startsWith("mqtt:ip:")){
				config.mqtt.ip = payload.msg.substring(payload.msg.indexOf(':',5)+1);
			}
			else if(payload.msg.startsWith("mqtt:port:")){
				config.mqtt.port = atoi(payload.msg.substring(payload.msg.indexOf(':',5)+1).c_str());
			}
			else if(payload.msg.startsWith("mqtt:topics:")){
				config.mqtt.topics = payload.msg.substring(payload.msg.indexOf(':',5)+1);
			}
			else if(payload.msg.startsWith("updateServer:path:")){
				config.updateServer.path = payload.msg.substring(payload.msg.indexOf(':',13)+1);
			}
			else if(payload.msg.startsWith("updateServer:user:")){
				config.updateServer.user = payload.msg.substring(payload.msg.indexOf(':',13)+1);
			}
			else if(payload.msg.startsWith("updateServer:pw:")){
				config.updateServer.pw = payload.msg.substring(payload.msg.indexOf(':',13)+1);
			}
			mySPIFFSConfig.setConfig(config);
		}
		else if(payload.topic.endsWith("/restart") && payload.msg.equals("restart")){
			ESP.restart();
		}
	}
}
