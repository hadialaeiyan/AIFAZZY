#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "instruction.h"
using namespace std;

class block
{
public:
	block();
	block(string Blockname);
	~block();
	
	void add_new_instruction(instruction* ins);
	void print_ins_out(); // print all instraction into the cmd. 
	string write_to_ins_out(); // get a string to print to a file.
	instruction* init(int index); // get the intraction at index index.
	instruction* first_init_pop(); // get the first instrunction in this block.
	instruction* init_pop_at(int index); //...
	int size(); // get the number of insruction in this block
	bool iniisemply();
	void add_new_OUT(block* b,string text);
	void add_new_IN(block* b);
	string getname();
	void DELETERELATIONIN();
	void deleteout(string blockname);
	string getlablename();
	void clone_pop_OUT(vector<block*>* A);
	vector<block*>* getOUT();
	vector<string>* getOUTTEXT();
	vector<block*>* getIN();
	void setsample();
	bool issample();
	string StringForGV();
	void clearout();
	void clearin();
	void clearoutelse(string elsestring);
	void Copyinsinto(block* nb);
	void Copyoutsinto(block* nb);
	void Copyoutsintoelse(block* nb,string elsestring);
	void change_continue_OUT_to(string text);
	block* getOUTContinue();
	void remove_IN(string Bname);
	void remove_OUT(string Bname);
	bool jumpisfor(string s);
	bool samejumpisfor(string s);
	block* getOUTfor(string call);
	vector<block*> getOUTsfor(string call);
	void Delete_IT_return_IN();
	bool isnewerthan(block * b);
	void change_label_with(string newLabel);
	int is_def_a_ptr_with_address(string address); // return -1 if not def and return >=0 if def
	block* Copy();
	block* Copy_with_ini_Copy();
	vector<block*>* succ(){return this->getOUT();}
	vector<block*>* pred(){return this->getIN();}
	vector<block*>* succ_t();
	vector<block*>* succ_f();
	string getOUTTEXT(block* b);
	//////////////////////////////////////////////////////////variables
	int blocktype; //0 = none, 1 = entryblock, 2 =Assignments, 3 = Tests, 4 = Junctions, 5 = exit.

private :
	bool notfound(vector<block*> bs,block* b);
					/*for search in _IN and _OUT.
					the block want to add, if last time added 
					no allow to add again.*/

	bool issamplevalue;
					/* when a block make is false 
					after make a block that block not found in code 
					it set to true
					*/
	string Bname; //save block name
	vector<instruction*> insbuf; // save instractions of this block
	vector<block*> _IN; // save all coming blocks to this block
	vector<block*> _OUT; // save all out going blocks from this block
	vector<string> _OUTTEXT;
	
};