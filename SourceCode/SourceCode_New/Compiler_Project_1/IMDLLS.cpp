#include "IMDLLS.h"


IMDLLS::IMDLLS(string s)
{
	loadfromstring(s);
}

IMDLLS::IMDLLS()
{
}

IMDLLS::~IMDLLS(void)
{
	this->functions.clear();
	this->offsets.clear();
}

void IMDLLS::loadfromstring(string str){
	string s = str;
	int index = s.find('|');
	this->dllname = s.substr(0,index);
	s=s.substr(index+1);
	index = s.find('|');
	this->dllIAT = s.substr(0,index);
	s=s.substr(index+1);
	string functioname,itoffset;
	while (true)
	{
		index = s.find('|');
		if(index == string::npos)
			break;
		functioname =  s.substr(0,index);
		s=s.substr(index+1);
		index = s.find('|');
		itoffset = s.substr(0,index);
		s=s.substr(index+1);
		if(itoffset.compare("")!=0 && functioname.compare("")!=0)
		{
			functions.push_back(functioname);
			offsets.push_back(itoffset);
		}
	}
}