/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: Logic.h
*************************************/

#include <iostream>
#include <vector>
#include <string>

using namespace std;

#ifndef LOGIC_H
#define LOGIC

class Connector;		//forward declaration

class Logic				//nodes, will probably need their inputs as well to create the verilog code
{
public:
	Logic(void);//default constructor
	Logic(string type, Connector* logicOutput, int dataWidth) ;		//overload constructor
	~Logic(void);													    //destructor

	//Setters
	void SetType(string inputType);
	void SetName(string name) { this->name = name; }
	void SetConnector(Connector *recievedConnector) { this->logicOutput = recievedConnector; }
	void SetL_inputs(string l_inputs) { this->l_inputs = l_inputs; }
	void SetOutType(string outType) { this->outType = outType; }
	void SetL_outputs(string l_inputs) { this->l_outputs = l_outputs; }
	void SetDataWidth(int inputDataWidth);
	void SetSign(bool sign) { this->sign = sign; }
	void SetDelay(); 

	//Getters
	string GetName() { return this->name; }
	int GetType() { return this->type; }
	string GetTypeString();
	Connector* GetConnector() { return this->logicOutput; }
	string Getl_inputs() { return this->l_inputs; }
	string Getl_outputs() { return this->l_outputs; }
	string GetOutType() { return this->outType; }
	int GetDataWidth();
	float GetDelay() { return this->delay; }
	bool GetSign() { return this->sign; }
		
private:
	int type;
	string name;
//		vector<Connector*> logicInputs;	//inputs for Logic object, might not need since graph is directional, would just know since this Logic would be on the edge list of connections
	Connector* logicOutput;	//Outputs for logic object(ie comparator might have gt, ln, eq)	//could get away without making a vector if you just made a comparator for each logic line, wouldn't change delay at all
	string l_inputs;		//I think these should be deleted, use Connector objects instead
	string l_outputs;
	string outType;			//specifically for the comparator module
	int dataWidth;
	bool sign;
	float delay;			//the delay of this logic item

};


#endif

