#pragma once
#include <iostream>
#include <string>
using namespace std;

class Valriables
{
public:

	bool ispointer;
	string variablename;
	string value;
	string offset;
	int vtype;
	string refregister;
	string pointerreferto;

	Valriables(void);
	~Valriables(void);

	Valriables* getacopy(){
		Valriables* x = new Valriables();
		x->ispointer = this->ispointer;
		x->offset = this->offset;
		x->pointerreferto = this->pointerreferto;
		x->refregister = this->refregister;
		x->value = this->value;
		x->variablename = this->variablename;
		x->vtype = this->vtype;
		return x;
	}
};

