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




class Connector	//edges in a directed graph
{
public:
	Connector(void);
	Connector(string name, string type, int dataWidth, bool sign);	//overload constructor
	~Connector(void);												//destructor

	//Setters
	void SetType(string type) { this->type = type; }					//set this edges type (input, output, wire, register)
	void SetName(string name) { this->name = name; }					//set the variable name of this edge (ie a,b,c, xwire, etc)
	void SetSize(int size) { this->size = size; }						//set the bit width of this edge(ie 1,2,4,8,16,32,64, etc)
	void SetSign(bool sign) { this->sign = sign; }						//set whether this edge is a signed or unsigned datapath
	void SetVisited(bool visited) { this->visited = visited; }			//set if this edge has been visited in a search algorithm
	void SetDelay(float delay) { this->delay = delay; }					//set the delay of the critical path at this edge
	void SetParent(Logic* parentToAdd) { this->parent = parentToAdd; }	//set this edges parent node(this is a directed graph)
	void AddChild(Logic* childToAdd) { this->children.push_back(childToAdd); }	//add a child node to this edge(this is a directed graph)
	void SetEdgeALAP(int inputTime) { this->alapt = inputTime; }					//set ALAP time
	void SetEdgeASAP(int inputTime) { this->asapt = inputTime; }					//set ASAP time

	//Getters
	Logic* GetParent() { return this->parent; }		//get this edges parent node(this is a directed graph)
	vector<Logic*> GetChildVector() { return this->children; }		//get this edges children vector
	string GetType() { return this->type; }			//get this edges type (input, output, wire, register)
	string GetName() { return this->name; }			//get the variable name of this edge (ie a,b,c, xwire, etc)
	int GetSize() { return this->size; }			//get the bit width of this edge(ie 1,2,4,8,16,32,64, etc)
	bool GetSign() { return this->sign; }			//get whether this edge is a signed or unsigned datapath
	bool GetVisited() { return this->visited; }		//check to see if this edge has been visited in a search algorithm
	float GetDelay() { return this->delay; }		//get the delay of the critical path at this edge
	int GetEdgeALAP() { return this->alapt; }										//get ALAP time
	int GetEdgeASAP() { return this->asapt; }										//get ASAP time


private:
	Logic *parent;				//parent node of this edge
	vector<Logic*> children;	//children nodes this edge is connected to
	string type;				//type of connector (input, output, wire, register)
	string name;				//name of the variable
	float delay;				//delay to this point
	int asapt;								//sets the ASAP time
	int alapt;								//sets the ALAP time
	int size;					//data width
	bool sign;					//signed(1) or unsigned(0) 
	bool visited;				//check whether this edge has been visited or not when scanning through the list
	
};



#endif