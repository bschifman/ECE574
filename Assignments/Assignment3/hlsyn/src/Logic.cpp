/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: Logic.cpp
*************************************/

#include <iostream>
#include <vector>
#include <string>

#include "Logic.h"
#include "Connector.h"

using namespace std;


Logic::Logic(void) {
	this->SetDataWidth(int(0));
	this->SetSign(int(0));
	this->SetNodeASAP(int(0));
	this->SetNodeALAP(int(0));
	this->SetNodeFDS(int(0));
	this->SetIfLevelOneOrZero(true);

}

Logic::Logic(string type, Connector *logicOutput, int dataWidth, bool sign, int depth, int size) {	//overload constructor
	this->SetType(type);
	this->SetDataWidth(dataWidth);
	this->SetConnector(logicOutput);
	this->SetInherentDelay();
	this->SetSign(sign);
	this->SetNodeASAP(int(0));
	this->SetNodeALAP(int(0));
	this->SetNodeFDS(int(0));
	this->SetIfElseDepth(depth);
	this->SetIfLevelOneOrZero(true);
	this->SetNodeNumber(size);
}

Logic::~Logic(void) {

}

//Logic mutator, will set recieved "type" into an integer class variable. Allowing use of switch statements
void Logic::SetType(string inputType) {
	this->typeName = inputType;
	if (inputType == "REG") { this->type = 1; }
	else if (inputType == "ADD") { this->type = 2; }
	else if (inputType == "SUB") { this->type = 3; }
	else if (inputType == "MUL") { this->type = 4; }
	else if (inputType == "COMP") { this->type = 5; }
	else if (inputType == "MUX2x1") { this->type = 6; }
	else if (inputType == "SHR") { this->type = 7; }
	else if (inputType == "SHL") { this->type = 8; }
	else if (inputType == "DIV") { this->type = 9; }
	else if (inputType == "MOD") { this->type = 10; }
	else if (inputType == "INC") { this->type = 11; }
	else if (inputType == "DEC") { this->type = 12; }
	else if (inputType == "if") { this->type = 13; }
	else if (inputType == "for") { this->type = 14; }
	else { this->type = 0; }

	return;
}

//Get string "type" of variable, this converts the integer value to a name for outputting
string Logic::GetTypeString() {
	if (this->type == 1) { return "REG"; }
	else if (this->type == 2) { return "ADD"; }
	else if (this->type == 3) { return "SUB"; }
	else if (this->type == 4) { return "MUL"; }
	else if (this->type == 5) { return "COMP"; }
	else if (this->type == 6) { return "MUX2x1"; }
	else if (this->type == 7) { return "SHR"; }
	else if (this->type == 8) { return "SHL"; }
	else if (this->type == 9) { return "DIV"; }
	else if (this->type == 10) { return "MOD"; }
	else if (this->type == 11) { return "INC"; }
	else if (this->type == 12) { return "DEC"; }
	else if (this->type == 13) { return "if"; }
	else if (this->type == 14) { return "for"; }
	else { return NULL; }
}

void Logic::SetDataWidth(int inputDataWidth) {		//stores the datawidth as a sequential int for use in switch statements and arrays
	if (inputDataWidth == 1) { this->dataWidth = 1; }
	else if (inputDataWidth == 2) { this->dataWidth = 2; }
	else if (inputDataWidth == 4) { this->dataWidth = 3; }
	else if (inputDataWidth == 8) { this->dataWidth = 4; }
	else if (inputDataWidth == 16) { this->dataWidth = 5; }
	else if (inputDataWidth == 32) { this->dataWidth = 6; }
	else if (inputDataWidth == 64) { this->dataWidth = 7; }
	else { this->dataWidth = 0; }

	return;
}

int Logic::GetDataWidth() {							//returns true data width from the stored sequential integer value
	if (this->dataWidth == 1) { return 1; }
	else if (this->dataWidth == 2) { return 2; }
	else if (this->dataWidth == 3) { return 4; }
	else if (this->dataWidth == 4) { return 8; }
	else if (this->dataWidth == 5) { return 16; }
	else if (this->dataWidth == 6) { return 32; }
	else if (this->dataWidth == 7) { return 64; }
	else { return false; }

	return true;
}


vector <Logic*> Logic::GetParentNodes() {
	vector<Connector*> tempConnectors;
	vector<Logic*> tempParentNodes;
	unsigned int i = 0;
	unsigned int j = 0;

	tempConnectors = this->GetParents();
	for (i = 0; i < tempConnectors.size(); i++) {
		for (j = 0; j < tempConnectors.at(i)->GetParent().size(); j++ ) {
			tempParentNodes.push_back(tempConnectors.at(i)->GetParent().at(j));
		}
	}

	return tempParentNodes;
}

void Logic::SetEasyInputs() {
	unsigned int i = 0;

	for (i = 0; i < this->GetParents().size(); i++) {
		this->easyInputs = this->easyInputs + this->GetParents().at(i)->GetName() + ' ';
	}

	return;
}

void Logic::SetInherentDelay() {

	switch (this->GetType())
	{
	case(1):
		this->scheduleDelayValue = 1;
		break;
	case(2):
		this->scheduleDelayValue = 1;
		break;
	case(3):
		this->scheduleDelayValue = 1;
		break;
	case(4):
		this->scheduleDelayValue = 2;		//Multiplier
		break;
	case(5):
		this->scheduleDelayValue = 1;
		break;
	case(6):
		this->scheduleDelayValue = 1;
		break;
	case(7):
		this->scheduleDelayValue = 1;
		break;
	case(8):
		this->scheduleDelayValue = 1;
		break;
	case(9):
		this->scheduleDelayValue = 3;		//Divider
		break;
	case(10):
		this->scheduleDelayValue = 3;		//Modulo
		break;
	case(11):
		this->scheduleDelayValue = 1;
		break;
	case(12):
		this->scheduleDelayValue = 1;
		break;
	case(13):
		this->scheduleDelayValue = 1;
		break;
	case(14):
		this->scheduleDelayValue = 1;
		break;
	default:
		this->scheduleDelayValue = 0;
		break;
	}


	return;
}