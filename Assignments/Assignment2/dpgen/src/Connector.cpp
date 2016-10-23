/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: Connector.cpp
*************************************/
#include <string>

#include "Connector.h"
using namespace std;

Connector::Connector() {

}

Connector::Connector(string name, string type, int dataWidth) {		//overload constructor for name, type, and dw
	this->size = dataWidth;
	this->delay = 0;
	this->name = name;
	return;
}

Connector::~Connector() {

}