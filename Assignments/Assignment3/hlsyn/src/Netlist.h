/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: Netlist.h
*************************************/
#ifndef NETLIST_H
#define NETLIST_H

#include <vector>
#include "Connector.h"
#include "Logic.h"
#include "StateCase.h"

using namespace std;

class Netlist
{
public:
	Netlist(void);
	~Netlist(void);

	bool parseFile(string filename, string latency);						//parse an input file specified upon execution
	bool parseEdge(string inputLine);						//Parse string of variables as edges	(ie 'input int64 a, b, c')
	bool parseNode(string inputLine);						//parse string of logic into a node		(ie 'a = b + c'
	bool outputModule(string outputFilename);				//writes all the parsed data from the input file into a verilog module 
	string CreateInputName(Logic* CurrentNode, Connector * tempParent);		//converts a module input name into a padded input as needed
	string CreateCOMPInputName(Connector * tempParent0, Connector * tempParent1);	//converts a module input name into a padded input as needed for compartors
	string CreateShiftName(Logic* CurrentNode, Connector * tempParent);		//converts a module input name into a padded input as needed for shift modules
	string outputEdgeLine(string type, unsigned int datawidth);				// outputs the verilog variables into verilog code format
	string outputNodeLine(int nodeNumber);									// outputs a line for a verilog module (ie ADD, SUB, DIV, etc) in verilog code
	Connector* findEdge(string edgeName);									//find the edge object pointer based off a variable name
	bool CalculateASAP();													//calculate all ASAP values for nodes and edges
	bool CheckIfASAPDone();													//absolute check to see if any nodes are unmarked
	bool CalculateALAP();													//calculate all ALAP values for nodes and edges
	bool CheckIfALAPDone();													//absolute check to see if any nodes are unmarked
	bool CalculateProbabilityFDS();											//compute probabilities for force directed scheduling
	bool CalculateForcesFDS();												//compute self/predecessor/successor/total forces for force directed scheduling
	bool CalculateFDS();													//compute force directed scheduling
	int FindNodeNumber(Logic* tempLogic);									//Finds node number in the netlist node vector of an inputted node
	bool outputHLSMModule(string outputFilename);							//writes all the parsed data from the input file into a verilog HLSM module 
	bool RecalculateASAP(Logic* inputNode, int minTime);													//Lots of crap, and for the function below too
	bool RecalculateALAP(Logic* inputNode, int minTime);													//^
	string outputCaseLine(Logic* caseNode);									// outputs a line for a verilog case module in verilog code
	bool CalculateCaseStates();												//compute the case state orders and links
//	bool CheckLatencyLevelComplete(int latencyLevel);						//check if everything in a latency level has been accounted for //this got deleted in the merge
//	int CheckLatencyLevelDepth(int latencyLevel);							//check if latency level has an if/else/for depth anywhere		//this got deleted in the merge
	bool RemoveAllDuplicateCases();											//iterates back to front through all cases and checks for duplicates, removing any if found
	bool RemoveAllEmptyCases();											//iterates back to front through all cases and checks for duplicates, removing any if found
	void UpdateCaseNodesAtLatency(StateCase* caseToUpdate, int currentLatency);	//adds all nodes at 0 depth and other parrellel braches of if statements


	//Getters
	vector<Connector*> GetEdges() { return this->edges; }
	vector<Logic*> GetNodes() { return this->nodes; }
	int GetLatency() { return this->latency; }								//get the int latency value for the circuit
	int GetIfElseDepth() { return this->ifElseDepth; }						//get the current int depth value for the circuit
	int GetIfForIncrementer() { return this->ifForIncrementer; }			//get the current incremented # of if/for variables in the circuit

	//Setters
	void SetLatency(string stringLatency);									//set the int latency value for the circuit. F*** stoi
	void SetIfElseDepth(int depth) { this->ifElseDepth = depth; }			//set the current int depth value for the circuit
	void SetIfForIncrementer(int newIncrement) { this->ifForIncrementer = newIncrement; }			//set the current incremented # of if/for variables in the circuit
	void SetIfForLevelOneOrZero(int level, bool boolValue) { this->ifForLevelOneOrZero[level] = boolValue; }			//set the current incremented # of if/for variables in the circuit


private:
	vector<Connector*> edges;
	vector<Logic*> nodes;
	vector<StateCase*> cases;
	vector< vector<float> > nodeProbabilityArray;
	vector<float> ADDSUBDistribution;
	vector<float> MULDistribution;
	vector<float> LOGRESDistribution;
	vector<float> DIVMODDistribution;
	vector<bool> ifForLevelOneOrZero;
	int latency;
	unsigned int ifElseDepth;
	unsigned int ifForIncrementer;

};

#endif

