#pragma once
#include<iostream>
#include<vector>
#include<string.h>
using namespace std;

class IMDLLS
{
public:
	IMDLLS(string s);
	IMDLLS();
	~IMDLLS(void);
	void loadfromstring(string s);


	string dllname;
	string dllIAT;
	vector<string> functions;
	vector<string> offsets;
};

