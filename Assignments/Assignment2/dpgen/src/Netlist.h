/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: Netlist.h
*************************************/
#ifndef NETLIST_H
#define NETLIST_H

#include <vector>

#include "Logic.h"

using namespace std;

class Netlist
{
public:
	Netlist(void);
	~Netlist(void);

	//Getters
	vector<Connector*> GetInputs() { return this->edges; }


private:
	vector<Connector*> edges;
	vector<Logic*> nodes;
};

#endif

