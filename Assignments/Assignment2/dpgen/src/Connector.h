/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: Connector.h
*************************************/
#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <string>

#include "Logic.h"

using namespace std;




class Connector	//edges
{
public:
	Connector(void);
	Connector(string name, string type, int dataWidth, bool sign);	//overload constructor
	~Connector(void);												//destructor

	//Setters
	void SetType(string type) { this->type = type; }
	void SetName(string name) { this->name = name; }
	void SetSize(int size) { this->size = size; } 
	void SetSign(bool sign) { this->sign = sign; }
	void SetVisited(bool visited) { this->visited = visited; }
	void SetDelay(float delay) { this->delay = delay; }
	void SetParent(Logic* parentToAdd) { this->parent = parentToAdd; }
	void AddChild(Logic* childToAdd) { this->children.push_back(childToAdd); }

	//Getters
	Logic* GetParent() { return this->parent; }
	string GetType() { return this->type; }
	string GetName() { return this->name; }
	int GetSize() { return this->size; }
	bool GetSign() { return this->sign; }
	bool GetVisited() { return this->visited; }
	float GetDelay() { return this->delay; }


private:
	Logic *parent;				//parent node of this edge
	vector<Logic*> children;	//children nodes this edge is connected to
	string type;				//type of connector (input, output, wire, register)
	string name;				//name of the variable
	int size;					//data width
	bool sign;					//signed(1) or unsigned(0) 
	bool visited;				//check whether this edge has been visited or not when scanning through the list
	float delay;				//delay to this point
};



#endif