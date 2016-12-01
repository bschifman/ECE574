#pragma once
/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: StateCase.h
*************************************/
#ifndef STATECASE_H
#define STATECASE_H

#include <iostream>
#include <vector>
#include <string>
//#include "Logic.h"

using namespace std;



class Connector;		//forward declaration
class Logic;		//forward declaration

class StateCase				//nodes, will probably need their inputs as well to create the verilog code
{
public:
	StateCase(void);																//default constructor
	StateCase(int caseNumber, int latencyLevel);														//overload constructor
	~StateCase(void);																//destructor


	void RemoveDuplicateNodes();																//remove node duclicates in a case statement
	bool CheckDuplicateCase(StateCase* caseToCheck);											//check if case is a duclicate of another case statement

	void AddParentCase(StateCase* parentToAdd) { this->caseParents.push_back(parentToAdd); }	//add a parent case to this case statement (ie an input to the module)
	void AddChildrenCase(StateCase* childToAdd) { this->caseChildren.push_back(childToAdd); }	//add a child case to this case statement (ie an output to the module)
	void AddNodeToCase(Logic* nodeToAdd) { this->caseNodes.push_back(nodeToAdd); }				//add a node to this case statement (ie an 'b = a + c' to the module)
	void SetCaseNumber(int newCaseNumber) { this->caseNumber = newCaseNumber; }
	void SetLatencyLevel(int latencyLevel) { this->latencyLevel = latencyLevel; }
	void SetTrueFalseCase(bool trueOrFalseCase) { this->trueFalseCase = trueOrFalseCase; }
	void SetIfElseDepthMax(int depthLevel) { this->ifElseDepthMax = depthLevel; }

	vector <StateCase*> GetParentCases() { return this->caseParents; }							//get parent case list to this case statement (ie an input to the module)
	vector <StateCase*> GetChildCases() { return this->caseChildren; }							//get children case list to this case statement (ie an output to the module)
	vector <Logic*> GetCaseNodes() { return this->caseNodes; }									//get the nodes of this case statement
	int GetCaseNumber() { return this->caseNumber; }											//get the current case number
	int GetLatencyLevel() { return this->latencyLevel; }										//get the case latency level
	bool GetTrueFalseCase() { return this->trueFalseCase; }										//
	int GetIfElseDepthMax() { return this->ifElseDepthMax; }


private:
	vector <StateCase*> caseParents;													//case inputs for the case statement
	vector <StateCase*> caseChildren;													//case Ouputs for the case statement
	vector <Logic*> caseNodes;															//children nodes within this case statement
	int caseNumber;																		//case number ordering scheme
	int latencyLevel;																	//case latecy level
	int ifElseDepthMax;
	bool trueFalseCase;																	//

};


#endif

