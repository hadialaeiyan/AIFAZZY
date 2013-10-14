#pragma once

#include <string>
#include <windows.h>
using namespace std;

#include "ENV.h"
#include "logger.h"
#include "OPcodeSimulatetion.h"
#include "block.h"
#include "instruction.h"
#include "State.h"
#include "Words\StringHash.cpp"


class Range_Analysis
{

public:
	Range_Analysis(void);
	~Range_Analysis(void);
	void setBasicBlocks(vector<block*>* for_each_ini);
	vector<block*>* getBasicBlocks();
	int DOANALYSIS();

	State* stateTree;

private:

	State* make_states();
	State* make_states(block* cur,HDICTIONARY<State*>* bbnamesaver);
	block* findbasicBlockStart();
	void loadFLAGSANDREGISTERS(ENV* env);
	void ____________ANALIYSIS(State* Statetree_root);
	void ____________ANALIYSIS_UPOperation(State* Statetree_root,vector<string>* bbnamesaver);
	void ____________Merge_Environment_Junctions(State* State);
	bool ____________ANALIYSIS_CONDITION_(State* state);
	void ____________ANALIYSIS_until_state(State* P,State* c);
	vector<State*>* Get_ALL_Parent_OF_State(State* Statetree_root,State* _State,vector<string>* bbnamesaver);
	vector<State*>* Get_ALL_Parent_OF_State(State* Statetree_root,State* _State);
	void merge2vector(vector<State*>* v1,vector<State*>* v2);
	void ____________Merge_Environment_Junctions__UPOPERATION(State* _State);

	vector<block*>* for_each_ini;
	OPcodeSimulatetion OPcsm;
};

