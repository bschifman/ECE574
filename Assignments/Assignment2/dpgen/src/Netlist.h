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

	bool parseFile(string filename);						//parse an input file specified upon execution
	bool parseEdge(string inputLine);						//Parse string of variables as edges	(ie 'input int64 a, b, c')
	bool parseNode(string inputLine);						//parse string of logic into a node		(ie 'a = b + c'
	bool outputModule(string outputFilename);				//writes all the parsed data from the input file into a verilog module 
	string CreateInputName(Logic* CurrentNode, Connector * tempParent);		//converts a module input name into a padded input as needed
	string CreateCOMPInputName(Connector * tempParent0, Connector * tempParent1);	//converts a module input name into a padded input as needed for compartors
	string CreateShiftName(Logic* CurrentNode, Connector * tempParent);		//converts a module input name into a padded input as needed for shift modules
	void findCriticalPath(void);
	void findCriticalPathTemp(vector<Logic*> tempList);
	string outputEdgeLine(string type, unsigned int datawidth);
	string outputNodeLine(int nodeNumber);
	void outputToReg();
	Connector* findEdge(string edgeName);

	//Getters
	vector<Connector*> GetEdges() { return this->edges; }
	vector<Logic*> GetNodes() { return this->nodes; }
	float GetCriticalPath() { return this->criticalPath; }

	//Setters
	void SetCriticalPath(float criticalPath) { this->criticalPath =+ criticalPath; }


private:
	vector<Connector*> edges;
	vector<Logic*> nodes;
	float criticalPath;
};

#endif

