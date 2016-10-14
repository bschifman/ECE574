/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: Logic.h
*************************************/
#include <iostream>
#include <vector>
#include <string>
#include "Connector.h"
using namespace std;

#ifndef LOGIC_H
#define LOGIC



/*This object is the various*/
class Logic		//nodes
{
public:
	Logic(string type, Connector *logicOutput, int dataWidth) { this->dataWidth = dataWidth;	this->logicOutput = logicOutput;	this->delay = this->GetDelay(); };				//constructor
		~Logic() {};			//destructor
		//Setters
		void SetType(std::string inputType);
		void SetName(std::string name) { this->name = name; }
		void SetL_inputs(std::string l_inputs) { this->l_inputs = l_inputs; }
		void SetOutType(std::string outType) { this->outType = outType; }
		void SetL_outputs(std::string l_inputs) { this->l_outputs = l_outputs; }
		void SetDataWidth(int inputDataWidth);
		void SetSign(bool sign) { this->sign = sign; }
		void SetDelay(); 
		//Getters
		std::string GetName() { return this->name; }
		int GetType() { return this->type; }
		std::string GetTypeString();
		std::string Getl_inputs() { return this->l_inputs; }
		std::string Getl_outputs() { return this->l_outputs; }
		std::string GetOutType() { return this->outType; }
		int GetDataWidth();
		float GetDelay() { return this->delay; }
		bool GetSign() { return this->sign; }
		
private:
		int type;
		std::string name;
//		vector<Connector*> logicInputs;	//inputs for Logic object, might not need since graph is directional, would just know since this Logic would be on the edge list of connections
		Connector *logicOutput;	//Outputs for logic object(ie comparator might have gt, ln, eq)	//could get away without making a vector if you just made a comparator for each logic line, wouldn't change delay at all
		std::string l_inputs;	//I think these should be deleted, use Connector objects instead
		std::string l_outputs;
		std::string outType;	//specifically for the comparator module
		int dataWidth;
		bool sign;
		float delay;			//the delay of this logic item

};

//Logic mutator, will set recieved "type" into a integer class variable. Allowing use of switch statements
void Logic::SetType(std::string inputType) { 
	if (!inputType.compare("REG")) { this->type = 1; }
	else if (!inputType.compare("ADD")) { this->type = 2; }
	else if (!inputType.compare("SUB")) { this->type = 3; }
	else if (!inputType.compare("MUL")) { this->type = 4; }
	else if (!inputType.compare("COMP")) { this->type = 5; }
	else if (!inputType.compare("MUX")) { this->type = 6; }
	else if (!inputType.compare("SHR")) { this->type = 7; }
	else if (!inputType.compare("SHL")) { this->type = 8; }
	else if (!inputType.compare("DIV")) { this->type = 9; }
	else if (!inputType.compare("MOD")) { this->type = 10; }
	else if (!inputType.compare("INC")) { this->type = 11; }
	else if (!inputType.compare("DEC")) { this->type = 12; }
	else { this->type = 0; }

	return;
}

//Get string "type" of variable, this converts the integer value to a name for outputting
std::string Logic::GetTypeString() {
	if (this->type == 1) { return "REG"; }
	else if (this->type == 2) { return "REG"; }
	else if (this->type == 3) { return "SUB"; }
	else if (this->type == 4) { return "MUL"; }
	else if (this->type == 5) { return "COMP"; }
	else if (this->type == 6) { return "MUX"; }
	else if (this->type == 7) { return "SHR"; }
	else if (this->type == 8) { return "SHL"; }
	else if (this->type == 9) { return "DIV"; }
	else if (this->type == 10) { return "MOD"; }
	else if (this->type == 11) { return "INC"; }
	else if (this->type == 12) { return "DEC"; }
	else { return NULL; }
}

void Logic::SetDataWidth(int inputDataWidth) {
	if (inputDataWidth==1) { this->dataWidth = 1; }
	else if (inputDataWidth == 2) { this->dataWidth = 2; }
	else if (inputDataWidth == 4) { this->dataWidth = 3; }
	else if (inputDataWidth == 8) { this->dataWidth = 4; }
	else if (inputDataWidth == 16) { this->dataWidth = 5; }
	else if (inputDataWidth == 32) { this->dataWidth = 6; }
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
	else { return 0; }

	return;
}

void Logic::SetDelay() {

		//					0	1-bit	2-bit	4-bit	8-bit	16-bit	32-bit
	float regArray[7] = {	0,	2.616,	2.644,	2.879,	3.061,	3.602,	3.966 };	//REG
	float addArray[7] = {	0,	2.704,	3.713,	4.924,	5.638,	7.270,	9.566 };	//Add
	float subArray[7] = {	0,	3.024,	3.412,	4.890,	5.569,	7.253,	9.566 };	//SUB
	float mulArray[7] = {	0,	2.438,	3.651,	7.453,	7.811,	12.395,	15.354 };	//MUL
	float compArray[7] = {	0,	3.031,	3.934,	5.949,	6.256,	7.264,	8.416 };	//COMP
	float muxArray[7] = {	0,	4.083,	4.115,	4.815,	5.623,	8.079,	8.766 };	//MUX2x1
	float shrArray[7] = {	0,	3.644,	4.007,	5.178,	6.460,	8.819,	11.095 };	//SHR
	float shlArray[7] = {	0,	3.614,	3.980,	5.152,	6.549,	8.565,	11.220 };	//SHL
	float divArray[7] = {	0,	0.619,	2.144,	15.439, 33.093,	86.312,	243.233 };	//DIV
	float modArray[7] = {	0,	0.758,	2.149,	16.078, 35.563,	88.142,	250.583 };	//MOD
	float incArray[7] = {	0,	1.792,	2.218,	3.111,	3.471,	4.347,	6.200 };	//INC
	float decArray[7] = {	0,	1.792,	2.218,	3.108,	3.701,	4.685,	6.503 };	//DEC

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


#endif

