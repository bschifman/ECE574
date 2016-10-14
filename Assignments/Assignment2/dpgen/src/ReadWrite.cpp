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




bool ReadWrite::parseFile(string filename) {


	bool CorrectFormat = false;
	string log;
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
			if (log.size() > 3) {//if there is enough data in the line, ie not an endline, continue

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


bool ReadWrite::parseEdge(vector<Connector*> connectorVector, string inputLine) {
	inputLine.erase(std::remove(inputLine.begin(), inputLine.end(), ','), inputLine.end());	//removes comma's from the string
	istringstream inSS(inputLine);       // Input string stream
//	Connector *connectorPtr = NULL;	//dont think i need this actually, good ol mutators
	string currentWord;
	string type;
	string dataWidthString;
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
bool ReadWrite::parseNode(vector<Logic*> logicVector, vector<Connector*> connectorVector, string inputLine) {
	istringstream inSS(inputLine);       // Input string stream
	Connector *connectorPtr = NULL;
	string currentWord;
	string outputEdge;
	std::string type;


	inSS >> outputEdge;	//records output of this Node/Logic
	//search through 'connectorVector' for name 'outputEdge' and save to *connectorPtr


	logicVector.push_back(new Logic(type, *connectorPtr, connectorPtr->GetSize()));	//create new logic/node and add to vector
	


	return true;	//haven't checked for many error problems yet

	return true;
}