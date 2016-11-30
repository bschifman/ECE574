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
