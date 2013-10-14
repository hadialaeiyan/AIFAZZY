#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
using namespace std;

#include "State.h"
#include "ENV.h"
#include "instruction.h"
#include "Words\StringHash.cpp"
#include "Logger.h"

enum comperation_kind
{
	equal,notequal,biger,bigerandequal,smaller,smallerandequal,notfound,
	ifodd,ifeven,LOOP,LOOPE,LOOPNE,ifsign,ifnotsign,ifoverflow,ifnotoverflow,
	ifcxisziro,ifecxisziro
};

class OPcodeSimulatetion
{

public:
	OPcodeSimulatetion(void);
	~OPcodeSimulatetion(void);
	
	bool Cmp_OR_test(State* state);
	void DOsimulation(State* state);

private:

	void loadOPcodes();
	void loadJUMPSKINDOPcodes();
	Range GetArg(string s);
	void setvariable(string s,Range* R);
	void Test_Env_changes(bool test_result, string arg1, string arg2,comperation_kind CKIND);
	bool is64_BitMode();

	comperation_kind getcomperation_kind_();
	void call();
	void ret();
	void push();
	void mov();
	void _test();
	void ADD();
	void SUB();
	void CMP();
	void POP();

	void AAA();
	void AAD();

	State* state;
	instruction* ini;
	ENV* env;
	HDICTIONARY<int> opcodes_list;
	HDICTIONARY<comperation_kind> JumpsKind;
};

