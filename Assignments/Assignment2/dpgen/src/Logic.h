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

class Logic
{
	public:
		void SetType(std::string inputType);
		std::string GetType();
		void SetName(std::string inputName);
		std::string GetName();


		void SetOutType(std::string inputOutType);
		std::string GetOutType();
		void Setdw(int inputdw);
		int Getdw();
		void SetSign(bool inputsign);
		bool GetSign();
		void SetDelay(float inputDelay);
		float GetDelay();

	private:
		std::string type;
		std::string name;
		std::string l_inputs;
		std::string l_outputs;
		std::string outType;	//specifically for the comparator module
		int dw;
		bool sign;
		float delay;




};

// Logic:: member function assignment of "type"
void Logic::SetType(std::string inputType) {
	type = inputType;  // type refers to data member
	return;
}
std::string Logic::GetType() { return type; } // type refers to data member



// Logic:: member function assignment of "Name"
void Logic::SetName(std::string inputName) {
	name = inputName;  // name refers to data member
	return;
}
std::string Logic::GetName() { return name; } // name refers to data member



/*  l_inputs functions*/

/*  l_outputs functions*/


// Logic:: member function assignment of "outType"
void Logic::SetOutType(std::string inputOutType) {
	outType = inputOutType;  // outType refers to data member
	return;
}
// Logic:: member function return of "outType"
std::string Logic::GetOutType() { return outType; } // outType refers to data member

													
// Logic:: member function assignment of "dw"
void Logic::Setdw(int inputdw) {
	dw = inputdw;  // dw refers to data member
	return;
}
// Logic:: member function return of "dw"
int Logic::Getdw() { return dw; } // dw refers to data member


// Logic:: member function assignment of "sign"
void Logic::SetSign(bool inputsign) {
	sign = inputsign;
	return;
}
bool Logic::GetSign() { return sign; }


// Logic:: member function assignment of "delay"
void Logic::SetDelay(float inputDelay) {
	delay = inputDelay;
	return;
}
float Logic::GetDelay() { return delay; }

#endif

