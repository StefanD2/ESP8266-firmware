/***********************************************************
*
* MySPIFFS.h - write and read json objects and save them 
* on the SPIFFS
*
* Author: Stefan Deimel
* Date: 17.03.2019
*
***********************************************************/

#ifndef __MYSPIFFS_HPP__
#define __MYSPIFFS_HPP__

#ifndef ESP8266
	#error This library is only working for an ESP8266.
#else
	
#include <Arduino.h>
#include <FS.h>
#include <ArduinoJson.h>

//TODO alles auf Strings umstellen ??????

//TODO fix writing / reading

// test sketch D:\Stefan\Arduino\myesp8266_spiffs_test\myesp8266_spiffs_test.ino

// add function to get value always as string

class MySPIFFS {
	public:
		// absolute path of the file
		MySPIFFS(char* path){
			_path = path;
		}
		~MySPIFFS(){}
		
		// delete file
		boolean remove(){
			return SPIFFS.remove(_path);
		}
		
		// remove a key
		boolean remove(char* key){
			File file = SPIFFS.open(_path,"r+");
			if(!file) // could not open file
				return 0;	
			DynamicJsonBuffer jbuf;
			JsonObject& jobject = jbuf.parseObject(file);
			jobject.remove(key);
			jobject.printTo(file);
			file.close();
			return 1;
		}
		
		// returns value of key as String
		String get(char* key){
			File file = SPIFFS.open(_path,"r");
			if(!file) // could not open file
				return "";
			DynamicJsonBuffer jbuf;
			JsonObject& jobject = jbuf.parseObject(file);
			// jobject.prettyPrintTo(Serial);
			file.close();
			if(!jobject.containsKey(key)) // key doesn't exist
				return "";
			return jobject.get<String>(key);
		}


		template <typename type>
		boolean read(char* key, type* value){
			File file = SPIFFS.open(_path,"r");
			if(!file) // could not open file
				return 0;
			//Serial.println("file");
			//Serial.println(file);
			DynamicJsonBuffer jbuf;
			JsonObject& jobject = jbuf.parseObject(file);
			// jobject.prettyPrintTo(Serial);
			file.close();
			if(!jobject.containsKey(key) || !jobject.is<type>(key)) // key doesn't exist or type is wrong
				return 0;
			*value = (type) jobject.get<type>(key);
			return 1;
		}
		
		template <typename type> 
		boolean write(char* key, type value, boolean override = false){
			if(!SPIFFS.exists(_path) || override){ // if file doesn't exist create it
				File file = SPIFFS.open(_path,"w");
				file.close();
			}
			File fileIn = SPIFFS.open(_path,"r"); // nachschauen ob man das neicht doch  mit nur einem file machen kann
			if(!fileIn) // could not open file
				return 0;
			DynamicJsonBuffer jbuf;
			// Serial << "filesize:" << fileIn.size() << endl;
			JsonObject& jobject = (fileIn.size()?jbuf.parseObject(fileIn):jbuf.createObject()); // if file contains elements parse them into json object, else create new json object
			fileIn.close();			
			jobject.set(key,value);
			File fileOut = SPIFFS.open(_path, "w");
			if(!fileOut)  // could not open file
				return 0;
			jobject.printTo(fileOut);
			// jobject.printTo(Serial);
			// Serial << "filesize end:" << fileOut.size() << endl;
			fileOut.close();
			return 1;
		}
		
	private:
		char* _path;

};

#endif /*ESP8266*/

#endif /*__MYSPIFFS_H__*/
