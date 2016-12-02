/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: StateCase.cpp
*************************************/

#include <iostream>
#include <vector>
#include <string>

#include "Logic.h"
#include "StateCase.h"

using namespace std;


StateCase::StateCase(void) {

}

StateCase::StateCase(int caseNumber, int latencyLevel) {	//overload constructor
	this->caseNumber = caseNumber;
	this->latencyLevel = latencyLevel;
	this->trueFalseCase = true;
	this->ifElseDepthMax = 0;
}

StateCase::~StateCase(void) {

}

void StateCase::RemoveDuplicateNodes() {	//remove node duclicates in a case statement
	unsigned int i = 0;
	unsigned int j = 0;

	for (i = 0; i < this->GetCaseNodes().size(); i++) {
		for (j = i + 1; j < this->GetCaseNodes().size(); j++) {
			if (this->GetCaseNodes().at(i) == this->GetCaseNodes().at(j)) {
				this->GetCaseNodes().erase(this->GetCaseNodes().begin() + j);
				j--;
			}
		}
	}

}


bool StateCase::CheckDuplicateCase(StateCase* caseToCheck) {											//check if case is a duclicate of another case statement
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int counterCheck = 0;
	if (this->caseNodes.size() != caseToCheck->caseNodes.size()) {			//not the same nodes, not a duplicate
		return false;
	}
	if (this->caseChildren.at(0) != caseToCheck->caseChildren.at(0)) {		//not the same child cases, not a duplicate
		return false;
	}

	for (i = 0; i < this->caseNodes.size(); i++) {
		for (j = 0; j < caseToCheck->caseNodes.size(); j++) {
			if (this->caseNodes.at(i) == caseToCheck->caseNodes.at(j)) {	//is the same node, count it
				counterCheck++;
				break;
			}
		}
	}
	if (counterCheck == this->caseNodes.size()) {							//this case shares the same nodes and child case, it is a duplicate case state
		return true;
	}
	else {
		return false;														//not the same nodes, not a duplicate
	}

}

