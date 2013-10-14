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

#include "codepair.h";
#include "block.h";
#include "Range_Analysis.h"
#include "Words\StringHash.cpp"

#define GVoutfilename "Graph\\bin\\AICFG.gv"
#define CONTINUE "continue"

enum brancheskind
	{
		sharty,ghyresharty,ret,call,notfind,
	};

class Abstract_interpretation
{
public:
	Abstract_interpretation(void);
	Abstract_interpretation(vector<block*>* _basicBlocks,vector<codepair*>* _opcodes);
	~Abstract_interpretation(void);
	void setBasicBlocks(vector<block*>* _basicBlocks);
	void setOpcodes(vector<codepair*>* _opcodes);
	int start_Abstract_interpretation();
	vector<block*>* getACopyofBasicBlocks();
	vector<block*>* foreachinstructionmakeabasicblock(vector<block*>* bb);
	void make_pdf(vector<block*>* b);
	void make_pdf(State* statetree);
	void MakeGVfile(vector<block*> inss);
	void MakeGVfile(State* statetree);
	string tracestatesforMakeGVfile(State* statetree,HDICTIONARY<State*>* fortrace,State* parrent);
	void setbranchkind();
	void Range_Analysis_(vector<block*>* for_each_ini);
	brancheskind getbranchkind(int index);

private:
	//vector<Node_b*>* getarcs(vector<block*>* for_each_ini);
	string* branches;
	int bnum; // this set the number of branches String * [?];
	vector<block*>* basicBlocks; /*همه بلاک های اولیه را در خود جای می دهد.*/
	vector<codepair*>* opcodes; /*همه آپکد های زبان اسنبلی را در خود ذخیره دارد*/
	int PDFNUMBER;
};

