/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: Netlist.cpp
*************************************/
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

#include "Netlist.h"


using namespace std;

bool Netlist::parseFile(string filename) {

	Logic *tempLogic = NULL;
	//vector<Logic*> logicVector;			//list of logic items
	//vector<Connector*> *connectorVector = new vector<Connector*>();	//list of connector items
	bool CorrectFormat = false;
	string line;
	fstream inFile;					    // Input file stream

	inFile.open(filename.c_str());		//opens file

	if (!inFile.is_open()) {			//check if it does not build
		cout << "Could not open text file." << endl;
		inFile.close();//close the input file
		return false;
	}

	else if (inFile.is_open()) {//if file is opens

		while (!inFile.eof()) {			// then while the file does not end, keep going through the loop
			getline(inFile, line);		// grabs the entire line of the input file
			if (!line.empty()) {			// checks for empty line

				if ((line.find("input") != string::npos) || (line.find("output") != string::npos) || (line.find("register") != string::npos) || (line.find("wire") != string::npos)) {

					if (!parseEdge(line)) { inFile.close(); return false; }//parse edges/connectors here
				}
				else {
					if (!parseNode(line)) { inFile.close(); return false; }//parse logic here
				}
			}
		}
	}
	if (CorrectFormat == false) {
		inFile.close();//close the input file
		return false;
	}

	inFile.close();//close the input file
	return true;
}


bool Netlist::parseEdge(string inputLine) {
	inputLine.erase(remove(inputLine.begin(), inputLine.end(), ','), inputLine.end());	//removes comma's from the string
	istringstream inSS(inputLine);														// Input string stream
	string currentWord = "";
	string type = "";
	string dataWidthString = "";
	string dataWidthString2 = "";
	bool tempSign = false;
	int dataWidthInt = 0;

	inSS >> type;
	inSS >> dataWidthString;

	if (dataWidthString.find("U") != string::npos) { tempSign = true; dataWidthString2 = dataWidthString.substr(4); }
	else { tempSign = false; dataWidthString2 = dataWidthString.substr(3); }
	dataWidthInt = stoi(dataWidthString2);
	//still may need to check for no value int width

	while (inSS >> currentWord) {
		edges.push_back(new Connector(currentWord, type, dataWidthInt, tempSign));	//create new connector and add to vector
	}

	return true;
}



bool Netlist::parseNode(string inputLine) {
	istringstream inSS(inputLine);       // Input string stream
	Connector *tempConnector = NULL;
	Logic *tempLogic = NULL;
	string currentWord;
	string outputEdge;
	string type;
	string variable1 = "";
	string variable2 = "";
	string variable3 = "";
	//	vector <Connector>::iterator It;
	int i = 0;


	inSS >> outputEdge;	//records output of this Node/Logic

	for (i = 0; i < edges.size(); ++i) {//search through 'connectorVector' for name 'outputEdge' and save to *connectorPtr, ie find the edge this logic line is referencing and pass it's address on
		if (edges.at(i)->GetName().find(outputEdge) != string::npos) {
			tempConnector = edges.at(i);
		}
	}

	inSS >> currentWord;															//records the '=' to get rid of it
	inSS >> variable1;																//records the first input variable
	if (tempLogic->GetConnector()->GetType().find("register") != string::npos) {	//check if the data type is a register, this will make the logic a REG since there is no +-/* symbol for reg in logic lines
		type = "REG";
		//make register stuff
	}
	else {
		inSS >> currentWord;								//records the symbol of logic type
		inSS >> variable2;									//records the second input variable
		if (currentWord.find("?") != string::npos) {		//Logic is a MUX 
			type = "MUX";									//deal with MUX here, has 3 input thingys
			inSS >> currentWord;							//records the ':' to get rid of it
			inSS >> variable3;								//records the third input variable
		}
		else if (!currentWord.compare("+")) {
			if (!variable2.compare("1")) { type = "INC"; }	//is an incrementor not an adder
			else { type = "ADD"; }							//is an adder
		}
		else if (!currentWord.compare("-")) {
			if (!variable2.compare("1")) { type = "DEC"; }  //is an decrementor not a subtractor
			else { type = "SUB"; }						    //is an subtractor
		}
		else if (!currentWord.compare("*")) { type = "MUL"; }
		else if (!currentWord.compare("==")) { type = "COMP"; }
		else if (!currentWord.compare(">>")) { type = "SHR"; }
		else if (!currentWord.compare("<<")) { type = "SHL"; }
		else if (!currentWord.compare("/")) { type = "DIV"; }
		else if (!currentWord.compare("%")) { type = "MOD"; }
	}

	tempLogic = new Logic(type, tempConnector, tempConnector->GetSize());	//create the new logic element with its output edge, type and datawidth 
	nodes.push_back(tempLogic);										//create new logic/node and add to vector

	return true;
}

Netlist::Netlist(void) {}
Netlist::~Netlist(void) {}