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
//		cout << "Reading from text file " << filename.substr(filename.rfind('\\') + 1) << "..." << endl;

		while (!inFile.eof()) {			// then while the file does not end, keep going through the loop
			getline(inFile, line);		// grabs the entire line of the input file
			inSS.clear();				//explicitly clear the stream
			inSS.str(line);				//put the file line into  the string stream to eliminate any white spaces if there are any(else ECE3 will complain)
			edgeType = "";				
			inSS >> edgeType;			//put the first variable of the unused whitespace purged line into edgetype
			if (!edgeType.empty()) {			// checks for empty line
				line = line.substr(0, line.find("//"));		//remove any comments from the line
				if ((atNodes == false) && ((edgeType == "wire") || (edgeType == "input") || (edgeType == "output") || (edgeType == "register"))) {

					if (!parseEdge(line)) { cout << "Error: Edge Parsing" << endl; CorrectFormat = false; break; }//parse edges/connectors here
				}
				else {

					if (!parseNode(line)) { cout << "Error: Node Parsing" << endl; CorrectFormat = false; break; }//parse logic here
					atNodes = true;			//the input file is now at nodes, no more variables to be declared past this point
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
	bool tempSign = true;		//sign true is a 'signed' variable
	int dataWidthInt = 0;

	inSS >> type;
	inSS >> dataWidthString;

	if (dataWidthString.find("Int") != string::npos) {				//looks for datawidth idenifier, ie proper file format
		if (dataWidthString.find("U") != string::npos) { tempSign = false; dataWidthString2 = dataWidthString.substr(4); }		//check if declared as 'unsigned' variables
		else { tempSign = true; dataWidthString2 = dataWidthString.substr(3); }
		istringstream dataWidthSS(dataWidthString2);
		dataWidthSS >> dataWidthInt;
		//still may need to check for no value int width
	}
	else { cerr << "Error: missing proper datawidth callout " << endl; return false; }		//if no int width described, return an error

	while (inSS >> logicSymbol) {			//cycle through all the variables of this data type in this line
		this->edges.push_back(new Connector(logicSymbol, type, dataWidthInt, tempSign));	//create new connector and add to netlist vector
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


	inSS >> outputEdge;									//records output edge (variable name)of this Node/Logic
	tempConnector = this->findEdge(outputEdge);			//identify which vector edge is associated with the variable
	if (tempConnector == NULL) {
		cerr << "Error: missing output or wire variable callout" << endl;
		return false;
	}

	sign |= tempConnector->GetSign();
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

	if ((tempConnector->GetType() == "register") || (logicSymbol.empty())) {				//check if the data type is a register, this will make the logic a REG since there is no +-/* symbol for reg in logic lines
		type = "REG";
		//make register stuff
	}
	else {
		
		if (logicSymbol.empty()) { cerr << "Error: missing datapath component type(+,-,*,==,>>,<<,/,%,?)" << endl; return false; }		//improper input, report error
		if (variable2.empty()) { cerr << "Error: missing input variable for datapath component " << endl; return false; }				//improper input, report error
		tempChkConnector = this->findEdge(variable2);
		if ((tempChkConnector == NULL) && (variable2 != "1") ) { cerr << "Error: missing input variable for datapath component " << endl; return false; }

		if (logicSymbol == "?") {		//Logic is a MUX2x1 
			type = "MUX2x1";									//deal with MUX here, has 3 input thingys
			if (!(garbage == ":")) { cerr << "Error: missing ':' for compator component " << endl; return false; }					//improper operator, should be a ':' report error
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
		else { cerr << "Error: incorrect operator " << endl; return false; }							//someone put gibberish into the file, tis a problem
	}
	//change size maybe depending on inputs?  I don't think so I would imagine the output size would determine but need to examine if so.
	
	if ((!variable1.empty()) && (variable1 != "1")) { sign |= this->findEdge(variable1)->GetSign(); }	//find the edges associated with the each variable and check if they are 'signed'
	if ((!variable2.empty()) && (variable2 != "1")) { sign |= this->findEdge(variable2)->GetSign(); }
	if ((!variable3.empty()) && (variable3 != "1")) { sign |= this->findEdge(variable3)->GetSign(); }

	if (type != "COMP") { tempLogic = new Logic(type, tempConnector, tempConnector->GetSize(), sign); }	//create the new logic element with its output edge, type and datawidth 
	else if (this->findEdge(variable1)->GetSize() > this->findEdge(variable2)->GetSize()) {				//compare the 2 input edges and use the larger datawidth
		tempLogic = new Logic(type, tempConnector, this->findEdge(variable1)->GetSize(), sign);			//if vector at 0 is bigger, use it
	}
	else { tempLogic = new Logic(type, tempConnector, this->findEdge(variable2)->GetSize(), sign); }	//if vector at 1 is bigger, use it

	this->nodes.push_back(tempLogic);																	//create new logic/node and add to vector
	tempConnector->SetParent(tempLogic);
	if (tempLogic->GetTypeString() == "COMP") { tempLogic->SetOutType(logicSymbol); }					//if the new node is a comparator, record which type it is

	if (!variable1.empty() && (variable1 != "1")) {
		tempConnector = this->findEdge(variable1);														//these "ifs" add the current node to any edge used in the logic
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


Connector *Netlist::findEdge(string edgeName) {	//finds an edge object in the vector based on a variable name
	Connector *tempConnector = NULL;
	unsigned int i = 0;

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
	unsigned int i = 0;

	for (i = 0; i < this->edges.size(); i++) {									//cycle through all of the edges 
		if (this->edges.at(i)->GetType() == "output") {							//if one of the edges is an output check to see if it is associated with a REG
			if (this->edges.at(i)->GetParent()->GetTypeString() != "REG") {		//if it is not associated with a REG, add a register in before the output edge
				tempName = this->edges.at(i)->GetName();
				tempName.append("wire");										//using the orginal outputs name, add 'wire' to the end to distinguish them(ie 'a' to 'awire')
				tempConnector = new Connector(this->edges.at(i)->GetName(), this->edges.at(i)->GetType(), this->edges.at(i)->GetSize(), this->edges.at(i)->GetSign());
				this->edges.push_back(tempConnector);
				this->edges.at(i)->SetName(tempName);
				this->edges.at(i)->SetType("wire");
				tempLogic = new Logic("REG", tempConnector, tempConnector->GetSize(), this->edges.at(i)->GetSign());
				this->nodes.push_back(tempLogic);								//create the new logic element with its output edge, type and datawidth
				tempConnector->SetParent(tempLogic);
				tempLogic->AddParent(this->edges.at(i));
			}
		}
	}


	return;
}


bool Netlist::outputModule(string outputFilename) {										//write all current data into a verilog module
	ofstream outFS;																		// Output file stream
	string truncatedFilename = "";
	unsigned int i = 0;

	outFS.open(outputFilename.c_str());													// Open file

	if (!outFS.is_open()) {																//check if it does not build
		cout << "Could not create or open file " << outputFilename << "." << endl;
		outFS.close();																	//close the output file
		return false;
	}
	else if (outFS.is_open()) {//if file is opens
//		cout << "Writing to file " << outputFilename << "..." << endl;					//Ben hates helpful comments, removed at his request, we'll be book burning next.
		truncatedFilename = outputFilename.substr(0, outputFilename.find("."));			//removes everything after the . of file name to make module name
		outFS << "`timescale 1ns / 1ps" << endl << endl;								//add time scale to the top of the file
		outFS << "module " << truncatedFilename << "(";

		for (i = 0; i < this->edges.size(); i++) {										//lists all of the inputs and outputs names into the module prototype
			if ((this->edges.at(i)->GetType() == "input") || (this->edges.at(i)->GetType() == "output")) {
				outFS << this->edges.at(i)->GetName() << ", ";
			}
		}
		outFS << "clk, rst);" << endl;
		outFS << "\t" << "input clk, rst;" << endl;
		for (i = 0; i < 7; i++) { outFS << this->outputEdgeLine("input", (1 << i)) ; }					//output all input variables
		outFS << endl;
		for (i = 0; i < 7; i++) { outFS << this->outputEdgeLine("output", (1 << i)) ; }					//output all output variables
		outFS << endl;
		for (i = 0; i < 7; i++) { outFS << this->outputEdgeLine("wire", (1 << i)) ; }					//output all wire variables
		for (i = 0; i < 7; i++) { outFS << this->outputEdgeLine("register", (1 << i)); }				//output all register variables as wire variables
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
	string outType = "";

	if (type == "register") {											//converts all register variables into wire variables in verilog(not sure if we should....works fine since we use REG...)
		outType = "wire";
	}
	else {
		outType = type;													//if it is not a register, use its default type
	}

	checked = false;
	for (i = 0; i < this->edges.size(); i++) {							//lists all of the inputs into the module prototype
		if ((this->edges.at(i)->GetType() == type) && ((unsigned int)this->edges.at(i)->GetSize() == datawidth)) {
			if (checked == false) {
				outSS << "\t" << outType << " ";
				if (datawidth > 1) {
					outSS << "[" << (datawidth - 1) << ":0] ";			//declare the datawidth array
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
	Connector *tempParent0 = this->nodes.at(nodeNumber)->GetParents().at(0);		//c = a + b this is the "a" element
	Connector *tempConnector = this->nodes.at(nodeNumber)->GetConnector();			//c = a + b this is the "c" element
	Connector *tempParent1 = NULL;
	Connector *tempParent2 = NULL;
	string outputLine = "";
	string tempString = this->nodes.at(nodeNumber)->GetTypeString();
		
	if ((tempString != "REG") && (tempString != "DEC") && (tempString != "INC")) {	//if it's not a any of these modules then it will have a "b" element of c = a + b
		tempParent1 = this->nodes.at(nodeNumber)->GetParents().at(1); 
	}
	if (tempString == "MUX2x1") {													//if it is a MUX then there will be another element
		tempParent2 = this->nodes.at(nodeNumber)->GetParents().at(2); 
	}
	

	ostringstream outSS;
	unsigned int i = 0;
	unsigned int j = 0;

	outSS << "\t";
	if ((tempString == "COMP") || (tempString == "DIV") || (tempString == "MOD") || (tempString == "SHR")) {
		if (this->nodes.at(nodeNumber)->GetSign() == 1) { outSS << "S"; }			//if module is signed, mark as such
	}
	outSS << tempString;
	outSS << "\t\t" << "#(.DATAWIDTH("; 
	if (tempString == "COMP") { 
		if (this->nodes.at(nodeNumber)->GetParents().at(0)->GetSize() > this->nodes.at(nodeNumber)->GetParents().at(1)->GetSize()) {	//compare the 2 imput edges and use the larger datawidth
			outSS << this->nodes.at(nodeNumber)->GetParents().at(0)->GetSize();		//if vector at 0 is bigger, use it
		}
		else{ outSS << this->nodes.at(nodeNumber)->GetParents().at(1)->GetSize(); }	//if vector at 1 is bigger, use it
	}
	else { outSS << this->nodes.at(nodeNumber)->GetConnector()->GetSize(); }		//if node is not a comparator, get datawidth from its output edge

	outSS << "))" << "\t\t";
	for (i = 0; i < (unsigned)nodeNumber; i++) { 
		if (tempString == this->nodes.at(i)->GetTypeString()) { j++; }	//count how many of this module already exist
	}
	outSS << tempString << "_" << (j) << " (";
	ostringstream nameSS;
	nameSS << "_" << (j);
	this->nodes.at(i)->SetName(nameSS.str());

	if(tempString == "REG"){											//if the type is a REG then it will only have a single input and output
		
		outSS << CreateInputName(this->nodes.at(nodeNumber), tempParent0);
		outSS << ", clk, rst, ";
		outSS << tempConnector->GetName() << ")";
	}
	else if ((tempString == "ADD") || (tempString == "SUB") || (tempString == "MUL") || (tempString == "DIV") || (tempString == "MOD")) {	//if it is one of these datatypes then it is the standard format a = b + c
		outSS << CreateInputName(this->nodes.at(nodeNumber), tempParent0) << ", " << CreateInputName(this->nodes.at(nodeNumber), tempParent1) << ", " << tempConnector->GetName() << ")";
	}

	else if ((tempString == "SHR") || (tempString == "SHL")){		//for the shifters we need the second element aka the shift ammount to only be unsigned padded
	outSS << CreateInputName(this->nodes.at(nodeNumber), tempParent0) << ", " << CreateShiftName(this->nodes.at(nodeNumber), tempParent1) << ", " << tempConnector->GetName() << ")";
	}

	else if (tempString == "MUX2x1") { outSS << CreateInputName(this->nodes.at(nodeNumber), tempParent1) << ", " << CreateInputName(this->nodes.at(nodeNumber), tempParent2) << ", " << tempParent0->GetName() << ", " << tempConnector->GetName() << ")"; }
	else if ((tempString == "INC") || (tempString == "DEC")) { outSS << CreateInputName(this->nodes.at(nodeNumber), tempParent0) << ", " << tempConnector->GetName() << ")"; }
	else if (tempString == "COMP") {	//depending on logic type (<,>,==) it will label output to proper channel
		outSS << ".a(" << CreateCOMPInputName(tempParent0, tempParent1) << "), .b(" << CreateCOMPInputName(tempParent1, tempParent0) << "), ";	//Comps are stupid, they have their own function for creating names
		if (this->nodes.at(nodeNumber)->GetOutType() == ">") { outSS << ".gt("; }
		if (this->nodes.at(nodeNumber)->GetOutType() == "<") { outSS << ".lt("; }
		if (this->nodes.at(nodeNumber)->GetOutType() == "==") { outSS << ".eq("; }
		outSS <<  tempConnector->GetName() << "))"; 
	}

	outSS << ";";
	return outSS.str();
}

string Netlist::CreateInputName(Logic* CurrentNode, Connector * tempParent) {
	ostringstream outSS;											// output string stream
	Connector *tempConnector = CurrentNode->GetConnector();

	if (tempParent->GetSize() != tempConnector->GetSize()) {
		if (tempParent->GetSize() > tempConnector->GetSize()) {		//truncate if the Parents size is greater than that of the output of the module
			outSS << tempParent->GetName() << "[" << tempConnector->GetSize() - 1 << ":" << "0" << "]";
		}
		else {														//Output is larger than the input depending on sign, pad with numbers
			if (tempParent->GetSign()) {							//if it is signed
				outSS << "{{" << tempConnector->GetSize() - tempParent->GetSize() << "{" << tempParent->GetName() << "[" << tempParent->GetSize() - 1 << "]}}, ";
				outSS << tempParent->GetName() << "[" << tempParent->GetSize() - 1 << ":" << "0" << "]}";
			}
			else {													//for unsigned connector
				outSS << "{{" << tempConnector->GetSize() - tempParent->GetSize() << "{" << 0 << "}},";
				outSS << tempParent->GetName() << "[" << tempParent->GetSize() - 1 << ":" << "0" << "]}";
			}
		}
	}
	else { outSS << tempParent->GetName(); }

	return outSS.str();
}

string Netlist::CreateShiftName(Logic* CurrentNode, Connector * tempParent) {		//this is only being called on the shift ammount
	ostringstream outSS;											// output string stream
	Connector *tempConnector = CurrentNode->GetConnector();

	if (tempParent->GetSize() != tempConnector->GetSize()) {
		if (tempParent->GetSize() > tempConnector->GetSize()) {		//truncate if the inputs size is greater than that of the output of the module
			if (tempParent->GetSize() > 1) {						//if the size of the shift is greater than 1 than make it fancy			
				outSS << tempParent->GetName() << "[" << tempConnector->GetSize() - 1 << ":" << "0" << "]";
			}
			else {
				outSS << tempParent->GetName();
			}
		}
		else {														//Output is larger than the input depending on sign, pad with zeros
			outSS << "{{" << tempConnector->GetSize() - tempParent->GetSize() << "{" << 0 << "}},";
			if (tempParent->GetSize() > 1) {
				outSS << tempParent->GetName() << "[" << tempParent->GetSize() - 1 << ":" << "0" << "]";
			}
			else {
				outSS << tempParent->GetName();
			}
			outSS << "}";
		}
	}
	else { outSS << tempParent->GetName(); }

	return outSS.str();
}

string Netlist::CreateCOMPInputName(Connector * tempParent0, Connector * tempParent1) {
	ostringstream outSS;       // output string stream

	if ((tempParent0->GetSize() < tempParent1->GetSize())) {		//choose the greater of the two inputs to determine the size of the module and the size of the inputs
		if (tempParent0->GetSign()) {					//if it is signed
		outSS << "{{" << tempParent1->GetSize() - tempParent0->GetSize() << "{" << tempParent0->GetName() << "[" << tempParent0->GetSize() - 1 << "]}}, ";
		outSS << tempParent0->GetName() << "[" << tempParent0->GetSize() - 1 << ":" << "0" << "]}";
		}
		else {											//if it is unsigned
			outSS << "{{" << tempParent1->GetSize() - tempParent0->GetSize() << "{" << 0 << "}}, ";
			outSS << tempParent0->GetName() << "[" << tempParent0->GetSize() - 1 << ":" << "0" << "]}";
		}
		
	}

	else { outSS << tempParent0->GetName(); }

	return outSS.str();
}

void Netlist::findCriticalPath() {
	vector<Logic*> tempList;
	unsigned int i = 0;
	unsigned int j = 0;
	float maxDelay = 0;
	float tempDelay = 0;
	float totalDelay = 0;
	bool check = false;

	for (i = 0; i < this->nodes.size(); i++) {											//loop through all nodes
		maxDelay = 0;
		tempDelay = 0;

		if (this->nodes.at(i)->GetTypeString() != "REG") {
			for (j = 0; j < this->nodes.at(i)->GetParents().size(); j++) {				//loop through all Parent Edges of each individual node
				if ((this->nodes.at(i)->GetParents().at(j)->GetType() != "input") && (this->nodes.at(i)->GetParents().at(j)->GetDelay() == 0)) { //check to see if components are created before there input edges have delays aka netlist is out of order
					tempList.push_back(this->nodes.at(i));
					check = true;
					break;
				}
				else {
					tempDelay = this->nodes.at(i)->GetParents().at(j)->GetDelay();		//assign a tempDelay from each Parent Edge
					if (maxDelay < tempDelay) { maxDelay = tempDelay; }					//choose the longest delay from all of the Parent Edges
				}
			}
		}
		if (!check) {
			totalDelay = maxDelay + this->nodes.at(i)->GetDelay();					//sum the inherent delay of the logic with the delay of the parent edge 
			this->nodes.at(i)->SetDelay(totalDelay);								//set delay of node
			this->nodes.at(i)->GetConnector()->SetDelay(totalDelay);				//set delay of output of node
			if (this->GetCriticalPath() < totalDelay) { this->SetCriticalPath(totalDelay); }
		}
		check = false;
	}

	findCriticalPathTemp(tempList);													//this is kinda crap, we probably can implement it in this function instead of creating a new one. Pressed for time.
	cout << "Critical Path : " << this->GetCriticalPath() << " ns" << endl;
}

void Netlist::findCriticalPathTemp(vector<Logic*> tempList) {						//this is crap, refer up to function call
	unsigned int i = 0;
	unsigned int j = 0;
	float maxDelay = 0;
	float tempDelay = 0;
	float totalDelay = 0;
	bool check = false;

	while (!tempList.empty()) {

		for (i = 0; i < tempList.size(); i++) {										//loop through all nodes in tempList
			maxDelay = 0;
			tempDelay = 0;
			check = false;

			for (j = 0; j < tempList.at(i)->GetParents().size(); j++) {				//loop through all Parent Edges of each individual node
				if ((tempList.at(i)->GetParents().at(j)->GetDelay() == 0) && (tempList.at(i)->GetParents().at(j)->GetType() != "input")) {
					check = true;
					break;					
				}
				else {
					tempDelay = tempList.at(i)->GetParents().at(j)->GetDelay();		//assign a tempDelay from each Parent Edge
					if (maxDelay < tempDelay) { maxDelay = tempDelay; }				//choose the longest delay from all of the Parent Edges
				}
			}

			if (!check) {
				totalDelay = maxDelay + tempList.at(i)->GetDelay();					//sum the inherent delay of the logic with the delay of the parent edge 
				tempList.at(i)->SetDelay(totalDelay);								//set delay of node
				tempList.at(i)->GetConnector()->SetDelay(totalDelay);				//set delay of output of node
				if (this->GetCriticalPath() < totalDelay) { this->SetCriticalPath(totalDelay); }
				tempList.erase(tempList.begin()+i);
				i--;																//decrement node element beacause the list has done gotten shrunked
			}

		}

	}

}

Netlist::Netlist(void) { this->criticalPath = 0; }									//DeSTRUCTOR!!!!!!!!~!~!
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