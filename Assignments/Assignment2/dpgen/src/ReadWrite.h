/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: ReadWrite.h
*************************************/



#ifndef READWRITE_H
#define READWRITE

#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include "Connector.h"
#include "Logic.h"

using namespace std;

class ReadWrite
{
public:
	bool parseFile(std::string filename);
	bool parseEdge(vector<Connector*> connectorVector, std::string inputLine);	//Parse string
	bool parseNode(vector<Logic*> logicVector, vector<Connector*> connectorVector, std::string inputLine);

private:

};

#endif