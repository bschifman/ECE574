/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: Logic.h
*************************************/
#ifndef LOGIC_H
#define LOGIC_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;



class Connector;		//forward declaration

class Logic				//nodes, will probably need their inputs as well to create the verilog code
{
public:
	Logic(void);																//default constructor
	Logic(string type, Connector* logicOutput, int dataWidth, bool sign, int depth);		//overload constructor
	~Logic(void);																//destructor

	//Setters
	void SetType(string inputType);												//set the type of node(logic element) this is (ie ADD, REG, DIV...)
	void SetName(string name) { this->name = name; }							//set the name of this node (ie ADD_0, ADD_4, etc)
	void SetConnector(Connector *recievedConnector) { this->logicOutput = recievedConnector; }	//set output(child) edge of this node(ie the output wire)
	void SetOutType(string outType) { this->outType = outType; }				//set the node output type, specifically for the COMP (ie >,<,==)
	void SetDataWidth(int inputDataWidth);										//set the datawidth of this node (ie 1,2,4,8,16,32,64, etc)
	void SetSign(bool sign) { this->sign = sign; }								//set whether this is a signed(1) or unsigned(0) module based on its output type
	void AddParent(Connector* parentToAdd) { this->logicInputs.push_back(parentToAdd); }	//add a parent edge to this node (ie an input to the module)
	void SetNodeALAP(int inputTime) { this->alapt = inputTime; }					//set ALAP time
	void SetNodeASAP(int inputTime) { this->asapt = inputTime; }					//set ASAP time
	void SetNodeFDS(int inputTime) { this->fdst = inputTime; }						//set FDS time
	void SetIfElseDepth(int depth) { this->ifElseDepth = depth; }					//set if else circuit depth
	void SetIfLevelOneOrZero(bool boolValue) { this->ifLevelOneOrZero = boolValue; }	//set the current if/else bool value of the circuit
	void SetEasyInputs();																//set that easyInputs string

	//Getters
	Connector* GetConnector() { return this->logicOutput; }						//get output(child) edge of this node(ie the output wire)
	vector <Connector*> GetParents() { return this->logicInputs; }				//get parent edge list to this node (ie an input to the module)
	vector<Logic*> GetParentNodes();											//get the parent nodes of a node
	string GetName() { return this->name; }										//get the name of this node (ie ADD_0, ADD_4, etc)
	string GetTypeString();														//get the type of node(logic element) this is (ie ADD, REG, DIV...)
	string GetOutType() { return this->outType; }								//get the node output type, specifically for the COMP (ie >,<,==)
	int GetType() { return this->type; }										//get the type of node in integer format(ie 1=REG,2=ADD...)
	int GetDataWidth();															//get the datawidth of this node (ie 1,2,4,8,16,32,64, etc)
	bool GetSign() { return this->sign; }										//get whether this is a signed(1) or unsigned(0) module based on its output type
	int GetNodeALAP() { return this->alapt; }									//get ALAP time
	int GetNodeASAP() { return this->asapt; }									//get ASAP time
	int GetNodeFDS() { return this->fdst; }										//get FDS time
	int GetTypeScheduleDelay() { return this->scheduleDelayValue; }				//get schedule value for this node type
	int GetIfElseDepth() { return this->ifElseDepth; }							//get if else circuit depth this node type
	bool GetIfLevelOneOrZero() { return this->ifLevelOneOrZero; }				//get the current if/else bool value of the circuit
		
private:

	Connector* logicOutput;					//Outputs for logic object(ie comparator might have gt, ln, eq)	//could get away without making a vector if you just made a comparator for each logic line, wouldn't change delay at all
	vector <Connector*> logicInputs;		//edge inputs for the logic object
	string typeName;
	string name;
	string outType;							//specifically for the comparator module
	string easyInputs;						//specifically for ease of reading debugging
	int asapt;								//sets the ASAP time
	int alapt;								//sets the ALAP time
	int fdst;								//sets the FDS time
	int scheduleDelayValue;
	int type;
	int dataWidth;
	int ifElseDepth;						//depth of circuit
	bool sign;
	bool ifLevelOneOrZero;					//records if the node is part of the if(true) component or the else(false) component
			
};


#endif

