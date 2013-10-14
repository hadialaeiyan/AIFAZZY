#include "Variable.h"


Variable::Variable(void)
{
}


Variable::~Variable(void)
{
	freeRanges();
}


Variable::Variable(string name)
{
	this->variable_name.push_back(name);
}

Variable::Variable(string name[],int indexnumber){
	for(int i=0;i<indexnumber;i++)
	this->variable_name.push_back(name[i]);
}

void Variable:: freeRanges(){
	for(unsigned int i=0;i<Ranges.size();i++)
		delete Ranges[i];
	Ranges.clear();
}

Variable* Variable:: Copy(){
	Variable* var = new Variable();
	var->variable_name =this->variable_name;
	for(unsigned int i =0 ;i<Ranges.size();i++)
	{
		var->Ranges.push_back(Ranges.at(i)->copy());
	}
	return var;
}

void Variable:: CopyRange(Variable* var2){
	for(unsigned int i =0 ;i<Ranges.size();i++)
	{
		var2->Ranges.push_back(Ranges.at(i)->copy());
	}
}

string printvectorstring(vector<string> v){
	string s="";
	for(int i=0;i<v.size();i++)
		s+=v[i]+", ";
	return s;
}

string Variable::StringForGV(){
	string s = printvectorstring(variable_name);
	for(int i=0;i<this->Ranges.size();i++)
		if(i == this->Ranges.size()-1){
			s.insert(s.length(),Ranges[i]->tostring());
		}else{
			s.insert(s.length(),Ranges[i]->tostring());
			s.insert(s.length(),",");
		}
	return s;
}

bool Variable::findname(string s){
		for(int i=0;i<variable_name.size();i++)
			if(s.compare(variable_name[i])==0)
				return true;
		return false;
}

Range* Variable::getvalue(int size,int number){
	
	union A_Register {
		long long _64bit;
		int _32bit[2];
		short _16bit[4];
		char _8bit[8];
	}Rexa,Rexb;

	double max = Ranges[0]->get__Max_value(),min = Ranges[0]->get__Min_value();
	Rexa._64bit = min;
	Rexb._64bit = max;
	double a,b;
	if(size == 64){
		a = Rexa._64bit;
		b = Rexb._64bit;
	}else if(size == 32){
	  	a = Rexa._32bit[number];
		b = Rexb._32bit[number];
	}else if(size==16){
		a = Rexa._16bit[number];
		b = Rexb._16bit[number];
	}else if(size==8){
		a = Rexa._8bit[number];
		b = Rexb._8bit[number];
	}
	Range* r = new Range(a,b);
	return r;
}

void Variable::setvalue(int size,int number,Range value){
	
	union A_Register {
		long long _64bit;
		int _32bit[2];
		short _16bit[4];
		char _8bit[8];
	}Rexa,Rexb,vala,valb;

	double max = Ranges[0]->get__Max_value(),min = Ranges[0]->get__Min_value();
	Rexa._64bit = min;
	Rexb._64bit = max;
	double a = value.get__Min_value(),b=value.get__Max_value();
	vala._64bit = (long long)a;
	valb._64bit = (long long)b;
	if(size == 64){
		Rexa._64bit = vala._64bit;
		Rexb._64bit = valb._64bit;
	}else if(size == 32){
	  	Rexa._32bit[number] = vala._32bit[number];
		Rexb._32bit[number] = valb._32bit[number];
	}else if(size==16){
		Rexa._16bit[number] = vala._16bit[number];
		Rexb._16bit[number] = vala._16bit[number];
	}else if(size==8){
		Rexa._8bit[number] = vala._32bit[number];
		Rexb._8bit[number] = vala._32bit[number];
	}
}