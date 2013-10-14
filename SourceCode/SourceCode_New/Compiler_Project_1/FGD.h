#pragma once

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <vector>
#include <windows.h>
#include <stdio.h>
#include <iomanip>
#include <algorithm>
#include <ostream>
#include <sstream>
#include <iostream>
#include <ios>
using namespace std;

#include "block.h"
#include "instruction.h"
#include "lable.h"
#include "codepair.h"
#include "Register.h"
#include "Valriables.h"
#include "IMDLLS.h"
#include "Abstract_interpretation.h"


#define filename  "ACM.txt"
#define outfilename "CFGoutput.txt"
#define GVoutfilename "Graph\\bin\\CFG.gv"
#define call_return "return"
#define con_after_call "con after call"
#define continue_code "continue code"
#define true_and_false "true and false"
#define jumping "jumping"
#define call_farakhony "call"
#define continue_cl "continue"
#define True_condition "true"
#define False_condition "false"


class CFGD
{
public:
	CFGD(void);
	~CFGD(void);

	void DOCFG(const char*,bool,string exefilename);
	void DOCFG();
	void print_block_out(vector<block*> inss);
	void makefilenameforout();
	void Do_Blocking(vector<block*>* inss);
	void setbranchkind();
	void Delete_Not_Use_Block(vector<block*>* bbuf);
	block* get_block_lable(string _name,vector<block*>* inss,bool docontinue);
	void write_to_file(vector<block*> inss);
	bool sep_block(block* b,int startindex,block* tonb);
	void clear();
	void getallbb(char*);
	vector<string> get_next_address();
	void make_jpg();
	void make_call_flow_graph_pdf();
	//////////////////////////////////////////////////////////////////////////variables
	vector<block*> bbuf;	//buffer for save the blocks.
	vector<lable*> labels; //buffer for labels. 
	string* branches; /// store the defult jump syntax that have in asm code
	int Bnamecounter;  // this is a conter for create a BB1 ... BBn
	int bnum; // this set the number of branches String * [?];
	bool wtofile /* is set true store the result in the file */ ,stocons  /* is set print the result in the CMD*/; 
	void make_pdf();

private:
	vector<block*>* TraceForFindReturn(block* blocklabel);/* این تابع در فراخوانی ها نیاز می شود. وقتی سر یک تابع را پیدا کرد ته آن را پیگیری می کند.*/
	void makeGraphinPDF();
	vector<block*> getindirectjumpblocks(vector<block*> bbuf);
	bool isID(string s);
	void merge2vector(vector<block*>* v1,vector<block*>* v2);
	void makefilenameforgvout();
	vector<string> split_string(string s,char c); // for spliting a character.
	void loadOPCodes();
	void MakeGVfile(vector<block*> inss);
	string maketruelable(string s);
	string getnextbnameoutsilde();
	string getnextbname();
	void findIndirectjumsPlace( vector<block*> indirectjumps);
	bool ispointertomemory(string label);
	vector<string> split_string_with_E(string s);
	vector<string> Get_in_prt_block_valuse(string s);
	string find_value_of_ptr(string indirto,block* B);
	string value_of_memory(string address,block* B);
	string FindlastDefptr(string address,block* B);
	vector<block*> get_pointer_definition(string address,block* B);
	bool checked_this_block_in_Get_pointer_definition(block* B);
	bool Find_this_label_in_last_labels(string inslabel);
	void LoadIATCALLS();
	long StringHexTointConverter(string hex);
	string isinIAT(string hex);
	void setcurrent_env_path();
	string delete_space_in_first_of_string(string s);
	void load_INfirst_registers();
	std::string int_to_hex( long i );
	long toint(string s);
	/////////////////////////////////////////////////////////////////////////////variables
	string fileforout; // save file path and name for write the result to file.
	string fileforgvout;
	vector<codepair*>* codes; /* the instrunctions save in it*/
	int pdfnumbercfg; // for show pdfs use it umber
	bool branchesset/* if branches set it set true*/,codesset/* if codes set it set to true */;
	vector<Valriables*> variabletabel;
	vector<Register*> registertabel;
	int templatevariablenamenumber,templatepointervariablenumber;
	//bool endjobinfirstreturn;
	vector<block*> bbchecked;
	string exefilename; // exe file path
	vector<IMDLLS*> imdlls; // import tables;
	vector<IMDLLS*> dlimdlls; // delay load import tables;
	string current_env_path; // this exe file;
	vector<string>* stack;
};

