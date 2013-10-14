#pragma once

#include <vector>
using namespace std;

#include "Variable.h"

class Variable1{
public:
	

};

class ENV
{
public:

	ENV(void);
	~ENV(void);
	Variable* getregister(string s);
	Variable* getvariable(string name);
	Variable* getregister(int index);
	Variable* getvariable(int index);
	int getregisterindex(string s);
	int getvariableindex(string s);
	void deletefromvar(int index);
	void deletefromregister(int index);
	void addtoregister(Variable* v);
	void addtovars(Variable* v);
	void deletefromvar(string name);
	void deletefromregister(string name);
	bool isregister(string name);
	bool isvariable(string name);
	vector<Variable*> CopyRegisters();
	vector<Variable*> Copyvars();
	void freevars();
	void freeregisters();
	void freestack();
	void ENV::PUSHtostack(string value);
	string ENV::POPfromstack();
	string topofstack();
	vector<string> copystack();
	ENV* Copy();
	void UNION(ENV* e);
	void operatorDOWN(ENV* e);
	void copystack(vector<string>* st);
	string INFOforGVfile();
	void operatorUP(ENV* e);

private:
	vector<Variable*> vars;
	vector<string> stack;
	vector<Variable*> regisers;
};

