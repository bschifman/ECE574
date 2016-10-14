/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: ReadWrite.h
*************************************/

#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#ifndef READWRITE_H
#define READWRITE

class ReadWrite
{
public:
	bool parseFile(string filename); 
	bool parseEdge(vector<Connector*> connectorVector, string inputLine);
	bool parseNode(vector<Logic*> logicVector, vector<Connector*> connectorVector, string inputLine);

private:

};

#endif