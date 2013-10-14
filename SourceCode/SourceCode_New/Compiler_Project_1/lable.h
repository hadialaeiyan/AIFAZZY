#pragma once

#include <string>
using namespace std;

#include "block.h"

class lable
{
public:
		// class constructor
		lable();
		// class destructor
		lable(string _name,block* _block,int _blockini);
		~lable();
		string name;
		block* blockL;
		int blockini;
};
