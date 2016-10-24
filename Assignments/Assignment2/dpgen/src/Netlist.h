/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: Netlist.h
*************************************/
#ifndef NETLIST_H
#define NETLIST_H

#include <vector>
#include "Connector.h"
#include "Logic.h"

using namespace std;

class Netlist
{
public:
	Netlist(void);
	~Netlist(void);

	bool parseFile(string filename);
	bool parseEdge(string inputLine);	//Parse string
	bool parseNode(string inputLine);
	Connector* findEdge(string edgeName);

	//Getters
	vector<Connector*> GetEdges() { return this->edges; }
	vector<Logic*> GetNodes() { return this->nodes; }


private:
	vector<Connector*> edges;
	vector<Logic*> nodes;
};

#endif

