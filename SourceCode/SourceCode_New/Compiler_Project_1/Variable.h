#pragma once

#include <vector>
#include <string>
using namespace std;

#include "Range.h"



class Variable
{
public:
	Variable(void);
	~Variable(void);
	Variable(string name);
	Variable(string name[],int indexnumber);
	void freeRanges();
	Variable* Copy();
	string StringForGV();
	void CopyRange(Variable* var2);

	bool findname(string s);
	Range* getvalue(int size,int number);
	void setvalue(int size,int number,Range value);

	vector<string> variable_name;
	vector<Range*> Ranges;
};

