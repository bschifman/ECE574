/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: Connector.h
*************************************/
#include <string>

#ifndef CONNECTOR_H
#define CONNECTOR

class Connector	//edges
{
public:
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
		int size;
		bool sign;
		bool visited;
		float delay;
		vector <Logic*> logicDown;	//list of all node input connections this connector/edge is connected to

};


#endif