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

	if (dataWidthString.find("Int") != string::npos) {
		if (dataWidthString.find("U") != string::npos) { tempSign = true; dataWidthString2 = dataWidthString.substr(4); }
		else { tempSign = false; dataWidthString2 = dataWidthString.substr(3); }
		dataWidthInt = stoi(dataWidthString2);
		//still may need to check for no value int width
	}
	else { cerr << "Error: missing proper datawidth callout " << endl; return false; }		//if no int width described, return an error

	while (inSS >> currentWord) {
		this->edges.push_back(new Connector(currentWord, type, dataWidthInt, tempSign));	//create new connector and add to vector
	}

	return true;
}



bool Netlist::parseNode(string inputLine) {
	istringstream inSS(inputLine);       // Input string stream
	Connector *tempConnector = NULL;
	Logic *tempLogic = NULL;
	string currentWord = "";
	string outputEdge = "";
	string type = "";
	string variable1 = "";
	string variable2 = "";
	string variable3 = "";
	int i = 0;


	inSS >> outputEdge;	//records output of this Node/Logic
	tempConnector = this->findEdge(outputEdge);

	inSS >> currentWord;															//records the '=' to get rid of it
	inSS >> variable1;																//records the first input variable
	if (tempConnector->GetType().find("register") != string::npos) {				//check if the data type is a register, this will make the logic a REG since there is no +-/* symbol for reg in logic lines
		type = "REG";
		//make register stuff
	}
	else {
		inSS >> currentWord;								//records the symbol of logic type
		inSS >> variable2;									//records the second input variable
		if (currentWord.empty()) { cerr << "Error: missing datapath component type(+,-,*,==,>>,<<,/,%)" << endl; return false; }		//improper input, report error
		if (variable2.empty()) { cerr << "Error: missing input variable for datapath component " << endl; return false; }		//improper input, report error

		if (currentWord.find("?") != string::npos) {		//Logic is a MUX 
			type = "MUX";									//deal with MUX here, has 3 input thingys
			inSS >> currentWord;							//records the ':' to get rid of it
			if (!currentWord.compare(":")) { cerr << "Error: missing ':' for compator component " << endl; return false; }		//improper operator, should be a ':' report error
			inSS >> variable3;								//records the third input variable
			if (variable3.empty()) { cerr << "Error: missing input variable for datapath component " << endl; return false; }		//improper operator, report error
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

	//change size maybe depending on inputs?  I don't think so I would imagine the output size would determine but need to examine if so.

	tempLogic = new Logic(type, tempConnector, tempConnector->GetSize());	//create the new logic element with its output edge, type and datawidth 
	this->nodes.push_back(tempLogic);										//create new logic/node and add to vector
	tempConnector->SetParent(tempLogic);

	tempConnector = this->findEdge(variable1);								//these "ifs" add the current node to any edge used in the logic
	if (tempConnector != NULL) {	tempConnector->AddChild(tempLogic);	}
	tempConnector = this->findEdge(variable2);
	if (tempConnector != NULL) { tempConnector->AddChild(tempLogic); }
	tempConnector = this->findEdge(variable3);
	if (tempConnector != NULL) { tempConnector->AddChild(tempLogic); }


	return true;
}


Connector * Netlist::findEdge(string edgeName) {
	Connector *tempConnector = NULL;
	int i = 0;

	for (i = 0; i < this->edges.size(); ++i) {//search through 'edges' for name 'edgeName' and save to *tempConnector, ie find the edge this string is referencing and pass it's address on
		if (this->edges.at(i)->GetName().find(edgeName) != string::npos) {
			tempConnector = this->edges.at(i);
			break;
		}
	}

	return tempConnector;
}

Netlist::Netlist(void) {}
Netlist::~Netlist(void) {

	while (this->edges.size() > 0) {
		this->edges.pop_back();
	}
	while (this->nodes.size() > 0) {
		this->nodes.pop_back();
	}
}