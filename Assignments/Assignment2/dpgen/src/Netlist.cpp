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

	string line;
	fstream inFile;					    // Input file stream
	istringstream inSS("");
	string edgeType = "";
	bool atNodes = false;
	bool CorrectFormat = true;

	inFile.open(filename.c_str());		//opens file

	if (!inFile.is_open()) {			//check if it does not build
		cout << "Could not open text file " << filename << "." << endl;
		inFile.close();//close the input file
		return false;
	}
	else if (inFile.is_open()) {//if file is opens
		cout << "Reading from text file " << filename.substr(filename.rfind('\\') + 1) << "..." << endl;

		while (!inFile.eof()) {			// then while the file does not end, keep going through the loop
			getline(inFile, line);		// grabs the entire line of the input file
			inSS.clear();
			inSS.str(line);
			edgeType = "";
			inSS >> edgeType;
			if (!edgeType.empty()) {			// checks for empty line
				if ((atNodes == false) && ((edgeType == "wire") || (edgeType == "input") || (edgeType == "output") || (edgeType == "register"))) {

					if (!parseEdge(line)) { cout << "Error: Edge Parsing" << endl; CorrectFormat = false; break; }//parse edges/connectors here
				}
				else {

					if (!parseNode(line)) { cout << "Error: Node Parsing" << endl; CorrectFormat = false; break; }//parse logic here
					atNodes = true;
				}
			}
		}
	}
	if (CorrectFormat == false) {
		inFile.close();//close the input file
		return false;
	}
	this->outputToReg();

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
	bool tempSign = true;
	int dataWidthInt = 0;

	inSS >> type;
	inSS >> dataWidthString;

	if (dataWidthString.find("Int") != string::npos) {		//looks for datawidth idenifier, ie proper file format
		if (dataWidthString.find("U") != string::npos) { tempSign = false; dataWidthString2 = dataWidthString.substr(4); }
		else { tempSign = true; dataWidthString2 = dataWidthString.substr(3); }
		istringstream dataWidthSS(dataWidthString2);
		dataWidthSS >> dataWidthInt;
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
	bool sign = false;


	inSS >> outputEdge;	//records output of this Node/Logic
	tempConnector = this->findEdge(outputEdge);
	sign |= tempConnector->GetSign();
	if (tempConnector == NULL) {
		cerr << "Error: missing output or wire variable callout" << endl;
		return false;
	}

	inSS >> garbage;									//records the '=' to get rid of it
	inSS >> variable1;									//records the first input variable
	inSS >> logicSymbol;								//records the symbol of logic type
	if (!logicSymbol.empty()) {
		inSS >> variable2;								//records the second input variable
		if (!variable2.empty()) {
			inSS >> garbage;							//records the ':' to get rid of it
			if (!garbage.empty()) {
				inSS >> variable3;						//records the third input variable
			}
		}
	}
	if ((!variable1.empty()) && (variable1 != "1")) { sign |= this->findEdge(variable1)->GetSign(); }
	if ((!variable2.empty()) && (variable2 != "1")) { sign |= this->findEdge(variable2)->GetSign(); }
	if ((!variable3.empty()) && (variable3 != "1")) { sign |= this->findEdge(variable3)->GetSign(); }


	if ((tempConnector->GetType() == "register")||(logicSymbol.empty())) {				//check if the data type is a register, this will make the logic a REG since there is no +-/* symbol for reg in logic lines
		type = "REG";
		//make register stuff
	}
	else {
		
		if (logicSymbol.empty()) { cerr << "Error: missing datapath component type(+,-,*,==,>>,<<,/,%,?)" << endl; return false; }		//improper input, report error
		if (variable2.empty()) { cerr << "Error: missing input variable for datapath component " << endl; return false; }		//improper input, report error
		tempChkConnector = this->findEdge(variable2);
		if ((tempChkConnector == NULL) && (variable2 != "1") ) { cerr << "Error: missing input variable for datapath component " << endl; return false; }

		if (logicSymbol == "?") {		//Logic is a MUX 
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

	tempLogic = new Logic(type, tempConnector, tempConnector->GetSize(), sign);	//create the new logic element with its output edge, type and datawidth 
	this->nodes.push_back(tempLogic);										//create new logic/node and add to vector
	tempConnector->SetParent(tempLogic);
	if (tempLogic->GetTypeString() == "COMP") { tempLogic->SetOutType(logicSymbol); }		//if the new node is a comparator, record which type it is

	if (!variable1.empty() && (variable1 != "1")) {
		tempConnector = this->findEdge(variable1);								//these "ifs" add the current node to any edge used in the logic
		if (tempConnector != NULL) { tempConnector->AddChild(tempLogic); tempLogic->AddParent(tempConnector); }
	}
	if (!variable2.empty() && (variable2 != "1")) {
		tempConnector = this->findEdge(variable2);
		if (tempConnector != NULL) { tempConnector->AddChild(tempLogic); tempLogic->AddParent(tempConnector); }
	}
	if (!variable3.empty() && (variable3 != "1")) {
		tempConnector = this->findEdge(variable3);
		if (tempConnector != NULL) { tempConnector->AddChild(tempLogic); tempLogic->AddParent(tempConnector); }
	}


	return true;
}


Connector *Netlist::findEdge(string edgeName) {
	Connector *tempConnector = NULL;
	unsigned int i = 0;
	//vector<Connector>::iterator it = find(this->edges.begin(), this->edges.end(), tempConnector);

	for (i = 0; i < this->edges.size(); ++i) {//search through 'edges' for name 'edgeName' and save to *tempConnector, ie find the edge this string is referencing and pass it's address on
		if (this->edges.at(i)->GetName() == (edgeName) ) {
			tempConnector = this->edges.at(i);
			return tempConnector;
		}
	}

	return tempConnector;
}

void Netlist::outputToReg() {
	Connector *tempConnector = NULL;
	Logic *tempLogic = NULL;
	string tempName = "";
	bool sign = false;
	unsigned int i = 0;

	for (i = 0; i < this->edges.size(); i++) {
		if (this->edges.at(i)->GetType() == "output") {
			if (this->edges.at(i)->GetParent()->GetTypeString() != "REG") {
				tempName = this->edges.at(i)->GetName();
				tempName.append("wire");
				tempConnector = new Connector(this->edges.at(i)->GetName(), this->edges.at(i)->GetType(), this->edges.at(i)->GetSize(), this->edges.at(i)->GetSign());
				this->edges.push_back(tempConnector);
				this->edges.at(i)->SetName(tempName);
				this->edges.at(i)->SetType("wire");
				tempLogic = new Logic("REG", tempConnector, tempConnector->GetSize(), sign);
				this->nodes.push_back(tempLogic);//create the new logic element with its output edge, type and datawidth
				tempConnector->SetParent(tempLogic);
				tempLogic->AddParent(this->edges.at(i));
			}
		}
	}


	return;
}


bool Netlist::outputModule(string outputFilename) {			//write all current data into a verilog module
	ofstream outFS; // Output file stream
	string truncatedFilename = "";
	unsigned int i = 0;

	outFS.open(outputFilename.c_str());	// Open file

	if (!outFS.is_open()) {			//check if it does not build
		cout << "Could not create or open file " << outputFilename << "." << endl;
		outFS.close();//close the output file
		return false;
	}
	else if (outFS.is_open()) {//if file is opens
		cout << "Writing to file " << outputFilename << "..." << endl;
		truncatedFilename = outputFilename.substr(0, outputFilename.find("."));		//removes everything after the . of file name to make module name
		outFS << "`timescale 1ns / 1ps" << endl << endl;
		outFS << "module " << truncatedFilename << "(";

		for (i = 0; i < this->edges.size(); i++) {				//lists all of the imputs into the module prototype
			if ((this->edges.at(i)->GetType() == "input")|| (this->edges.at(i)->GetType() == "output")) {
				outFS << this->edges.at(i)->GetName() << ", ";
			}
		}
		outFS << "clk, rst);" << endl;
		outFS << "\t" << "input clk, rst;" << endl;
		for (i = 0; i < 7; i++) { outFS << this->outputEdgeLine("input", (1 << i)) ; }	//output all input variables
		outFS << endl;
		for (i = 0; i < 7; i++) { outFS << this->outputEdgeLine("output", (1 << i)) ; }	//output all output variables
		outFS << endl;
		for (i = 0; i < 7; i++) { outFS << this->outputEdgeLine("wire", (1 << i)) ; }	//output all wire variables
		outFS << endl;

		for (i = 0; i < this->nodes.size(); i++) { outFS << this->outputNodeLine(i) << endl;	}		//output all nodes/logics
		

		outFS << endl << "endmodule" << endl;
	}


	outFS.close();		//close the output file
	return true;
}


string Netlist::outputEdgeLine(string type, unsigned int datawidth) {	//formats a string of variable callouts for an output file
	string outputLine = "";
	ostringstream outSS;
	unsigned int i = 0;
	bool checked = false;

	checked = false;
	for (i = 0; i < this->edges.size(); i++) {			//lists all of the imputs into the module prototype
		if ((this->edges.at(i)->GetType() == type) && ((unsigned int)this->edges.at(i)->GetSize() == datawidth)) {
			if (checked == false) {
				outSS << "\t" << type << " ";
				if (datawidth > 1) {
					outSS << "[" << (datawidth - 1) << ":0] ";
				}
			}
			else { outSS << ", "; }
			outSS << this->edges.at(i)->GetName();
			checked = true;
		}
	}
	if (checked == true) { outSS << ";" << endl; }

	return outSS.str();
}



string Netlist::outputNodeLine(int nodeNumber) {
	string outputLine = "";
	ostringstream outSS;
	unsigned int i = 0;
	unsigned int j = 0;

	outSS << "\t";
	if (this->nodes.at(nodeNumber)->GetSign() == 1) { outSS << "S"; }	//if module is signed, mark as such
	outSS << this->nodes.at(nodeNumber)->GetTypeString();
	if(this->nodes.at(nodeNumber)->GetTypeString() == "MUX"){ outSS << "2X1"; }	//mux has a special designator, maybe should change name to that throughout code....
	outSS << "\t\t" << "#(.DATAWIDTH("; 
	if (this->nodes.at(nodeNumber)->GetTypeString() == "COMP") { 
		if (this->nodes.at(nodeNumber)->GetParent().at(0)->GetSize() > this->nodes.at(nodeNumber)->GetParent().at(1)->GetSize()) {	//compare the 2 imput edges and use the larger datawidth
			outSS << this->nodes.at(nodeNumber)->GetParent().at(0)->GetSize();
		}
		else{ outSS << this->nodes.at(nodeNumber)->GetParent().at(1)->GetSize(); }
	}
	else { outSS << this->nodes.at(nodeNumber)->GetConnector()->GetSize(); }		//if node is not a comparator, get datawidth from its output edge

	outSS << "))" << "\t\t";
	for (i = 0; i < (unsigned)nodeNumber; i++) { 
		if (this->nodes.at(nodeNumber)->GetTypeString() == this->nodes.at(i)->GetTypeString()) { j++; }	//count how many of this module already exist
	}
	outSS << this->nodes.at(nodeNumber)->GetTypeString() << "_" << (j) << " (";
	if(this->nodes.at(nodeNumber)->GetTypeString() == "REG"){ outSS << this->nodes.at(nodeNumber)->GetParent().at(0)->GetName() << ", clk, rst, " << this->nodes.at(nodeNumber)->GetConnector()->GetName() << ")"; }
	else if (this->nodes.at(nodeNumber)->GetTypeString() == "ADD") { outSS << this->nodes.at(nodeNumber)->GetParent().at(0)->GetName() << ", " << this->nodes.at(nodeNumber)->GetParent().at(1)->GetName() << ", " << this->nodes.at(nodeNumber)->GetConnector()->GetName() << ")"; }
	else if (this->nodes.at(nodeNumber)->GetTypeString() == "SUB") { outSS << this->nodes.at(nodeNumber)->GetParent().at(0)->GetName() << ", " << this->nodes.at(nodeNumber)->GetParent().at(1)->GetName() << ", " << this->nodes.at(nodeNumber)->GetConnector()->GetName() << ")"; }
	else if (this->nodes.at(nodeNumber)->GetTypeString() == "MUL") { outSS << this->nodes.at(nodeNumber)->GetParent().at(0)->GetName() << ", " << this->nodes.at(nodeNumber)->GetParent().at(1)->GetName() << ", " << this->nodes.at(nodeNumber)->GetConnector()->GetName() << ")"; }
	else if (this->nodes.at(nodeNumber)->GetTypeString() == "MUX") { outSS << this->nodes.at(nodeNumber)->GetParent().at(1)->GetName() << ", " << this->nodes.at(nodeNumber)->GetParent().at(2)->GetName() << ", " << this->nodes.at(nodeNumber)->GetParent().at(0)->GetName() << ", " << this->nodes.at(nodeNumber)->GetConnector()->GetName() << ")"; }
	else if (this->nodes.at(nodeNumber)->GetTypeString() == "SHR") { outSS << this->nodes.at(nodeNumber)->GetParent().at(0)->GetName() << ", " << this->nodes.at(nodeNumber)->GetParent().at(1)->GetName() << ", " << this->nodes.at(nodeNumber)->GetConnector()->GetName() << ")"; }
	else if (this->nodes.at(nodeNumber)->GetTypeString() == "SHL") { outSS << this->nodes.at(nodeNumber)->GetParent().at(0)->GetName() << ", " << this->nodes.at(nodeNumber)->GetParent().at(1)->GetName() << ", " << this->nodes.at(nodeNumber)->GetConnector()->GetName() << ")"; }
	else if (this->nodes.at(nodeNumber)->GetTypeString() == "DIV") { outSS << this->nodes.at(nodeNumber)->GetParent().at(0)->GetName() << ", " << this->nodes.at(nodeNumber)->GetParent().at(1)->GetName() << ", " << this->nodes.at(nodeNumber)->GetConnector()->GetName() << ")"; }
	else if (this->nodes.at(nodeNumber)->GetTypeString() == "MOD") { outSS << this->nodes.at(nodeNumber)->GetParent().at(0)->GetName() << ", " << this->nodes.at(nodeNumber)->GetParent().at(1)->GetName() << ", " << this->nodes.at(nodeNumber)->GetConnector()->GetName() << ")"; }
	else if (this->nodes.at(nodeNumber)->GetTypeString() == "INC") { outSS << this->nodes.at(nodeNumber)->GetParent().at(0)->GetName() << ", " << this->nodes.at(nodeNumber)->GetConnector()->GetName() << ")"; }
	else if (this->nodes.at(nodeNumber)->GetTypeString() == "DEC") { outSS << this->nodes.at(nodeNumber)->GetParent().at(0)->GetName() << ", " << this->nodes.at(nodeNumber)->GetConnector()->GetName() << ")"; }
	else if (this->nodes.at(nodeNumber)->GetTypeString() == "COMP") { //depending 
		outSS << ".a(" << this->nodes.at(nodeNumber)->GetParent().at(0)->GetName() << "), .b(" << this->nodes.at(nodeNumber)->GetParent().at(1)->GetName() << "), ";
		if (this->nodes.at(nodeNumber)->GetOutType() == ">") { outSS << ".gt("; }
		if (this->nodes.at(nodeNumber)->GetOutType() == "<") { outSS << ".lt("; }
		if (this->nodes.at(nodeNumber)->GetOutType() == "==") { outSS << ".eq("; }
		outSS <<  this->nodes.at(nodeNumber)->GetConnector()->GetName() << "))"; //this is missing which gt, eq, lt that the child edge should be connected too
	}

	//outSS << "COMP    #(.DATAWIDTH(8))    COMP_0(.a(d), .b(e), .gt(g));";

	/*
	module REG(d, Clk, Rst, q);
	module ADD(a, b, sum);
	module SUB(a, b, diff);
	module MUL(a, b, prod);
	module COMP(a, b, gt, lt, eq);
	module MUX2x1(a, b, sel, d);
	module SHR(a, sh_amt, d);
	module SHL(a, sh_amt, d);
	module DIV(a, b, quot);
	module MOD(a, b, rem);
	module INC(a, d);
	module DEC(a, d);
	*/

	outSS << ";";
	return outSS.str();
}



Netlist::Netlist(void) {}
Netlist::~Netlist(void) {
	unsigned int i = 0;

	for (i = this->edges.size(); i > 0; i--) {
		delete this->edges.at(i - 1);
		this->edges.pop_back();
	}
	for (i = this->nodes.size(); i > 0; i--) {
		delete this->nodes.at(i - 1);
		this->nodes.pop_back();
	}

}