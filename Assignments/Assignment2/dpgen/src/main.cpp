/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: main.cpp
*************************************/
//Connector Vector is empty for when we get to the logic parsing
#include <iostream>
#include <string>
#include "Netlist.h"

int main(int argc, char **argv) {
	Netlist n1;
	string filename = "";
	string filename2 = "";
	if (argc == 2) {					//check for 1 input argument
		filename = argv[1];
		if (!n1.parseFile(filename)){
			cerr << "Error: Unable to parse file." << endl;
			return -1;
		}
		cout << filename.substr(filename.rfind('\\')+1) << " file successfully parsed." << endl;
		cout << "No output filename detected, will export to dpgenDefault.txt ..." << endl;
		if (!n1.outputModule("dpgenDefault.txt")) {
			cerr << "Error: Unable to output file dpgenDefault.txt." << endl;
			return -1;
		}
		cout << "dpgenDefault.txt file successfully generated." << endl;
	}
	else if (argc == 3) {	//check for 2 input arguments
		filename = argv[1];
		if (!n1.parseFile(filename)) {
			cerr << "Error: Unable to parse file." << endl;
			return -1;
		}
		
		cout << filename.substr(filename.rfind('\\') + 1) << " file successfully parsed." << endl;
		filename2 = argv[2];
		if (!n1.outputModule(filename2)) {
			
			filename2 = filename2.substr(filename2.rfind('\\') + 1);
			cerr << "Error: Unable to output file " << filename2 << endl;
			return -1;
		}
		cout << filename2 << " file successfully generated." << endl;
	}
	else {
		std::cerr << "Usage " << argv[0] << " input output" << std::endl << std::endl;
		return -1;
	}

	n1.findCriticalPath();
	
	return 0;
}