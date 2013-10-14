#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <stdint.h>
using namespace std;
#include "codepair.h"
#include "Register.h"
#include "stringCalculator.h"
#include "Valriables.h"

enum type{ none,real,floatingpoint};

class instruction
{
public:
	instruction();
	instruction(string ins,int line,vector<codepair*>* codesindex,vector<Valriables*>* _variabletabel,vector<Register*>* _registertabel,int* _templatevariablenamenumber,int* _templatepointervariablenumber,vector<string>* _stack);
	~instruction();
	void print_inis();
	string to_write_inis();
	string to_write_iniss();
	bool isBranch(string* branches,int len);
	bool islableform();
	string getlablename();
	string gotowhere(string* branches,int len,int* returnindexbranches);
	bool isa_Valid_instruction();
	void calculateDefandUseID();
	void loadinstruction(string ins);
	string deletespace(string s);
	bool isDef(string s);
	bool isUse(string s);
	void deleteComments();
	string deleteComments(string ss);
	void deletesamenameinVector(vector<string> X);
	void chenge_label_with(string newLabel);
	instruction* Copy();
	string getinstructin(){return ini;}
	void addstringtoendofini(string s);
	///////////////////////////////////////////////////////////////////////////////variables
	string opcode;
	vector<string> D/*Def in ini*/,inivar , U;


	long StringHexTointConverter(string hex);
	short StringHexToshortConverter(string hex);
	std::string int_to_hex( int i );
	string getnexttemplatevariablenamenumber();
	string getnexttemplatepointervariablenumber();
	string setDEFUSE(bool use,bool def,string arg);
	string getString(vector<string> s);
	vector<string> split_string(string s,char c);
	vector<string> split_string_with_E(string s);
	string split_first_string(string s,char c);
	int getindex();
	bool isID(string s);
	bool isregister(string s);
	bool isptr(string s);
	Register* getRegister(string name);
	Register* getRegister_affter_me(string name);
	Valriables* getVariable(string name);
	Valriables* instruction::getVariable_affter_me(string name);
	string GetLocationToJump(string jumpto);
	void GetaCopyFromEnviromnents();
	bool isHexnumber(string s);
	///////////////////////////////////////////////////////////////////////////////variables
private :

	bool valid; /* instruction  is valid or no */
	string ini;
	string code;
    int linenumber; /*store the line number of instrucion in the file*/
	vector<codepair*>* codesindex;
	vector<Valriables*>* variabletabel_affter_me;
	vector<Register*>* registertabel_affter_me;
	vector<Valriables*>* variabletabel;
	vector<Register*>* registertabel;
	vector<string>* stack;
	vector<string>* stack_affter_me;
	int* templatevariablenamenumber,*templatepointervariablenumber;
};
