#pragma once

#include <iostream>
#include <string>
using namespace std;

class Register
{
public:

	string registername;
	string value;
	int rtype;
	string refregister;
	string refmomory;
	string refpointer;

	void clear(){
		value = "";
		rtype = 0;
		refregister = "";
		refmomory = "";
		refpointer = "";
	}

	Register(void);
	~Register(void);

	Register* getacopy(){
		Register* x = new Register();
		x->refmomory = this->refmomory;
		x->refpointer = this->refpointer;
		x->registername = this->registername;
		x->refregister = this->refregister;
		x->value = this->value;
		x->rtype = this->rtype;
		return x;
	}

};

