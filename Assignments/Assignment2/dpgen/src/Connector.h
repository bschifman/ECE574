/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: Connector.h
*************************************/
#include <string>

#ifndef CONNECTOR_H
#define CONNECTOR

class Connector
{
	public:
		//Setters
		void set_type(std::string type) { this->type = type; }
		void set_name(std::string name) { this->name = name; }
		void set_size(int size) { this->size = size; } 
		void set_sign(bool sign) { this->sign = sign; }
		void set_delay(float delay) { this->delay = delay; }
		//Getters
		std::string get_type() { return this->type; }
		std::string get_name() { return this->name; }
		int get_size() { return this->size; }
		bool get_sign() { return this->sign; }
		float get_delay() { return this->delay; }

	private:
		std::string type;	//type of connector (input, output, wire, register)
		std::string name;
		int size;
		bool sign;
		float delay;

};


#endif