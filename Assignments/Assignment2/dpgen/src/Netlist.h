/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: Netlist.h
*************************************/
#include <vector>

#include "Connector.h"
#include "Logic.h"

#ifndef NETLIST_H
#define NETLIST

class Netlist
{
	public:
	
		//Getters
		std::vector<Connector> get_inputs() {
			return this->get_inputs;
		}


	private:
		std::vector<Connector*> inputs;
		std::vector<Connector*> outputs;
		std::vector<Connector*> wires;
		std::vector<Connector*> registers;
		std::vector<Logic*> logics;



};

#endif

