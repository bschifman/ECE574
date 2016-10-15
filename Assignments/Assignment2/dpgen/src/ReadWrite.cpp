/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: ReadWrite.cpp
*************************************/

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include "ReadWrite.h"
#include "Logic.h"
#include "Connector.h"
using namespace std;




bool ReadWrite::parseFile(std::string filename) {


	bool CorrectFormat = false;
	std::string log;
	Logic *logicPtr = NULL;
	vector<Logic*> logicVector;	//list of logic items
	vector<Connector*> connectorVector;	//list of logic items
	fstream inFile;     // Input file stream

	inFile.open(filename.c_str());//opens file

	if (!inFile.is_open()) { //check if it does not build
		cout << "Could not open text file." << endl;
		inFile.close();//close the input file
		return false;
	}

	else if (inFile.is_open()) {//if file is opens

		while (!inFile.eof()) {// then while the file does not end, keep going through the loop
			getline(inFile, log);// grabs the entire line of the input file
			if (log.size() >= 3) {//if there is enough data in the line, ie not an endline, continue to parse

				if ((log.find("input") != string::npos)|| (log.find("output") != string::npos)|| (log.find("register") != string::npos)|| (log.find("wire") != string::npos)) {
					if (!parseEdge(connectorVector, log)) { inFile.close(); return false; }//parse edges/connectors here
				}
				else {
					if (!parseNode(logicVector, connectorVector, log)) { inFile.close(); return false; }//parse logic here
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


bool ReadWrite::parseEdge(vector<Connector*> connectorVector, std::string inputLine) {
	inputLine.erase(std::remove(inputLine.begin(), inputLine.end(), ','), inputLine.end());	//removes comma's from the string
	istringstream inSS(inputLine);       // Input string stream
//	Connector *connectorPtr = NULL;	//dont think i need this actually, good ol overloading constructors
	std::string currentWord;
	std::string type;
	std::string dataWidthString;
	int dataWidthInt = 0;


	inSS >> type;	//records type of edge
	inSS >> dataWidthString;		//records the datawidth
	if (dataWidthString.find("32") != string::npos) {	dataWidthInt = 32;	}	//sets the datawidths
	else if (dataWidthString.find("16") != string::npos) { dataWidthInt = 16; }
	else if (dataWidthString.find("8") != string::npos) { dataWidthInt = 8; }
	else if (dataWidthString.find("4") != string::npos) { dataWidthInt = 4; }
	else if (dataWidthString.find("2") != string::npos) { dataWidthInt = 2; }
	else if (dataWidthString.find("1") != string::npos) { dataWidthInt = 1; }
	else { return false; }	//second word in line wasn't a data width, error, tis screwed

	while (inSS >> currentWord) {
		connectorVector.push_back(new Connector(currentWord, type, dataWidthInt));	//create new connector and add to vector
	}


	return true;	//haven't checked for many error problems yet
}



bool ReadWrite::parseNode(vector<Logic*> logicVector, vector<Connector*> connectorVector, std::string inputLine) {
	istringstream inSS(inputLine);       // Input string stream
	Connector *connectorPtr = NULL;
	Logic *logicPtr = NULL;
	std::string currentWord;
	std::string outputEdge;
	std::string type;
	std::string variable1 = NULL;
	std::string variable2 = NULL;
	std::string variable3 = NULL;
//	vector <Connector>::iterator It;
	int i = 0;


	inSS >> outputEdge;	//records output of this Node/Logic
	
	for (i = 0; i < connectorVector.size();++i) {//search through 'connectorVector' for name 'outputEdge' and save to *connectorPtr, ie find the edge this logic line is referencing and pass it's address on
		if (connectorVector.at(i)->GetName().find(outputEdge) != string::npos) {
			connectorPtr = connectorVector.at(i);
		}
	}
	
	inSS >> currentWord;	//records the '=' to get rid of it
	inSS >> variable1;	//records the first input variable
	if (logicPtr->GetConnector()->GetType().find("REG") != string::npos) {	//check if the data type is a register, this will make the logic a REG since there is no +-/* symbol for reg in logic lines

	}
	else {
		inSS >> currentWord;	//records the symbol of logic type
		inSS >> variable2;	//records the second input variable
		if (currentWord.find("?") != string::npos) {	//Logic is a MUX 
			type = "MUX";//deal with MUX here, has 3 input thingys
		}
		else if (!currentWord.compare("+")) { 
			if (!variable2.compare("1")) {
				type = "INC";//is an incrementor not an adder
			}
			else {
				type = "ADD";//is an adder
			}
		}
		else if (!currentWord.compare("-")) { 
			if (!variable2.compare("1")) {
				type = "DEC";//is an decrementor not an adder
			}
			else {
				type = "SUB";//is an subtractor
			}
		}
		else if (!currentWord.compare("*")) { type = "MUL"; }
		else if (!currentWord.compare("==")) { type = "COMP"; }
		else if (!currentWord.compare(">>")) { type = "SHR"; }
		else if (!currentWord.compare("<<")) { type = "SHL"; }
		else if (!currentWord.compare("/")) { type = "DIV"; }
		else if (!currentWord.compare("%")) { type = "MOD"; }
	}
	
	logicPtr = new Logic(type, connectorPtr, connectorPtr->GetSize());	//create the new logic element with its output edge, type and datawidth 
	logicVector.push_back(logicPtr);	//create new logic/node and add to vector

	return true;	//haven't checked for many error problems yet
}