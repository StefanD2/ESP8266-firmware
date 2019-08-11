#include "MyMQTT.hpp"

// --------------------------- public functions ----------------------------

MyMQTT::MyMQTT(){
	callbacksSize = 0;
}

MyMQTT::~MyMQTT(){
	free(callbacks);
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

void MyMQTT::addCallback(std::function<void(payload_t)> callback){
	if(callbacksSize == 0)
		callbacks = (std::function<void(payload_t)>*) malloc(sizeof(std::function<void(payload_t)>));
	else
		callbacks = (std::function<void(payload_t)>*) realloc(callbacks, (callbacksSize + 1) * sizeof(std::function<void(payload_t)>));
	callbacks[callbacksSize] = callback;
	callbacksSize++;
}

void MyMQTT::begin(){
	loop();
}

void MyMQTT::loop(){
	// stay connected
	while(!client.connected()){
		Serial.println("not conn");
		if(client.connect(config.wifi.host.c_str())){
			lastTime = 0;
			subscribe();
		}
	}
	if(millis() - lastTime > 1000){
		publish({"BROADCAST/ONLINE",config.wifi.host});
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
	for(size_t i = 0; i < callbacksSize; i++)
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
		client.subscribe(topics.substring((lastPos?(lastPos+1):lastPos),(lastPos = (topics.indexOf(';',(lastPos+1))))).c_str());
	}
}
