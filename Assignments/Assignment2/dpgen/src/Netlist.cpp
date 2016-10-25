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
	string line;
	fstream inFile;					    // Input file stream
	istringstream inSS("");
	string edgeType = "";
	bool atNodes = false;
	bool CorrectFormat = true;

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
				inSS.clear();
				inSS.str(line);
				inSS >> edgeType;
				if ((atNodes == false) && ((edgeType == "wire") || (edgeType == "input") || (edgeType == "output") || (edgeType == "register"))) {

					if (!parseEdge(line)) { cout << "Error: Edge Parsing" << endl; CorrectFormat = false; break; }//parse edges/connectors here
				}
				else {

					if (!parseNode(line)) { cout << "Error: Node Parsing" << endl; CorrectFormat = false; break; }//parse logic here
					atNodes = true;
				}
			}
		}
		this->outputToReg();
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
	string logicSymbol = "";
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

	while (inSS >> logicSymbol) {
		this->edges.push_back(new Connector(logicSymbol, type, dataWidthInt, tempSign));	//create new connector and add to vector
	}

	return true;
}



bool Netlist::parseNode(string inputLine) {
	istringstream inSS(inputLine);       // Input string stream
	Connector *tempConnector = NULL;
	Connector *tempChkConnector = NULL;
	Logic *tempLogic = NULL;
	string garbage = "";
	string logicSymbol = "";
	string outputEdge = "";
	string type = "";
	string variable1 = "";
	string variable2 = "";
	string variable3 = "";
	int i = 0;


	inSS >> outputEdge;	//records output of this Node/Logic
	tempConnector = this->findEdge(outputEdge);
	if (tempConnector == NULL) {
		cerr << "Error: missing input, output or wire variable callout" << endl;
		return false;
	}

	inSS >> garbage;					   										    //records the '=' to get rid of it
	inSS >> variable1;																//records the first input variable
	inSS >> logicSymbol;								//records the symbol of logic type
	if (!logicSymbol.empty()) {
		inSS >> variable2;									//records the second input variable
		if (!variable2.empty()) {
			inSS >> garbage;							//records the ':' to get rid of it
			if (!garbage.empty()) {
				inSS >> variable3;								//records the third input variable
			}
		}
	}


	if ((tempConnector->GetType() == "register")||(logicSymbol.empty())) {				//check if the data type is a register, this will make the logic a REG since there is no +-/* symbol for reg in logic lines
		type = "REG";
		//make register stuff
	}
	else {
		
		
		if (logicSymbol.empty()) { cerr << "Error: missing datapath component type(+,-,*,==,>>,<<,/,%)" << endl; return false; }		//improper input, report error
		if (variable2.empty()) { cerr << "Error: missing input variable for datapath component " << endl; return false; }		//improper input, report error
		tempChkConnector = this->findEdge(variable2);
		if (tempChkConnector == NULL) { cerr << "Error: missing input variable for datapath component " << endl; return false; }

		if (logicSymbol.find("?") != string::npos) {		//Logic is a MUX 
			type = "MUX";									//deal with MUX here, has 3 input thingys
			if (!(garbage == ":")) { cerr << "Error: missing ':' for compator component " << endl; return false; }		//improper operator, should be a ':' report error
			if (variable3.empty()) { cerr << "Error: missing input variable for datapath component " << endl; return false; }		//improper operator, report error
			tempChkConnector = this->findEdge(variable3);
			if (tempChkConnector == NULL) { cerr << "Error: missing input variable for datapath component " << endl; return false; }
		}
		else if (!logicSymbol.compare("+")) {
			if (!variable2.compare("1")) { type = "INC"; }	//is an incrementor not an adder
			else { type = "ADD"; }							//is an adder
		}
		else if (!logicSymbol.compare("-")) {
			if (!variable2.compare("1")) { type = "DEC"; }  //is an decrementor not a subtractor
			else { type = "SUB"; }						    //is an subtractor
		}
		else if (!logicSymbol.compare("*")) { type = "MUL"; }
		else if (!logicSymbol.compare(">>")) { type = "SHR"; }
		else if (!logicSymbol.compare("<<")) { type = "SHL"; }
		else if ((!logicSymbol.compare("==")) || (!logicSymbol.compare("<")) || (!logicSymbol.compare(">"))) { type = "COMP"; }
		else if (!logicSymbol.compare("/")) { type = "DIV"; }
		else if (!logicSymbol.compare("%")) { type = "MOD"; }
		else { cerr << "Error: incorrect operator " << endl; return false; }
	}

	//change size maybe depending on inputs?  I don't think so I would imagine the output size would determine but need to examine if so.

	tempLogic = new Logic(type, tempConnector, tempConnector->GetSize());	//create the new logic element with its output edge, type and datawidth 
	this->nodes.push_back(tempLogic);										//create new logic/node and add to vector
	tempConnector->SetParent(tempLogic);

	if (!variable1.empty()) {
		tempConnector = this->findEdge(variable1);								//these "ifs" add the current node to any edge used in the logic
		if (tempConnector != NULL) { tempConnector->AddChild(tempLogic); }
	}
	if (!variable2.empty()) {
		tempConnector = this->findEdge(variable2);
		if (tempConnector != NULL) { tempConnector->AddChild(tempLogic); }
	}
	if (!variable3.empty()) {
		tempConnector = this->findEdge(variable3);
		if (tempConnector != NULL) { tempConnector->AddChild(tempLogic); }
	}


	return true;
}


Connector *Netlist::findEdge(string edgeName) {
	Connector *tempConnector = NULL;
	int i = 0;
	//vector<Connector>::iterator it = find(this->edges.begin(), this->edges.end(), tempConnector);

	for (i = 0; i < this->edges.size(); ++i) {//search through 'edges' for name 'edgeName' and save to *tempConnector, ie find the edge this string is referencing and pass it's address on
		if (this->edges.at(i)->GetName().find(edgeName) != string::npos) {
			tempConnector = this->edges.at(i);
			return tempConnector;
		}
	}

	return tempConnector;
}

void Netlist::outputToReg() {
	Connector *tempConnector = NULL;
	Logic *tempLogic = NULL;
	int i = 0;
	string tempName = "";
	for (i = 0; i < this->edges.size(); i++) {
		if (this->edges.at(i)->GetType() == "output") {
			if (this->edges.at(i)->GetParent()->GetTypeString() != "REG") {
				tempName = this->edges.at(i)->GetName();
				tempName.append("wire");
				tempConnector = new Connector(this->edges.at(i)->GetName(), this->edges.at(i)->GetType(), this->edges.at(i)->GetSize(), this->edges.at(i)->GetSign());
				this->edges.push_back(tempConnector);
				this->edges.at(i)->SetName(tempName);
				tempLogic = new Logic("REG", tempConnector, tempConnector->GetSize());
				this->nodes.push_back(tempLogic);//create the new logic element with its output edge, type and datawidth
				tempConnector->SetParent(tempLogic);
			}
		}
	}


	return;
}

Netlist::Netlist(void) {}
Netlist::~Netlist(void) {
	int  i = 0;
	for (i = this->edges.size(); this->edges.size() > 0; i--) {
//		this->edges.at(i)->~Connector();
		this->edges.pop_back();
	}
	for (i = this->nodes.size(); this->nodes.size() > 0;i--) {
//		this->nodes.at(i)->~Logic();
		this->nodes.pop_back();
	}
}