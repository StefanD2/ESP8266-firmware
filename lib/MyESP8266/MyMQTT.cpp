#include "MyMQTT.hpp"

// --------------------------- public functions ----------------------------

MyMQTT::MyMQTT(){
	callbacks.reserve(2);
	nextReconnect = 0;
}

void MyMQTT::setConfig(config_t config){
	this->config = config;
	client.setServer(config.mqtt.ip.c_str(),config.mqtt.port);
	client.setCallback([this] (char* topic, byte* payload, unsigned int length) { this->callback(topic, payload, length); });
	client.setClient(wificlient);
	begin();
}

boolean MyMQTT::publish(payload_t payload){
	return client.publish(payload.topic.c_str(),payload.msg.c_str());
}

void MyMQTT::addCallback(std::function<void(payload_t)> callback){ //FIXME
	callbacks.push_back(callback);
}

void MyMQTT::begin(){
	loop();
}

void MyMQTT::loop(){
	// stay connected
	int connectionAttempt = 0;
	while(!client.connected() && nextReconnect < millis()) {
		if(client.connect(config.wifi.host.c_str())){
			lastTime = 0;
			subscribe();
		}
		connectionAttempt++;
		if(connectionAttempt > 3){
			nextReconnect = millis() + 900000L;
		}
		yield();
	}
	if(millis() - lastTime > 1000){
		publish({"BROADCAST/ONLINE",config.wifi.host + " " + millis()});
		lastTime = millis();
	}
	client.loop();
}

// --------------------------- private functions ---------------------------

void MyMQTT::callback(char* topic, byte* payload, unsigned int length){
	payload_t payloadS;
	payloadS.topic = String(topic);
	payloadS.msg = "";
	for(unsigned int i = 0; i < length; i++)
		payloadS.msg += (char) payload[i];
	// call callback functions
	for(size_t i = 0; i < callbacks.size(); i++)
		callbacks[i](payloadS);
}

void MyMQTT::subscribe(){
	size_t topicCounter = 0;
	for(unsigned int i = 0; i < config.mqtt.topics.length(); i++){
		if(config.mqtt.topics.charAt(i) == ';')
			topicCounter++;
	}
	unsigned int lastPos = 0;
	String topics = config.mqtt.topics;
	topics.replace("*host*",config.wifi.host);
	for(size_t i = 0; i < topicCounter; i++){
		String sub = topics.substring((lastPos?(lastPos+1):lastPos),(lastPos = (topics.indexOf(';',(lastPos+1)))));
		client.subscribe(sub.c_str());
	}
}
