#pragma once

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
using namespace std;

#include "block.h"
#include "instruction.h"
#include "lable.h"

#define filename  "ACM.txt"
#define outfilename "CFGoutput.txt"


class CFGD
{
public:
	CFGD(void);
	~CFGD(void);

	void DOCFG(char* in);
	void DOCFG();
	void print_block_out(vector<block*> inss);
	void Do_Blocking(vector<block*>* inss);
	void setbranchkind();
	void Delete_Not_Use_Block(vector<block*>* bbuf);
	block* get_block_lable(string _name,vector<block*>* inss);
	void write_to_file(vector<block*> inss);
	void sep_block(block* b,int startindex,block* tonb);
	

	//////////////////////////////////////////////////////////////////////////variables
	vector<block*> bbuf;	//buffer for save the blocks.
	vector<lable*> labels; //buffer for labels. 
	string* branches; /// store the defult jump syntax that have in asm code
	int Bnamecounter;  // this is a conter for create a BB1 ... BBn
	int bnum; // this set the number of branches String * [?];
	bool wtofile /* is set true store the result in the file */ ,stocons  /* is set print the result in the CMD*/; 
	

private:
	string maketruelable(string s);
	string getnextbnameoutsilde();
	string getnextbname();
	string fileforout; // save file path and name for write the result to file.

	void makefilenameforout(){
		char myPath[_MAX_PATH+1];
		GetModuleFileName(NULL,myPath,_MAX_PATH);
		fileforout.insert(0,myPath);
		int locationfind = fileforout.find_last_of('\\');
		if(locationfind >=0)
		{
			fileforout = fileforout.substr(0,locationfind+1);
		}
		fileforout.insert(fileforout.length(),outfilename);
	}

	
};

