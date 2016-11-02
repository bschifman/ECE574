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

	bool parseFile(string filename);
	bool parseEdge(string inputLine);	//Parse string
	bool parseNode(string inputLine);
	bool outputModule(string outputFilename);
	string CreateInputName(Logic* CurrentNode, Connector * tempParent);
	string CreateCOMPInputName(Connector * tempParent0, Connector * tempParent1);
	string CreateShiftName(Logic* CurrentNode, Connector * tempParent);
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

