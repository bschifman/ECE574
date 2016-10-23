/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: ReadWrite.h
*************************************/
#ifndef READWRITE_H
#define READWRITE_H

#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include "Connector.h"

using namespace std;

class ReadWrite
{
public:
	bool parseFile(string filename);
	bool parseEdge(vector<Connector*> connectorVector, string inputLine);	//Parse string
	bool parseNode(vector<Logic*> logicVector, vector<Connector*> connectorVector, string inputLine);

private:

};

#endif