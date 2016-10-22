/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: Connector.h
*************************************/


#ifndef CONNECTOR_H
#define CONNECTOR

#include <string>
using namespace std;

class Connector	//edges
{
public:
	Connector();
	Connector(string name, string type, int dataWidth) { this->size = dataWidth;	this->delay = 0; this->name = name; };				//constructor
	~Connector() {};			//destructor
	//Setters
	void SetType(std::string type) { this->type = type; }
	void SetName(std::string name) { this->name = name; }
	void SetSize(int size) { this->size = size; } 
	void SetSign(bool sign) { this->sign = sign; }
	void SetVisited(bool visited) { this->visited = visited; }
	void SetDelay(float delay) { this->delay = delay; }
	//Getters
	std::string GetType() { return this->type; }
	std::string GetName() { return this->name; }
	int GetSize() { return this->size; }
	bool GetSign() { return this->sign; }
	bool GetVisited() { return this->visited; }
	float GetDelay() { return this->delay; }

private:
	std::string type;	//type of connector (input, output, wire, register)
	std::string name;	//name of the variable
	int size;			//data width
	bool sign;			//signed or unsigned
	bool visited;		//check whether this edge has been visited or not when scanning through the list
	float delay;		//delay to this point
	vector <Logic*> logicDown;	//list of all node input connections this connector/edge is pointing to

};

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


#endif