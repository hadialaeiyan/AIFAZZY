//baraye negahdari va zakhire labelha.
#include "lable.h"


lable::lable(void)
{
}


lable::~lable(void)
{
	delete blockL;
}

lable:: lable(string _name,block* _block,int _blockini){
	name = _name;
	blockL = _block;
	blockini = _blockini;
}
