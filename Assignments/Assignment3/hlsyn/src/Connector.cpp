/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: Connector.cpp
*************************************/
#include <string>

#include "Connector.h"

using namespace std;

Connector::Connector(void) {

}

Connector::Connector(string name, string type, int dataWidth, bool sign) {		//overload constructor for name, type, and dw
	this->size = dataWidth;	
	this->name = name;
	this->type = type;
	this->sign = sign;
	return;
}

Connector::~Connector(void) {

}