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
	void findCriticalPath(void);											// loops through the nodes to find the critical path
	void findCriticalPathTemp(vector<Logic*> tempList);						//  if there are any elements out of order in the netlist, loops through this secondary netlist to complete the critical path
	string outputEdgeLine(string type, unsigned int datawidth);				// outputs the verilog variables into verilog code format
	string outputNodeLine(int nodeNumber);									// outputs a line for a verilog module (ie ADD, SUB, DIV, etc) in verilog code
	void outputToReg();														//finds any output edges and adds a REG module to them if they dont have one
	Connector* findEdge(string edgeName);									//find the edge object pointer based off a variable name
	bool CalculateASAP();													//calculate all ASAP values for nodes and edges
	bool CheckIfASAPDone();													//absolute check to see if any nodes are unmarked
	bool CalculateALAP();													//calculate all ALAP values for nodes and edges
	bool CheckIfALAPDone();													//absolute check to see if any nodes are unmarked
	bool CalculateProbabilityFDS();											//compute probabilities for force directed scheduling
	bool CalculateForcesFDS();												//compute self/predecessor/successor/total forces for force directed scheduling
	bool CalculateFDS();													//compute force directed scheduling
	void SetLatency(string latency) { this->latency = stoi(latency); }		//set the int latency value for the circuit
	void SetIfElseDepth(int depth) { this->ifElseDepth = depth; }			//set the current int depth value for the circuit
	int GetLatency() { return this->latency; }								//get the int latency value for the circuit
	int GetIfElseDepth() { return this->ifElseDepth; }						//get the current int depth value for the circuit
	int FindNodeNumber(Logic* tempLogic);									//Finds node number in the netlist node vector of an inputted node

	//Getters
	vector<Connector*> GetEdges() { return this->edges; }
	vector<Logic*> GetNodes() { return this->nodes; }
	float GetCriticalPath() { return this->criticalPath; }

	//Setters
	void SetCriticalPath(float criticalPath) { this->criticalPath =+ criticalPath; }	//updates critical path of the entire circuit with a new critical path


private:
	vector<Connector*> edges;
	vector<Logic*> nodes;
	vector<vector<float> > nodeProbabilityArray;
	vector<float> ADDSUBDistribution;
	vector<float> MULDistribution;
	vector<float> LOGRESDistribution;
	vector<float> DIVMODDistribution;
	float criticalPath;
	int latency;
	int ifElseDepth;

};

#endif

