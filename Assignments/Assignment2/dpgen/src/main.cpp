/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: main.cpp
*************************************/
//Connector Vector is empty for when we get to the logic parsing
#include <iostream>

#include "Netlist.h"

int main(int argc, char **argv) {
	if (argc == 2) {					//check for correct argument number
		Netlist n1;
		if (!n1.parseFile(argv[1])){
			cerr << "Unable to parse file." << endl;
			return -1;
		}



	}

	else {
		std::cerr << "Usage " << argv[0] << " input output" << std::endl << std::endl;
		return -1;
	}



	return 0;
}