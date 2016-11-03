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
	Logic(string type, Connector* logicOutput, int dataWidth, bool sign);		//overload constructor
	~Logic(void);																//destructor

	//Setters
	void SetType(string inputType);
	void SetName(string name) { this->name = name; }
	void SetConnector(Connector *recievedConnector) { this->logicOutput = recievedConnector; }
	void SetOutType(string outType) { this->outType = outType; }
	void SetDataWidth(int inputDataWidth);
	void SetSign(bool sign) { this->sign = sign; }
	void SetInherentDelay();
	void SetDelay(float totalDelay) { this->delay = totalDelay; }
	void AddParent(Connector* parentToAdd) { this->logicInputs.push_back(parentToAdd); }

	//Getters
	Connector* GetConnector() { return this->logicOutput; }
	vector <Connector*> GetParents() { return this->logicInputs; }
	string GetName() { return this->name; }	
	string GetTypeString();
	string GetOutType() { return this->outType; }
	int GetType() { return this->type; }
	int GetDataWidth();
	float GetDelay() { return this->delay; }	
	bool GetSign() { return this->sign; }
		
private:

	Connector* logicOutput;					//Outputs for logic object(ie comparator might have gt, ln, eq)	//could get away without making a vector if you just made a comparator for each logic line, wouldn't change delay at all
	vector <Connector*> logicInputs;		//edge inputs for the logic object
	string typeName;
	string l_inputs;						//I think these should be deleted, use Connector objects instead
	string name;
	string l_outputs;
	string outType;							//specifically for the comparator module
	int type;
	int dataWidth;
	bool sign;
	float delay;							//the delay of this logic item
			
};


#endif
