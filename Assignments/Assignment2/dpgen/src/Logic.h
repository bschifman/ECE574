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


/*This object is the various*/
class Logic
{
public:
		//Setters
		void SetType(std::string type) { this->type = type; }
		void SetName(std::string name) { this->name = name; }
		void SetL_inputs(std::string l_inputs) { this->l_inputs = l_inputs; }
		void SetOutType(std::string outType) { this->outType = outType; }
		void SetL_outputs(std::string l_inputs) { this->l_outputs = l_outputs; }
		void SetDw(int dw) { this->dw = dw; }
		void SetSign(bool sign) { this->sign = sign; }
		void SetDelay(float delay) { this->delay = delay; }
		//Getters
		std::string GetName() { return this->name; }
		std::string GetType() { return this->type; }
		std::string Getl_inputs() { return this->l_inputs; }
		std::string Getl_outputs() { return this->l_outputs; }
		std::string GetOutType() { return this->outType; }
		int Getdw() { return this->dw; }
		float GetDelay() { return this->delay; }
		bool GetSign() { return this->sign; }
		
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

#endif

