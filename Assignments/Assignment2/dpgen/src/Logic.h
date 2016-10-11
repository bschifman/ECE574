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
		
		void SetType(std::string inputType) { this->type = type; }
		std::string GetType() { return this->type; }
		void SetName(std::string inputName) { this->name = inputName; }
		std::string GetName() { return this->name; }
		void Setl_inputs(std::string inputl_inputs) { this->outType = inputl_inputs; }
		std::string Getl_inputs() { return this->l_inputs; }
		void Setl_outputs(std::string inputl_outputs) { this->outType = inputl_outputs; }
		std::string Getl_outputs() { return this->l_outputs; }
		void SetOutType(std::string inputOutType) { this->outType = inputOutType; }
		std::string GetOutType() { return this->outType; }
		void Setdw(int inputdw) { this->dw = inputdw; }
		int Getdw() { return this->dw; }
		void SetSign(bool inputsign) { this->sign = inputsign; }
		bool GetSign() { return this->sign; }
		void SetDelay(float inputDelay) { this->delay; }
		float GetDelay() { return this->delay; }

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

