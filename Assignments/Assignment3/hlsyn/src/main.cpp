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
	string latency = "";
	if (argc == 3) {					//check for 1 input argument
		filename = argv[1];
		latency = argv[2];
		if (!n1.parseFile(filename, latency)){	
			cerr << "Error: Unable to parse file." << endl;
			return -1;
		}
		cout << filename.substr(filename.rfind('\\')+1) << " file successfully parsed." << endl;	//reports what file was parsed
		cout << "No output filename detected, will export to hlsynDefault.txt ..." << endl;
		if (!n1.outputHLSMModule("hlsynDefault.txt")) {				//if there is no specified output filename, generate a default output file
			cerr << "Error: Unable to output file hlsynDefault.txt." << endl;
			return -1;
		}
		cout << "hlsynDefault.txt file successfully generated." << endl;
	}
	else if (argc == 4) {	//check for 2 input arguments
		filename = argv[1];
		latency = argv[2];
		if (!n1.parseFile(filename, latency)) {		//if there is a output filename specified, generate that file
			cerr << "Error: Unable to parse file." << endl;
			return -1;
		}
		
//		cout << filename.substr(filename.rfind('\\') + 1) << " file successfully parsed." << endl;
		filename2 = argv[3];
		if (!n1.outputHLSMModule(filename2)) {
			
			filename2 = filename2.substr(filename2.rfind('\\') + 1);
			cerr << "Error: Unable to output file " << filename2 << endl;
			return -1;
		}
//		cout << filename2 << " file successfully generated." << endl;
	}
	else {	//if there are not a valid number of input arguments then print file useage statement
		std::cerr << "Usage " << argv[0] << " input latency output" << std::endl << std::endl;
		return -1;
	}

	n1.findCriticalPath();	//determine and output the critical path
	
	return 0;
}