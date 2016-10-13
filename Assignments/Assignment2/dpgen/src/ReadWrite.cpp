/*************************************
Names: Kevin Curtis, Benjamin Schifman
NetId: kevincurtis, bschifman
Assignment #: 2
File: ReadWrite.cpp
*************************************/

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "ReadWrite.h"
#include "Logic.h"
#include "Connector.h"
using namespace std;




bool ReadWrite::parseFile(string filename) {


	bool CorrectFormat = false;
	string log;
	Logic *logicPtr = NULL;
	vector<Logic*> array;	//list of logic items
	fstream inFile;     // Input file stream

	inFile.open(filename.c_str());//opens file

	if (!inFile.is_open()) { //check if it does not build
		cout << "Could not open text file." << endl;
		inFile.close();//close the input file
		return false;
	}

	else if (inFile.is_open()) {//if file is opens

		while (!inFile.eof()) {// then while the file does not end keep going through the loop

			getline(inFile, log);// grabs the entire line of the input file

			while (log.find("input") != string::npos) {// checks to see if the input is the word, will actually have to be rearraged, this is just a skeleton

				//create inputs, ie connectors, here

				getline(inFile, log);// grabs the entire line of the input file

				if (log.size()<3) {//if there is too little data in the line, ie an endline, stop adding stuffs
					break;	//or something, sleeping pill is kicking in, cant think anymore
				}
			}

			//find outputs

			//find logic



			}

		}
		if (CorrectFormat == false) {
			inFile.close();//close the input file
			return false;
		}

	inFile.close();//close the input file

	return true;
}