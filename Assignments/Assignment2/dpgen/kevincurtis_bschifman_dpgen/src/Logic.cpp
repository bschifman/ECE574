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

using namespace std;


Logic::Logic(void) {

}

Logic::Logic(string type, Connector *logicOutput, int dataWidth, bool sign) {	//overload constructor
	this->SetType(type);
	this->SetDataWidth(dataWidth);
	this->SetConnector(logicOutput);
	this->SetInherentDelay();
	this->SetSign(sign);
}

Logic::~Logic(void) {

}

//Logic mutator, will set recieved "type" into a integer class variable. Allowing use of switch statements
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
	else { return NULL; }
}

void Logic::SetDataWidth(int inputDataWidth) {
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

int Logic::GetDataWidth() {
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

void Logic::SetInherentDelay() {

	//						0	1-bit	2-bit	4-bit	8-bit	16-bit	32-bit	64-bit
	float regArray[8] = { 0,	2.616,	2.644,	0,		2.879,	3.061,	3.602,	3.966 };	//REG
	float addArray[8] = { 0,	2.704,	3.713,	0,		4.924,	5.638,	7.270,	9.566 };	//Add
	float subArray[8] = { 0,	3.024,	3.412,	0,		4.890,	5.569,	7.253,	9.566 };	//SUB
	float mulArray[8] = { 0,	2.438,	3.651,	0,		7.453,	7.811,	12.395,	15.354 };	//MUL
	float compArray[8] = { 0,	3.031,	3.934,	0,		5.949,	6.256,	7.264,	8.416 };	//COMP
	float muxArray[8] = { 0,	4.083,	4.115,	0,		4.815,	5.623,	8.079,	8.766 };	//MUX2x1
	float shrArray[8] = { 0,	3.644,	4.007,	0,		5.178,	6.460,	8.819,	11.095 };	//SHR
	float shlArray[8] = { 0,	3.614,	3.980,	0,		5.152,	6.549,	8.565,	11.220 };	//SHL
	float divArray[8] = { 0,	0.619,	2.144,	0,		15.439, 33.093,	86.312,	243.233 };	//DIV
	float modArray[8] = { 0,	0.758,	2.149,	0,		16.078, 35.563,	88.142,	250.583 };	//MOD
	float incArray[8] = { 0,	1.792,	2.218,	0,		3.111,	3.471,	4.347,	6.200 };	//INC
	float decArray[8] = { 0,	1.792,	2.218,	0,		3.108,	3.701,	4.685,	6.503 };	//DEC

	switch (this->GetType())
	{
	case(1):
		this->delay = regArray[this->dataWidth];
		break;
	case(2):
		this->delay = addArray[this->dataWidth];
		break;
	case(3):
		this->delay = subArray[this->dataWidth];
		break;
	case(4):
		this->delay = mulArray[this->dataWidth];
		break;
	case(5):
		this->delay = compArray[this->dataWidth];
		break;
	case(6):
		this->delay = muxArray[this->dataWidth];
		break;
	case(7):
		this->delay = shrArray[this->dataWidth];
		break;
	case(8):
		this->delay = shlArray[this->dataWidth];
		break;
	case(9):
		this->delay = divArray[this->dataWidth];
		break;
	case(10):
		this->delay = modArray[this->dataWidth];
		break;
	case(11):
		this->delay = incArray[this->dataWidth];
		break;
	case(12):
		this->delay = decArray[this->dataWidth];
		break;
	default:
		this->delay = 0;
		break;
	}


	return;
}
