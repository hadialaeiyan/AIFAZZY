#include "ENV.h"

ENV::ENV(void)
{
}

ENV::~ENV(void)
{
	freevars();
	freeregisters();

}

Variable* ENV::getregister(string s){
	for(unsigned int i=0;i<regisers.size();i++)
		if(regisers.at(i)->findname(s))
			return regisers[i];
	return NULL;
}
Variable* ENV::getvariable(string name){
	for(unsigned int i=0;i<vars.size();i++)
	if(vars.at(i)->findname(name))
		return vars[i];
	return NULL;
}

Variable* ENV::getregister(int index){
	return regisers[index];
}

Variable* ENV::getvariable(int index){
	return vars[index];
}

int ENV::getregisterindex(string s){
	for(unsigned int i=0;i<regisers.size();i++)
		if(regisers.at(i)->findname(s))
			return i;
	return -1;
}

int ENV::getvariableindex(string s){
	for(unsigned int i=0;i<vars.size();i++)
		if(vars.at(i)->findname(s))
			return i;
	return -1;
}

void ENV::deletefromvar(int index){
	vars.erase(vars.begin()+index);
}

void ENV::deletefromregister(int index){
	regisers.erase(regisers.begin()+index);
}

void ENV::addtoregister(Variable* v){
	if(getregister(v->variable_name.at(0))==NULL)
		regisers.push_back(v);
}

void ENV::addtovars(Variable* v){
	vars.push_back(v);
}

void ENV::deletefromvar(string name){
	int index = getvariableindex(name);
	if(index>=0)
		deletefromvar(index);
}

void ENV::deletefromregister(string name){
	int index = getregisterindex(name);
	if(index>=0)
		deletefromregister(index);
}

bool ENV::isregister(string name){
	for(unsigned int i=0;i<regisers.size();i++)
		if(regisers.at(i)->findname(name))
			return true;
	return false;
}

bool ENV::isvariable(string name){
	for(unsigned int i=0;i<vars.size();i++)
		if(vars.at(i)->findname(name))
			return true;
	return false;
}

vector<Variable*> ENV::CopyRegisters(){
	vector<Variable*>* vars = new vector<Variable*>();
	for(unsigned int i=0;i<regisers.size();i++)
		vars->push_back(regisers[i]->Copy());
	return *vars;
}

vector<Variable*> ENV::Copyvars(){
	vector<Variable*>* varss = new vector<Variable*>();
	for(unsigned int i=0;i<vars.size();i++)
		varss->push_back(vars[i]->Copy());
	return *varss;
}

void ENV::freevars(){
	for(unsigned int i=0;i<vars.size();i++)
			delete vars[i];
		vars.clear();
}

void ENV::freeregisters(){
	for(unsigned int i=0;i<regisers.size();i++)
			delete regisers[i];
		regisers.clear();
}

ENV* ENV::Copy(){
	ENV* env = new ENV();
	env->regisers = CopyRegisters();
	env->vars = Copyvars();
	env->stack = copystack();
	return env;
}

void ENV::UNION(ENV* e){
	if(e!=NULL){
		if(e->stack.size()>this->stack.size())
		{
			this->freestack();
			this->copystack(&(e->stack));
		}
		for(int i=0;i<this->regisers.size();i++)
		{
			Variable* reg = e->getregister(regisers.at(i)->variable_name[0]);
			if(reg!=NULL){
				Range* RR = regisers.at(i)->Ranges[0]->operatorUNION(*reg->Ranges[0]);
				regisers.at(i)->freeRanges();
				regisers.at(i)->Ranges.push_back(RR);
			}
		}
		for(int i=0;i<e->regisers.size();i++)
		{
			Variable* reg = getregister(e->regisers.at(i)->variable_name[0]);
			if(reg==NULL){
				addtoregister(e->regisers[i]->Copy());
			}
		}
		for(int i=0;i<this->vars.size();i++)
		{
			Variable* var = e->getvariable(vars.at(i)->variable_name[0]);
			if(var!=NULL){
				Range* RR = vars.at(i)->Ranges[0]->operatorUNION(*var->Ranges[0]);
				vars.at(i)->freeRanges();
				vars.at(i)->Ranges.push_back(RR);
			}
		}
		for(int i=0;i<e->vars.size();i++)
		{
			Variable* var = getvariable(e->vars.at(i)->variable_name[0]);
			if(var==NULL){
				addtovars(e->vars[i]->Copy());
			}
		}
	}
}

void ENV::operatorUP(ENV* e){
	//this->freestack();
	//this->copystack(&(e->stack));
	/*به ازای تتک تک رجیستر ها عملیات بالا را انجام می دهیم.*/
	for(int i=0;i<this->regisers.size();i++) 
	{
		Variable* reg = e->getregister(regisers.at(i)->variable_name[0]);
		if(reg!=NULL &&regisers.at(i)->Ranges[0]->UPERBOUNDISINF()){
			Range* RR =regisers.at(i)->Ranges[0]->operatorUP(*reg->Ranges[0]);
			regisers.at(i)->freeRanges();
			regisers.at(i)->Ranges.push_back(RR);
		}
	}
	for(int i=0;i<this->vars.size();i++)
	{
		Variable* var = e->getvariable(vars.at(i)->variable_name[0]);
		if(var!=NULL&& vars.at(i)->Ranges[0]->UPERBOUNDISINF()){
			Range* RR = vars.at(i)->Ranges[0]->operatorUP(*var->Ranges[0]);
			vars.at(i)->freeRanges();
			vars.at(i)->Ranges.push_back(RR);
		}
	}
}

void ENV::operatorDOWN(ENV* e){
	//this->freestack();
	//this->copystack(&(e->stack));
	/*به ازای تتک تک رجیستر ها عملیات پایین را انجام می دهیم.*/
	for(int i=0;i<this->regisers.size();i++) 
	{
		Variable* reg = e->getregister(regisers.at(i)->variable_name[0]);
		if(reg!=NULL){
			Range* RR =regisers.at(i)->Ranges[0]->operatorDOWN(*reg->Ranges[0]);
			regisers.at(i)->freeRanges();
			regisers.at(i)->Ranges.push_back(RR);
		}
	}
	for(int i=0;i<this->vars.size();i++)
	{
		Variable* var = e->getvariable(vars.at(i)->variable_name[0]);
		if(var!=NULL){
			Range* RR = vars.at(i)->Ranges[0]->operatorDOWN(*var->Ranges[0]);
			vars.at(i)->freeRanges();
			vars.at(i)->Ranges.push_back(RR);
		}
	}
}

void ENV::freestack(){
	stack.clear();
}

void ENV::PUSHtostack(string value){
	this->stack.push_back(value);
}

string ENV::POPfromstack(){
	if(stack.size()>0){
		string s = this->stack.at(stack.size()-1);
		stack.erase(stack.begin()+stack.size()-1);
		return s;
	}
	return "NULL";
}

string ENV::topofstack(){
	return stack.at(0);
}

vector<string> ENV::copystack(){
	vector<string>* st = new vector<string>();
	for(unsigned int i=0;i<this->stack.size();i++)
		st->push_back(stack[i]);
	return *st;
}

void ENV::copystack(vector<string>* st){
	for(unsigned int i=0;i<st->size();i++)
		this->stack.push_back(st->at(i));
}

string ENV::INFOforGVfile(){
	string s = "#V =  #R =  #S = ";
	char c[18];
	_itoa(this->stack.size(),c,10);
	s.insert(17,c);
	_itoa(this->regisers.size(),c,10);
	s.insert(11,c);
	_itoa(this->vars.size(),c,10);
	s.insert(5,c);
	if(stack.size()>0){
		s.insert(s.length(),"\\nstack:");
		for(int i=0;i<stack.size();i++)
		{
			s.insert(s.length(),stack.at(i));
			if(i!=stack.size()-1)
				s.insert(s.length(),"|");
		}
	}
	if(this->regisers.size()>0){
		s.insert(s.length(),"\\nregisters:");
		for(int i=0;i<regisers.size();i++)
		{
			s.insert(s.length(),"\\n   ");
			s.insert(s.length(),regisers.at(i)->StringForGV());
		}
	}
	if(this->vars.size()>0){
		s.insert(s.length(),"\\nstack:");
		for(int i=0;i<vars.size();i++)
		{
			s.insert(s.length(),"\\n   ");
			s.insert(s.length(),vars.at(i)->StringForGV());
		}
	}
	return s;
}