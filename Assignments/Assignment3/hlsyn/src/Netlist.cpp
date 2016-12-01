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

bool Netlist::parseFile(string filename, string latency) {

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
				if ((atNodes == false) && ((edgeType == "wire") || (edgeType == "input") || (edgeType == "output") || (edgeType == "register") || (edgeType == "variable"))) {

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
	//this->outputToReg();			//I dont think we need output to register for assignment 3
	this->SetLatency(latency);
	this->CalculateFDS();
	this->CalculateCaseStates();


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
	ostringstream tempName;					//output string stream
	Connector *tempConnector = NULL;
	Connector *tempConnectorUp = NULL;
	Connector *tempChkConnector = NULL;
	Logic *tempLogic = NULL;
	string garbage = "";
	string logicSymbol = "";
	string outputEdge = "";
	string type = "";
	string variable1 = "";
	string variable2 = "";
	string variable3 = "";
//	string tempName = "";
	bool sign = false;
	int i = 0;


	inSS >> outputEdge;										//records output edge (variable name)of this Node/Logic
	if (!outputEdge.compare("if")) {
		inSS >> garbage;									//records begining "("
		inSS >> variable1;									//records the conditional variable for the if
		inSS >> garbage;									//records ending ")"
		this->SetIfElseDepth(this->GetIfElseDepth() +int(1));					//increment the depth of the circuit(teired if/for statements)
		this->SetIfForIncrementer((this->GetIfForIncrementer() + int(1)));		//count how many if/for statements there have been
		this->ifForLevelOneOrZero.resize(this->GetIfElseDepth());				//increase the true/false depth vector(if=true, else=false)
		this->SetIfForLevelOneOrZero((this->GetIfElseDepth() - int(1)), true);				//set the current depth vector to true for the if statement

		tempConnectorUp = this->findEdge(variable1);							//identify which vector edge is associated with the variable
		sign |= tempConnectorUp->GetSign();

		tempName << "if ";
		tempName << this->GetIfForIncrementer();								//using the orginal outputs name, add 'wire' to the end to distinguish them(ie 'a' to 'awire')
		tempConnector = new Connector(tempName.str(), "wire", 1, 0);			//name, type, datawidth, sign
		this->edges.push_back(tempConnector);
		tempLogic = new Logic("if", tempConnector, tempConnector->GetSize(), sign, this->GetIfElseDepth());			//create new "if" node
		this->nodes.push_back(tempLogic);
		tempConnector->AddParent(tempLogic);
		tempLogic->AddParent(tempConnectorUp);									//add the conditional variable as the parent to the "if" node
		tempConnectorUp->AddChild(tempLogic);

		if (this->GetIfElseDepth() > 1) {																			//this checks if this node is part of a if/else/for statement
			tempLogic->SetIfLevelOneOrZero(this->ifForLevelOneOrZero.at(this->GetIfElseDepth() - (int)1));			//takes the value of whether it is a if(true) or else(false) section

			for (i = 0; i < this->nodes.size(); i++) {
				if ((this->nodes.at(i)->GetTypeString() == "if") && (this->nodes.at(i)->GetIfElseDepth() == (this->GetIfElseDepth() - (int)1))) {
					tempLogic->AddParent(this->nodes.at(i)->GetConnector());										//add the "if"'s output edge as the parent to this node
				}
			}
		}
		
	}
	else if (!outputEdge.compare("else")) {											//increases depth 
		this->SetIfElseDepth(this->GetIfElseDepth() + int(1));
		this->SetIfForLevelOneOrZero(this->GetIfElseDepth(), false);
	}
	else if (!outputEdge.compare("for")) {
		inSS >> garbage;														//records begining "("
		//ALL OTHER FOR LOOP INFORMATION
		this->SetIfElseDepth(this->GetIfElseDepth() + int(1));					//increase circuit depth

		//creation of FOR nodes and edges
		cerr << "Error: We were too lazy to implement the For loops." << endl;
		return false;
	}
	else if (!outputEdge.compare("{")) {
		//garbage line, shouldn't happen but covered if so, ifElseDepth increased at if/for initialization not the "{" symbol
	}
	else if (!outputEdge.compare("}")) {					//close of an if/for statement
		this->SetIfElseDepth(this->GetIfElseDepth() - int(1));//decrease circuit depth
		this->ifForLevelOneOrZero.resize(this->GetIfElseDepth());
	}
	else {
		tempConnector = this->findEdge(outputEdge);			//identify which vector edge is associated with the variable
		if (tempConnector == NULL) {
			cerr << "Error: missing output or wire variable callout, improper file format." << endl;
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
			if ((tempChkConnector == NULL) && (variable2 != "1")) { cerr << "Error: missing input variable for datapath component " << endl; return false; }

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

		if (type != "COMP") { tempLogic = new Logic(type, tempConnector, tempConnector->GetSize(), sign, this->GetIfElseDepth()); }	//create the new logic element with its output edge, type and datawidth 
		else if (this->findEdge(variable1)->GetSize() > this->findEdge(variable2)->GetSize()) {				//compare the 2 input edges and use the larger datawidth
			tempLogic = new Logic(type, tempConnector, this->findEdge(variable1)->GetSize(), sign, this->GetIfElseDepth());			//if vector at 0 is bigger, use it
		}
		else { tempLogic = new Logic(type, tempConnector, this->findEdge(variable2)->GetSize(), sign, this->GetIfElseDepth()); }	//if vector at 1 is bigger, use it

		this->nodes.push_back(tempLogic);																	//create new logic/node and add to vector
		tempConnector->AddParent(tempLogic);
		if (tempLogic->GetTypeString() != "REG") { tempLogic->SetOutType(logicSymbol); }					//if the new node is a comparator, record which type it is

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
		if (this->GetIfElseDepth() > 0) {																	//this checks if this node is part of a if/else/for statement
			tempLogic->SetIfLevelOneOrZero(this->ifForLevelOneOrZero.at(this->GetIfElseDepth() - int(1)));			//takes the value of whether it is a if(true) or else(false) section

			for (i = 0; i < this->nodes.size(); i++) {
				if ((this->nodes.at(i)->GetTypeString() == "if") && (this->nodes.at(i)->GetIfElseDepth() == this->GetIfElseDepth())) {
					//this is same level as the "if" statement at this point, bind it as an input

					tempLogic->AddParent(this->nodes.at(i)->GetConnector());								//add the "if"'s output edge as the parent to this node
					this->nodes.at(i)->GetConnector()->AddChild(tempLogic);
				}
			}
		}
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
		for (i = 0; i < 7; i++) { outFS << this->outputEdgeLine("variable", (1 << i)); }				//output all variable variables as variables
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
				outSS << "\t";
				if ((outType == "output") || (outType == "variable")) {	//if the edge is an "output" or a "variable", turn it into a "reg" as well
					outSS << "reg ";
				}
				outSS << outType << " ";
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

bool Netlist::CalculateASAP() {
	int i = 0;
	int j = 0;
	int k = 0;
	int currentLatency = 0;
	int tempNodeDelay = 0;

	for (i = 0; i < this->edges.size(); i++) {
		this->edges.at(i)->SetEdgeASAP(-9);			// negative 9 chosen for default arbitrairily since no node has large enough latency to get to that point when subtracting
	}
	for (i = 0; i < this->nodes.size(); i++) {
		this->nodes.at(i)->SetNodeASAP(-9);			// negative 9 chosen for default arbitrairily since no node has large enough latency to get to that point when subtracting
	}

	for (i = 0; i < this->edges.size(); i++) {				//set the base point for checking the ASAP
		if (this->edges.at(i)->GetType() == "input") {		//if the connector is an input then it will have an ASAP of 0
			this->edges.at(i)->SetEdgeASAP(0);
		}
	}

	for (currentLatency = 0; currentLatency <= this->GetLatency(); currentLatency++) {						//cycle through each latency timeframe
		for (i = 0; i < this->edges.size(); i++) {															//look at all the edges for this time frame
			if (this->edges.at(i)->GetEdgeASAP() == currentLatency) {										//only evaluate edges active during this time frame
				for (j = 0; j < this->edges.at(i)->GetChildVector().size(); j++) {							//loop through all the nodes this edge is a parent of
					if (this->edges.at(i)->GetChildVector().at(j)->GetNodeASAP() <= currentLatency) {		//if the node value is lower value than this time frame update it
						tempNodeDelay = this->edges.at(i)->GetChildVector().at(j)->GetTypeScheduleDelay();	//save this node type scheduleing inherihent delay(ie MUL is 2, DIV/MOD is 3, all other types is 1)
						if ((tempNodeDelay + currentLatency) > this->GetLatency()) {
							cout << "ERROR:  Input Schedule bounds too small for input file." << endl;
							return false;
						}
						this->edges.at(i)->GetChildVector().at(j)->SetNodeASAP(currentLatency + 1);									//update node ASAP value
						if ( this->edges.at(i)->GetChildVector().at(j)->GetConnector() != NULL) {									//make sure the current downstream node has a child edge
							this->edges.at(i)->GetChildVector().at(j)->GetConnector()->SetEdgeASAP(currentLatency + tempNodeDelay);	//update the child edge of the current node to a new ASAP value
						}
					}
					for (k = 0; k < this->edges.at(i)->GetParent().size(); k++) {
						if ((this->edges.at(i)->GetChildVector().at(j)->GetNodeASAP() == currentLatency) && (this->edges.at(i)->GetParent().at(k)->GetTypeString() == "MUL")) {

						}
					}
				}
			}
		}
	}
	if (this->CheckIfASAPDone() == false) {
		cout << "Error is computing ASAP schedule" << endl;		//there is something terribly wrong with the graph if this fails
		return false;
	}

	return true;
}

bool Netlist::CheckIfASAPDone() {
	int i = 0;
	bool check = true;

	for (i = 0; i < this->edges.size(); i++) {
		if (this->edges.at(i)->GetEdgeASAP() == (-9)) {		//this means one of the edges didnt get checked during the schedule
			cerr << "ERROR: ASAP edges not all calculated.";
			return false;
		}
	}

	for (i = 0; i < this->nodes.size(); i++) {
		if (this->nodes.at(i)->GetNodeASAP() == (-9)) {		//this means one of the nodes didnt get checked during the schedule
			cerr << "ERROR: ASAP nodes not all calculated.";
			return false;
		}
	}


	return check;
}


bool Netlist::CalculateALAP() {
	int i = 0;
	int j = 0;
	int k = 0;
	int currentLatency = 0;
	int tempNodeDelay = 0;

	for (i = 0; i < this->edges.size(); i++) {
		this->edges.at(i)->SetEdgeALAP((this->GetLatency()) + 1);			// setting all unscheduled edges to larger than the largest possible latency
	}
	for (i = 0; i < this->nodes.size(); i++) {
		this->nodes.at(i)->SetNodeALAP((this->GetLatency()) + 1);			// setting all unscheduled nodes to larger than the largest possible latency
	}

	for (i = 0; i < this->edges.size(); i++) {								//set the base point for checking the ASAP
		if (!this->edges.at(i)->GetChildVector().size()) {
			this->edges.at(i)->SetEdgeALAP(this->GetLatency());
		}
	}
	for (currentLatency = this->GetLatency(); currentLatency > 0; currentLatency--) {						//cycle through each latency timeframe
		for (i = 0; i < this->edges.size(); i++) {															//look at all the edges for this time frame
			if (this->edges.at(i)->GetEdgeALAP() == currentLatency) {										//only evaluate edges active during this time frame
				for (k = 0; k < this->edges.at(i)->GetParent().size(); k++ ) {								//check if this edge has a parent node
					if (this->edges.at(i)->GetParent().at(k)->GetNodeALAP() > currentLatency) {				//if the node value is higher value than this time frame update it
						tempNodeDelay = this->edges.at(i)->GetParent().at(k)->GetTypeScheduleDelay();		//save this node type scheduleing inherihent delay(ie MUL is 2, DIV/MOD is 3, all other types is 1)
						if ((currentLatency - tempNodeDelay) < 0) {
							cout << "ERROR:  User input Schedule bounds too small for input file." << endl;
							return false;
						}
						this->edges.at(i)->GetParent().at(k)->SetNodeALAP(currentLatency - tempNodeDelay + 1);											//update parent node ALAP value
						for (j = 0; j < this->edges.at(i)->GetParent().at(k)->GetParents().size(); j++ ) {							//make sure the current upstream nodes have a parent edges
							this->edges.at(i)->GetParent().at(k)->GetParents().at(j)->SetEdgeALAP(currentLatency - tempNodeDelay);	//update the parent edge of the current node to a new ALAP value
						}
					}
				}
			}
		}
	}
	if (this->CheckIfALAPDone() == false) {
		cout << "Error is computing ALAP schedule" << endl;		//there is something terribly wrong with the graph if this fails
		return false;
	}

	return true;
}

bool Netlist::CheckIfALAPDone() {
	int i = 0;
	bool check = true;

	for (i = 0; i < this->edges.size(); i++) {
		if (this->edges.at(i)->GetEdgeALAP() == ((this->GetLatency()) + 1)) {	//this means one of the edges didnt get checked during the schedule
			cerr << "ERROR: ALAP edges not all calculated.";
			return false;
		}
	}

	for (i = 0; i < this->nodes.size(); i++) {
		if (this->nodes.at(i)->GetNodeALAP() == ((this->GetLatency()) + 1)) {	//this means one of the nodes didnt get checked during the schedule
			cerr << "ERROR: ALAP nodes not all calculated.";
			return false;
		}
	}


	return check;
}


bool Netlist::CalculateProbabilityFDS() {
	int i = 0;
	int j = 0;
	bool check = true;

	this->nodeProbabilityArray.resize(this->nodes.size());												//DISTRIBUTION ARRAY expanding the array to [#of nodes][#of time slots]
	for (i = 0; i < this->nodes.size(); ++i) {
		this->nodeProbabilityArray[i].resize(this->latency +1);

		this->nodeProbabilityArray[i][0] = 0;															//this is an unused filler value to make calculation easier(ie start at 1 not 0)
		for (j = 1; j < this->nodeProbabilityArray[i].size(); j++) {									//initialize array values to 0
			if ((this->nodes.at(i)->GetNodeASAP() <= j) && (j <= this->nodes.at(i)->GetNodeALAP())) {	//check if current timeframe is in node mobility
				this->nodeProbabilityArray[i][j] = 1/((float)this->nodes.at(i)->GetNodeALAP() - (float)this->nodes.at(i)->GetNodeASAP() + 1);
			}
			else {
				this->nodeProbabilityArray[i][j] = 0;													//if current timeframe is not in schedule of node, set to 0
			}
		}
	}
	
	this->ADDSUBDistribution.clear();
	this->MULDistribution.clear();
	this->DIVMODDistribution.clear();
	this->LOGRESDistribution.clear();
	
	this->ADDSUBDistribution.resize(nodeProbabilityArray[0].size());		//resize distribution vectors to latency width of other arrays
	this->MULDistribution.resize(nodeProbabilityArray[0].size());
	this->DIVMODDistribution.resize(nodeProbabilityArray[0].size());
	this->LOGRESDistribution.resize(nodeProbabilityArray[0].size());

	for (i = 0; i < this->nodeProbabilityArray.size(); i++) {															//loop through each node, calculate the Distribution graphs
		for (j = 1; j < this->nodeProbabilityArray[i].size(); j++) {													//loop through each scheduled time fram
			if ((this->nodes.at(i)->GetTypeString() == "ADD") || (this->nodes.at(i)->GetTypeString() == "SUB")) {		//if node is a ADD or SUB increase it's probability for that time frame
				this->ADDSUBDistribution.at(j) = this->ADDSUBDistribution.at(j) + this->nodeProbabilityArray[i][j];
			}
			else if (this->nodes.at(i)->GetTypeString() == "MUL") {														//if node is a MUL increase it's probability for that time frame
				this->MULDistribution.at(j) = this->MULDistribution.at(j) + this->nodeProbabilityArray[i][j];
			}
			else if ((this->nodes.at(i)->GetTypeString() == "DIV") || (this->nodes.at(i)->GetTypeString() == "MOD")) {	//if node is a DIV or MOD increase it's probability for that time frame
				this->DIVMODDistribution.at(j) = this->DIVMODDistribution.at(j) + this->nodeProbabilityArray[i][j];
			}
			else {																										//else if node is anything else increase it's probability for that time frame
				this->LOGRESDistribution.at(j) = this->LOGRESDistribution.at(j) + this->nodeProbabilityArray[i][j];
			}
		}
	}

	return check;		//might need checks
}


bool Netlist::CalculateForcesFDS() {
	int i = 0;
	int j = 0;
	int k = 0;
	int minNodeNumber = -1;
	int minTimeSlot = -1;
	float minVal = 100;
	bool check = true;
	vector<float> currentDistribution;
	vector<vector<float>> selfForces;													//create vectors to become arrays of the forces
	vector<vector<float>> successorForces;
	vector<vector<float>> predecessorForces;
	vector<vector<float>> totalForces;
	vector<Logic*> tempChildNodes;
	vector<Logic*> tempParentNodes;

	this->CalculateProbabilityFDS();

	selfForces.resize(this->nodes.size());												
	successorForces.resize(this->nodes.size());
	predecessorForces.resize(this->nodes.size());
	totalForces.resize(this->nodes.size());

	for (i = 0; i < this->nodes.size(); ++i) {											//initialize the creation of all the force arrays for each node and time slot
		selfForces[i].resize(this->latency + 1);										//expanding the arrays to [#of nodes][#of time slots]
		successorForces[i].resize(this->latency + 1);
		predecessorForces[i].resize(this->latency + 1);
		totalForces[i].resize(this->latency + 1);

	}

	for (i = 0; i < this->nodes.size(); i++) {																		//CALCULATE SELF FORCES
		if ((this->nodes.at(i)->GetTypeString() == "ADD") || (this->nodes.at(i)->GetTypeString() == "SUB")) {		//if node is a ADD or SUB use that probability for that time frame
			currentDistribution = this->ADDSUBDistribution;
		}
		else if (this->nodes.at(i)->GetTypeString() == "MUL") {														//if node is a MUL use that probability for that time frame
			currentDistribution = this->MULDistribution;
		}
		else if ((this->nodes.at(i)->GetTypeString() == "DIV") || (this->nodes.at(i)->GetTypeString() == "MOD")) {	//if node is a DIV or MOD use that probability for that time frame
			currentDistribution = this->DIVMODDistribution;
		}
		else {																										//else if node is anything else use LOGRES that probability for that time frame
			currentDistribution = this->LOGRESDistribution;
		}
		//CALCULATE SELF FORCES
		for (j = this->nodes.at(i)->GetNodeASAP(); j <= this->nodes.at(i)->GetNodeALAP(); j++) {
			for (k = 1; k <= this->latency; k++) {
				if (k == j) {
					selfForces[i][j] = selfForces[i][j] + currentDistribution[k] * (1 - this->nodeProbabilityArray[i][k]);		//calculate the self force if it is in its time slot
				}
				else {
					selfForces[i][j] = selfForces[i][j] + currentDistribution[k] * (0 - this->nodeProbabilityArray[i][k]);		//calculate the self forces not in the time slot	
				}
			}
		}
	}
	
	for (i = 0; i < this->nodes.size(); i++) {
		
		//CALCULATE SUCCESSOR FORCES
		if (this->nodes.at(i)->GetNodeALAP() != this->GetLatency()) {						//Shortcut to see if you are one of the last nodes (no successor)
			j = this->nodes.at(i)->GetNodeALAP();											//Only will have successor forces when node is in ALAP
			tempChildNodes = this->nodes.at(i)->GetConnector()->GetChildVector();
			if (tempChildNodes.size()) {																				//If you have any immediate children nodes then continue
				for (k = 0; k < tempChildNodes.size(); k++) {															//Loop through all of the child nodes
					if ((j + this->nodes.at(i)->GetTypeScheduleDelay()) == tempChildNodes.at(k)->GetNodeALAP()) {		//If child node has an ALAP time equal to the node time + delay then it is forced, add it
						successorForces[i][j] += selfForces[(FindNodeNumber(tempChildNodes.at(k)))][j + this->nodes.at(i)->GetTypeScheduleDelay()];		//set the succesor forces values
					}
				}
			}
		
		}
		
		//CALCULATE PREDECESSOR FORCES
		if (this->nodes.at(i)->GetNodeASAP() != 1) {							//Shortcut to see if you are at one of the beginning nodes (no predecessors)
			j = this->nodes.at(i)->GetNodeASAP();								//Only will have predecessor force when node is in ASAP
			tempParentNodes = this->nodes.at(i)->GetParentNodes();
			if (tempParentNodes.size()) {																			//If you have any immediate Parent nodes then continue
				for (k = 0; k < tempParentNodes.size(); k++) {															//Loop through all of the Parent nodes
					if ((j - tempParentNodes.at(k)->GetTypeScheduleDelay()) == tempParentNodes.at(k)->GetNodeASAP()) {		//If Parent node has an ASAP time equal to the schedule time - delay of predecessor then it is forced, add it
						predecessorForces[i][j] += selfForces[(FindNodeNumber(tempParentNodes.at(k)))][j - tempParentNodes.at(k)->GetTypeScheduleDelay()];		//set the predecessor forces values
					}
				}
				
			}
		}

		//CALCULATE TOTAL FORCES
		for (j = this->nodes.at(i)->GetNodeASAP(); j <= this->nodes.at(i)->GetNodeALAP(); j++) {			//Loop through all possible time frames
			totalForces[i][j] = selfForces[i][j] + successorForces[i][j] + predecessorForces[i][j];			//Calculate the total force for each node at each time
			if ((totalForces[i][j] <= minVal) && (this->nodes.at(i)->GetNodeFDS() == 0)) {					//Look for the lowest value in the total forces matrix that doesn't already have a force
				minVal = totalForces[i][j];				//Keeps track of the minVal of the total forces matrix
				minNodeNumber = i;						//Keeps track of the node to lock in to place
				minTimeSlot = j;						//Keeps track of the time of the node to schedule
			}
			
		}

	}

	if (this->nodes.at(minNodeNumber)->GetNodeFDS() == 0) {					//After the total forces have been calculated, then check if the node doesn't already have a schedule time
		this->nodes.at(minNodeNumber)->SetNodeFDS(minTimeSlot);				//Set the FDS of the node with the min val in the total force matrix
		this->nodes.at(minNodeNumber)->SetNodeALAP(minTimeSlot);			//Set the ALAP of the node with the min val in the total force matrix 
		this->nodes.at(minNodeNumber)->SetNodeASAP(minTimeSlot);			//Set the ASAP of the node with the min val in the total force matrix

		if (this->nodes.at(minNodeNumber)->GetConnector()->GetChildVector().size()) {		//If there are no child nodes, don't recalculate ASAP
			if (!RecalculateASAP(this->nodes.at(minNodeNumber), minTimeSlot)) {
				cerr << "ERROR in RecalculateASAP" << endl;
			}
		}
		if (this->nodes.at(minNodeNumber)->GetParentNodes().size()) {						//If there are no parent nodes, don't recalculate ALAP
			if (!RecalculateALAP(this->nodes.at(minNodeNumber), minTimeSlot)) {
				cerr << "ERROR in RecalculateALAP" << endl;
			}
		}
		

	}
	return check;		//might need checks
}



bool Netlist::CalculateFDS() {
	int i = 0;
	bool check = true;

	for (i = 0; i < this->nodes.size(); i++) {		//Make it easier to read
		this->nodes.at(i)->SetEasyInputs();
	}

	this->CalculateASAP();
	this->CalculateALAP();

	for (i = 0; i < this->nodes.size(); i++) {		//Calculate the FDS for all of the nodes
		if (!CalculateForcesFDS()) {				//If you screwed up...
			check = false;
		}
	}

	return check;		//might need checks
}


int Netlist::FindNodeNumber(Logic* tempLogic) {
	int i = 0;

	for (i = 0; i < this->nodes.size(); i++) {
		if (tempLogic == this->nodes.at(i)) {
			return i;
		}
	}

	return -1;
}



bool Netlist::outputHLSMModule(string outputFilename) {									//write all current data into a verilog module
	ofstream outFS;																		// Output file stream
	string truncatedFilename = "";
	unsigned int i = 0;
	unsigned int j = 0;

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
		outFS << "module " << "HLSM " << "(Clk, Rst, Start, Done";

		for (i = 0; i < this->edges.size(); i++) {										//lists all of the inputs and outputs names into the module prototype
			if ((this->edges.at(i)->GetType() == "input") || (this->edges.at(i)->GetType() == "output")) {
				outFS << ", " << this->edges.at(i)->GetName();
			}
		}
		outFS << ");" << endl;

		outFS << "\t" << "input Clk, Rst, Start;" << endl;
		outFS << "\t" << "output reg Done;" << endl;

		for (i = 0; i < 7; i++) { outFS << this->outputEdgeLine("input", (1 << i)); }					//output all input variables
		outFS << endl;
		for (i = 0; i < 7; i++) { outFS << this->outputEdgeLine("output", (1 << i)); }					//output all output variables
		outFS << endl;
		for (i = 0; i < 7; i++) { outFS << this->outputEdgeLine("wire", (1 << i)); }					//output all wire variables
		for (i = 0; i < 7; i++) { outFS << this->outputEdgeLine("register", (1 << i)); }				//output all register variables as wire variables
		for (i = 0; i < 7; i++) { outFS << this->outputEdgeLine("variable", (1 << i)); }				//output all variable variables as variables
		
		outFS << "\t" << "reg State, NextState;" << endl;												//might need a bit width for State...probably use
		outFS << endl;
		outFS << "\t" << "always @(State) begin" << endl;
		outFS << "\t" << "\t" << "case (State)" << endl;
		outFS << "\t" << "\t" << "\t" << "0: begin" << endl;
		outFS << "\t" << "\t" << "\t" << "\t" << "if (Start == 1)" << endl;
		outFS << "\t" << "\t" << "\t" << "\t" << "\t" << "NextState <= 1" << endl;
		outFS << "\t" << "\t" << "\t" << "\t" << "else" << endl;
		outFS << "\t" << "\t" << "\t" << "\t" << "\t" << "NextState <= 0" << endl;
		outFS << "\t" << "\t" << "\t" << "end" << endl;

		for (i = 1; i < this->cases.size()-1; i++) {
			outFS << "\t" << "\t" << "\t" << i << ": begin" << endl;
			for (j = 0; j < this->cases.at(i)->GetCaseNodes().size(); j++) {
				outFS << this->outputCaseLine(this->cases.at(i)->GetCaseNodes().at(j)) << endl;
				if (this->cases.at(i)->GetCaseNodes().at(j)->GetTypeString() == "if") {
					outFS << "\t" << "\t" << "\t" << "\t" << "\t" << "NextState <= " << this->cases.at(i)->GetChildCases().at(0)->GetCaseNumber() << endl;
						outFS << "\t" << "\t" << "\t" << "\t" << "else" << endl;
					outFS << "\t" << "\t" << "\t" << "\t" << "\t" << "NextState <= " << this->cases.at(i)->GetChildCases().at(1)->GetCaseNumber() << endl;
				}
			}
			if (this->cases.at(i)->GetChildCases().size() == 1) {
				outFS << "\t" << "\t" << "\t" << "\t" << "NextState <= " << this->cases.at(i)->GetChildCases().at(0)->GetCaseNumber() << endl;
			}
			outFS << "\t" << "\t" << "\t" << "end" << endl;
		}

		outFS << "\t" << "\t" << "endcase" << endl << endl;
		outFS << "\t" << "end" << endl << endl;

		outFS << "\t" << "always @(posedge Clk) begin" << endl;
		outFS << "\t" << "\t" << "if (Rst == 1 )" << endl;
		outFS << "\t" << "\t" << "\t" << "State <= 0" << endl;											//state 0 is wait state
		outFS << "\t" << "\t" << "else" << endl;
		outFS << "\t" << "\t" << "\t" << "State <= NextState" << endl;
		outFS << "\t" << "end" << endl;
		outFS << "endmodule" << endl;
	}

	cout << "done?";
	outFS.close();		//close the output file
	return true;
}


string Netlist::outputCaseLine(Logic* caseNode) {
	Connector *tempConnector = caseNode->GetConnector();			//c = a + b this is the "c" element
	Connector *tempParent0 = NULL;													//c = a + b this is the "a" element
	Connector *tempParent1 = NULL;
	Connector *tempParent2 = NULL;
	Connector *tempParent3 = NULL;
	ostringstream outSS;
	unsigned int i = 0; 

	for (i = 0; i < caseNode->GetParents().size(); i++) {
		switch (i) {
		case 0: tempParent0 = caseNode->GetParents().at(0);
			break;
		case 1: tempParent1 = caseNode->GetParents().at(1);
			break;
		case 2: tempParent2 = caseNode->GetParents().at(2);
			break;
		case 3: tempParent3 = caseNode->GetParents().at(3);
			break;
		}
	}

	if (caseNode->GetTypeString() == "if") {

		outSS << "\t\t\t\t" << "if (" << caseNode->GetParents().at(0)->GetName() << " == 1)" ;		//puts the if statement parent edge name in
	}
	else	{

		outSS << "\t\t\t\t\t" << tempConnector->GetName() << " = ";						//print out the output edge(ie 'b = ')
		if (tempParent0 != NULL) {
			outSS << tempParent0->GetName() << " ";										//print out to first input edge(ie 'b = c')
		}
		if (caseNode->GetTypeString() == "MUX2x1") {
			outSS << "? ";																//print out mux symbol(ie 'b = c ? ')
			if (tempParent1 != NULL) {
				outSS << tempParent1->GetName() << " : ";								//print out second input edge and mux symbol(ie 'b = c ? d : ')
			}
			if (tempParent2 != NULL) {
				outSS << tempParent2->GetName();										//print out third input edge and mux symbol(ie 'b = c ? d : e')
			}
		}
		else if ((caseNode->GetTypeString() == "INC") || (caseNode->GetTypeString() == "DEC")) {
			outSS << caseNode->GetOutType() << " 1";					//print out inc/dec symbol and 1(ie 'b = c + 1')
		}
		else {
			outSS << caseNode->GetOutType() << " ";					//print out component symbol(ie 'b = c % ')
			if (tempParent1 != NULL) {
				outSS << tempParent1->GetName();										//print out second input edge(ie 'b = c % d')
			}
		}
		outSS << ";";
	}
	return outSS.str();
}

bool Netlist::RecalculateASAP(Logic* inputNode, int minTime) {
	int i = 0;
	int j = 0;
	int k = 0;
	bool check = true;
	Logic* tempNode;

	for (i = 0; i < inputNode->GetConnector()->GetChildVector().size(); i++) {				//loop through all of the children nodes of the node that has been locked
		tempNode = inputNode->GetConnector()->GetChildVector().at(i);						//for ease of typing
		if (tempNode->GetNodeASAP() <= (minTime + inputNode->GetTypeScheduleDelay() - 1)) {	//If the ASAP time of one of the child nodes is <= end of node duration
			tempNode->SetNodeASAP(minTime + tempNode->GetTypeScheduleDelay());				//Reassign ASAP time of the parent node
			this->RecalculateASAP(tempNode, tempNode->GetNodeASAP());						//Recursively call function to act on all children nodes
		}
	}

	return check;

}

bool Netlist::RecalculateALAP(Logic* inputNode, int minTime) {
	int i = 0;
	int j = 0;
	int k = 0;
	bool check = true;
	Logic* tempNode;

	for (i = 0; i < inputNode->GetParentNodes().size(); i++) {							//loop through all of the parents of the node that has been locked
		tempNode = inputNode->GetParentNodes().at(i);									//for ease of typing
		if (tempNode->GetNodeALAP() >= minTime) {										//If the ALAP time of one of the parent nodes is >= minTime
			tempNode->SetNodeALAP(minTime - tempNode->GetTypeScheduleDelay());			//Reassign ALAP time of the parent node
			this->RecalculateALAP(tempNode, tempNode->GetNodeALAP());					//Recursively call function to act on all parent nodes
		}
	}

	return check;
}

bool Netlist::CalculateCaseStates() {
	int i = 0;
	int j = 0;
	int k = 0;
	int m = 0;
	int n = 0;
	int tempNextLatency = 0;
	bool check = true;
	bool firstCaseAtLatency = true;
	bool generatedExtraCase = false;
	StateCase *tempCase = NULL;
	StateCase *tempCaseFalse = NULL;
	int caseCounter = 2;

	tempCase = new StateCase(0, 0);						//create new BEGIN '0' case statement (counter, latency)
	this->cases.push_back(tempCase);
	tempCase = new StateCase(1, 1);						//create new '1' case statement
	this->cases.push_back(tempCase);
	tempCase->AddParentCase(this->cases.at(0));			//link the 0 and 1 case statements together
	this->cases.at(0)->AddChildrenCase(tempCase);

	for (i = 0; i < this->nodes.size(); i++) {
		if (this->nodes.at(i)->GetNodeFDS() == 1) {
			tempCase->AddNodeToCase(this->nodes.at(i));						//add all nodes that are in case 1 to case 1
			this->nodes.at(i)->SetScheduled(true);							//mark all nodes in this case as having been scheduled
		}
	}



	for (i = 1; i <= this->GetLatency(); i++) {
		firstCaseAtLatency = true;										//each new latency will need atleast 1 new case object if something is scheduled

		for (j = 0; j < this->cases.size(); j++) {
			if (this->cases.at(j)->GetLatencyLevel() == i) {			//go through cases looking at all the ones with the same latency level as current level(i)
																
				for (k = 0; k < this->cases.at(j)->GetCaseNodes().size(); k++) {					//move all nodes that are in the wrong level down a level
					if (this->cases.at(j)->GetCaseNodes().at(k)->GetNodeFDS() != i) {				//if this case has a node with a different(downsteam) latency level, shove it downstream to the next/a new case state

						if (this->cases.at(j)->GetChildCases().size() == 0) {						//if there isnt currently a child case to this current case, create one, else shove the unwanted higher latency nodes down it
							firstCaseAtLatency = false;
							tempCase = new StateCase(caseCounter, i + 1);							//create new 'child' case statement
							caseCounter++;
							this->cases.push_back(tempCase);
							tempCase->SetTrueFalseCase(true);
							this->cases.at(j)->AddChildrenCase(tempCase);
							tempCase->AddParentCase(this->cases.at(j));

							tempCase->AddNodeToCase(this->cases.at(j)->GetCaseNodes().at(k));						//add the unwanted node down to the new case state
							this->cases.at(j)->RemoveCaseNode(k);													//removes the unwanted node from the current case state
							k--;																					//a node was removed, interate back down 1 k value	
						}
						else {
							this->cases.at(j)->GetChildCases().at(0)->AddNodeToCase(this->cases.at(j)->GetCaseNodes().at(k));	//add the unwanted node down to the case state
							this->cases.at(j)->RemoveCaseNode(k);																//removes the unwanted node from the current case state
							k--;																								//a node was removed, interate back down 1 k value		
						}
					}
				}

				for (k = 0; k < this->cases.at(j)->GetCaseNodes().size(); k++) {				//loop through all case nodes
					if (this->cases.at(j)->GetCaseNodes().at(k)->GetTypeString() == "if") {		//node at 'k' is an if statement
						if (firstCaseAtLatency == false) {
							firstCaseAtLatency = false;
							tempCase = new StateCase(caseCounter, i + 1);						//create new 'if(1) child' case statement
							caseCounter++;
							this->cases.push_back(tempCase);
							tempCase->SetTrueFalseCase(true);
							this->cases.at(j)->AddChildrenCase(tempCase);
							tempCase->AddParentCase(this->cases.at(j));

							tempCaseFalse = new StateCase(caseCounter, i + 1);					//create new 'if(0) child' case statement
							caseCounter++;
							this->cases.push_back(tempCaseFalse);
							tempCaseFalse->SetTrueFalseCase(false);
							this->cases.at(j)->AddChildrenCase(tempCaseFalse);
							tempCaseFalse->AddParentCase(this->cases.at(j));

							for (m = 0; m < this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().size(); m++) { //look at all of the 'k' 'if' node children and sort them by 'if(1)' and 'if(0)'
								if (this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().at(m)->GetIfLevelOneOrZero() == true) {
									tempCase->AddNodeToCase(this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().at(m));						//add all nodes that are in 'true' case to tempcase
									this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().at(m)->SetScheduled(true);							//mark all nodes in this case as having been scheduled
									tempCase->SetIfElseDepthMax(this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().at(m)->GetIfElseDepth());	//sets the current case ifelse depth to proper depth
								}
								else {
									tempCaseFalse->AddNodeToCase(this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().at(m));					//add all nodes that are in 'true' case to tempcase
									this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().at(m)->SetScheduled(true);							//mark all nodes in this case as having been scheduled
									tempCase->SetIfElseDepthMax(this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().at(m)->GetIfElseDepth());	//sets the current case ifelse depth to proper depth
								}
							}
							for (m = 0; m < this->nodes.size(); m++) {		//loop through all nodes and add any that are depth 0, add them to the 'if(1)' and 'if(0)'
								if ((this->nodes.at(m)->GetNodeFDS() == (i + 1)) && (this->nodes.at(m)->GetIfElseDepth() == 0)) {
									tempCase->AddNodeToCase(this->nodes.at(m));
									this->nodes.at(m)->SetScheduled(true);
									tempCaseFalse->AddNodeToCase(this->nodes.at(m));
								}
							}
							tempCase->RemoveDuplicateNodes();						//incase of any overlap of nodes, remove duplicates
						}
						else {
							tempCase = this->cases.at(j)->GetChildCases().at(0);	//existing 'if(1)' child case statement

							tempCaseFalse = new StateCase(caseCounter, i + 1);		//create new 'if(0) child' case statement
							caseCounter++;
							this->cases.push_back(tempCaseFalse);
							tempCaseFalse->SetTrueFalseCase(false);
							this->cases.at(j)->AddChildrenCase(tempCaseFalse);
							tempCaseFalse->AddParentCase(this->cases.at(j));

							for (m = 0; m < tempCase->GetCaseNodes().size(); m++) {
								if (tempCase->GetCaseNodes().at(m)->GetIfLevelOneOrZero() == false) {		//iterate through and move nodes from 'if(1)' branch to if(0) branch if in wrong one
									tempCaseFalse->AddNodeToCase(tempCase->GetCaseNodes().at(m));
									tempCase->RemoveCaseNode(m);
									m--;
								}
							}
							
							for (m = 0; m < this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().size(); m++) { //look at all of the 'k' 'if' node children and sort them by 'if(1)' and 'if(0)'
								if (this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().at(m)->GetIfLevelOneOrZero() == true) {
									tempCase->AddNodeToCase(this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().at(m));						//add all nodes that are in 'true' case to tempcase
									this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().at(m)->SetScheduled(true);							//mark all nodes in this case as having been scheduled
									tempCase->SetIfElseDepthMax(this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().at(m)->GetIfElseDepth());	//sets the current case ifelse depth to proper depth
								}
								else {
									tempCaseFalse->AddNodeToCase(this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().at(m));					//add all nodes that are in 'true' case to tempcase
									this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().at(m)->SetScheduled(true);							//mark all nodes in this case as having been scheduled
									tempCase->SetIfElseDepthMax(this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().at(m)->GetIfElseDepth());	//sets the current case ifelse depth to proper depth
								}
							}
							for (m = 0; m < this->nodes.size(); m++) {		//loop through all nodes and add any that are depth 0, add them to the 'if(1)' and 'if(0)'
								if ((this->nodes.at(m)->GetNodeFDS() == (i + 1)) && (this->nodes.at(m)->GetIfElseDepth() == 0)) {
									tempCase->AddNodeToCase(this->nodes.at(m));
									this->nodes.at(m)->SetScheduled(true);
									tempCaseFalse->AddNodeToCase(this->nodes.at(m));
								}
							}
							////remove duplicate nodes
							tempCase->RemoveDuplicateNodes();					//incase of any overlap of nodes, remove duplicates
							tempCaseFalse->RemoveDuplicateNodes();				//incase of any overlap of nodes, remove duplicates
						}
					}
				}
				generatedExtraCase = false;
				for (k = 0; k < this->cases.at(j)->GetCaseNodes().size(); k++) {				//loop through all of this case states nodes
					if ((this->cases.at(j)->GetCaseNodes().at(k)->GetTypeString() != "if") && (this->cases.at(j)->GetCaseNodes().at(k)->GetIfElseDepth() >0)) {		//check if something is teired down if node from above
						if (generatedExtraCase == false) {											//if this is the first item found
							firstCaseAtLatency = false;
							tempCase = new StateCase(caseCounter, i + 1);							//create new case statement
							caseCounter++;
							this->cases.push_back(tempCase);
							tempCase->SetTrueFalseCase(true);
							this->cases.at(j)->AddChildrenCase(tempCase);
							tempCase->AddParentCase(this->cases.at(j));

							for (m = 0; m < this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().size(); m++) {	//loop through all the cases' node's children nodes(ie nodes for the next case layer)
								if (this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().at(m)->GetIfElseDepth() == this->cases.at(j)->GetCaseNodes().at(k)->GetIfElseDepth()) { //if the cases' node component's child node is not the same ifelse depth, it is outside of the depth
									tempCase->AddNodeToCase(this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().at(m));	//add the cases' node's child node to the next downstream case statement
									this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().at(m)->SetScheduled(true);							//mark all nodes in this case as having been scheduled
									tempCase->SetIfElseDepthMax(this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().at(m)->GetIfElseDepth());	//sets the current case ifelse depth to proper depth
								}
								else {
									//need to do something if it is not nested in the if statement....(ie a downsteam node that is outside the if statement)
								}
							}
							tempCase->RemoveDuplicateNodes();					//incase of any overlap of nodes, remove duplicates
						}
						else {
							for (m = 0; m < this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().size(); m++) {	//loop through all the cases' node's children nodes(ie nodes for the next case layer)
								if (this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().at(m)->GetIfElseDepth() == this->cases.at(j)->GetCaseNodes().at(k)->GetIfElseDepth()) { //if the cases' node component's child node is not the same ifelse depth, it is outside of the depth
									tempCase->AddNodeToCase(this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().at(m));	//add the cases' node's child node to the next downstream case statement
									this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().at(m)->SetScheduled(true);							//mark all nodes in this case as having been scheduled
									tempCase->SetIfElseDepthMax(this->cases.at(j)->GetCaseNodes().at(k)->GetConnector()->GetChildVector().at(m)->GetIfElseDepth());	//sets the current case ifelse depth to proper depth
								}
								else {
									//need to do something if it is not nested in the if statement....(ie a downsteam node that is outside the if statement)
								}
							}
							tempCase->RemoveDuplicateNodes();					//incase of any overlap of nodes, remove duplicates
						}
					}
				}
			}
		}
		if (firstCaseAtLatency == true) {													//if there were no other if branches made this round
			for (j = 0; j < this->cases.size(); j++) {
				if (this->cases.at(j)->GetLatencyLevel() == i) {
					if (this->cases.at(j)->GetChildCases().size() == 0) {
						tempCase = new StateCase(caseCounter, i + 1);							//create new case statement
						caseCounter++;
						this->cases.push_back(tempCase);
						tempCase->SetTrueFalseCase(true);
						this->cases.at(j)->AddChildrenCase(tempCase);
						tempCase->AddParentCase(this->cases.at(j));
						//////there are no imbeded if cases, everything above this should point to this tempCase?

						for (k = 0; k < this->nodes.size(); k++) {		//loop through all nodes and add any that are depth 0, add them to this case state
							if ((this->nodes.at(k)->GetNodeFDS() == (i + 1)) && (this->nodes.at(k)->GetIfElseDepth() == 0)) {
								tempCase->AddNodeToCase(this->nodes.at(k));
								this->nodes.at(k)->SetScheduled(true);
							}
						}
					}
					else {
						tempCase = this->cases.at(j)->GetChildCases().at(0);					//existing child case statement
						for (k = 0; k < this->nodes.size(); k++) {		//loop through all nodes and add any that are depth 0, add them to this case state
							if ((this->nodes.at(k)->GetNodeFDS() == (i + 1)) && (this->nodes.at(k)->GetIfElseDepth() == 0)) {
								tempCase->AddNodeToCase(this->nodes.at(k));
								this->nodes.at(k)->SetScheduled(true);
							}
						}
					}
					tempCase->RemoveDuplicateNodes();					//incase of any overlap of nodes, remove duplicates
				}
			}
		}


	}

	//Connect all loose ends to a final 'end' case state
	tempCase = new StateCase(caseCounter, this->GetLatency() + 1);							//create new 'end' case statement after latency point
	caseCounter++;
	this->cases.push_back(tempCase);
	tempCase->SetTrueFalseCase(true);

	for (i = 0; i < this->cases.size() - 1; i++) {
		if (this->cases.at(i)->GetChildCases().size() == 0) {								//if the are any cases floating without downstream cases, connect them to the 'end' case
			this->cases.at(i)->AddChildrenCase(tempCase);
		}
	}

	//REMOVE EMPTY CASES
	this->RemoveAllEmptyCases();

	//REMOVE DUPLICATE CASE STATES
	this->RemoveAllDuplicateCases();


	//RENUMBER ALL CASE STATES
	for (i = 0; i < this->cases.size(); i++) {
		this->cases.at(i)->SetCaseNumber(i);
	}


	return check;		//might need checks
}

void Netlist::RemoveAllDuplicateCases() {
	int i = 0;
	int j = 0;
	int k = 0;
	int m = 0;
	int firstCaseNodeCounter = 0;
	int secondCaseNodeCounter = 0;
	for (i = this->GetLatency(); i >1; i--) {
		for (j = this->cases.size() - (int)1; j >= 0; j--) {
			if (this->cases.at(j)->GetLatencyLevel() == i) {

				for (k = j - (int)1; k >= 0; k--) {
					if (this->cases.at(k)->GetLatencyLevel() == i) {
						if (this->cases.at(j)->CheckDuplicateCase(this->cases.at(k)) == true) {							//check if the 2 cases in this latency are the same
							this->cases.at(j)->AddParentCase(this->cases.at(k)->GetParentCases().at(0));										//branch one of the parents to the other case
							this->cases.at(j)->GetParentCases().at(this->cases.at(j)->GetParentCases().size() - 1)->GetChildCases().pop_back();	//remove the parent's child of the case to be discarded
							this->cases.at(j)->GetParentCases().at(this->cases.at(j)->GetParentCases().size() - 1)->AddChildrenCase(this->cases.at(j));	//add the case to keep to the other cases parent child vector
							for (m = 0; m < this->cases.at(k)->GetChildCases().at(0)->GetParentCases().size(); m++) {							//loop through the case to discards child parents until it locates itself
								if (this->cases.at(k)->GetChildCases().at(0)->GetParentCases().at(m) == this->cases.at(k)) {
									this->cases.at(k)->GetChildCases().at(0)->RemoveParentCase( m);	//once the case has located itself from its child's parent list, remove it
									break;
								}
							}
							delete this->cases.at(k);																	//free this case statement memory
							this->cases.erase(this->cases.begin() + k);													//remove this case statement from the list
						}

					}
				}
			}
		}
	}
}

void Netlist::RemoveAllEmptyCases() {
	int i = 0;
	int j = 0;
	int k = 0;
	int m = 0;
	int n = 0;
	StateCase* tempCase;

	for (i = 1; i < this->cases.size() - 1; i++) {
		if (this->cases.at(i)->GetCaseNodes().size() == 0) {								//if the are any cases without any nodes, connect the parent case and child case of this case and remove it
			tempCase = this->cases.at(i)->GetChildCases().at(0);
			tempCase->AddParentCase(this->cases.at(i)->GetParentCases().at(0));
			for (j = 0; j < tempCase->GetParentCases().size(); j++) {
				if (tempCase->GetParentCases().at(j) == this->cases.at(i)) {
					tempCase->RemoveParentCase( j);
					break;
				}
			}
			tempCase = this->cases.at(i)->GetParentCases().at(0);
			tempCase->AddChildrenCase(this->cases.at(i)->GetChildCases().at(0));
			for (j = 0; j < tempCase->GetChildCases().size(); j++) {
				if (tempCase->GetChildCases().at(j) == this->cases.at(i)) {
					tempCase->RemoveChildCase( j);
					break;
				}
			}
			delete this->cases.at(i);
			this->cases.erase(this->cases.begin() + i);
		}
	}

}

Netlist::Netlist(void) {															//CONSTRUCTOR...
	this->ifElseDepth = 0;
	this->ifForIncrementer = 0;
}

Netlist::~Netlist(void) {															//DeSTRUCTOR!!!!!!!!~!~!
	unsigned int i = 0;

	for (i = this->edges.size(); i > 0; i--) {
		delete this->edges.at(i - 1);
		this->edges.pop_back();
	}
	for (i = this->nodes.size(); i > 0; i--) {
		delete this->nodes.at(i - 1);
		this->nodes.pop_back();
	}
	for (i = this->cases.size(); i > 0; i--) {
		delete this->cases.at(i - 1);
		this->cases.pop_back();
	}

}

