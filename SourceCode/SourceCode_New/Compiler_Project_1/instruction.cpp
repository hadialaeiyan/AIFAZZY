
#include "instruction.h"
#include <iomanip>
#include <algorithm>
#include <ostream>
#include <sstream>
#include <iostream>
#include <ios>
using namespace std;

instruction::instruction(){

}
instruction::instruction(string ins, int line,vector<codepair*>* _codesindex,vector<Valriables*>* _variabletabel,vector<Register*>* _registertabel,int* _templatevariablenamenumber,int* _templatepointervariablenumber,vector<string>* _stack)
{
	valid = true;
	variabletabel =_variabletabel;
	registertabel=_registertabel;
	templatevariablenamenumber=_templatevariablenamenumber;
	templatepointervariablenumber=_templatepointervariablenumber;
	codesindex = _codesindex;
    linenumber = line;
	variabletabel_affter_me = new vector<Valriables*>();
	registertabel_affter_me = new vector<Register*>();
	stack = _stack;
	stack_affter_me = new vector<string>();
    loadinstruction(ins);
}

instruction::~instruction(void)
{
	D.clear();
	inivar.clear();
	U.clear();
	for(int i=0;i<codesindex->size();i++)
		delete (codesindex->at(i));
	codesindex->clear();
	for(int i=0;this->variabletabel_affter_me->size();i++)
		delete (variabletabel_affter_me->at(i));
	variabletabel_affter_me->clear();
	for(int i=0;this->registertabel_affter_me->size();i++)
		delete (registertabel_affter_me->at(i));
	registertabel_affter_me->clear();
	stack_affter_me->clear();
}

void instruction::addstringtoendofini(string s){
	this->ini.insert(this->ini.length(),s);
}

void instruction::GetaCopyFromEnviromnents(){
	for(int i=0;i<this->variabletabel->size();i++)
		variabletabel_affter_me->push_back(variabletabel->at(i)->getacopy());
	for(int i=0;i<this->registertabel->size();i++)
		registertabel_affter_me->push_back(registertabel->at(i)->getacopy());
	for(int i=0;i<this->stack->size();i++)
		stack_affter_me->push_back(stack->at(i));
}

void instruction::chenge_label_with(string newLabel){
	string p2 = ini.substr(ini.find(":"));
	p2.insert(0,newLabel);
	ini = p2;
}

vector<string>  instruction:: split_string_with_E(string s){
	vector<string> data;
	unsigned int lastindex = 0;
	unsigned int i =0 ;
	for(; i<s.length();i++)
		if(s[i]=='+'||s[i]=='-'||s[i]=='*'||s[i]=='/')
		{
			data.push_back(s.substr(lastindex,i-lastindex));
			lastindex = i+1;
		}
	data.push_back(s.substr(lastindex,i-lastindex));
	return data;
}

string instruction::getnexttemplatevariablenamenumber(){
	string ret("T");
	char c[10];
	_itoa_s(*templatevariablenamenumber,c,sizeof(c),10);
	ret.insert(1,c);
	(*templatevariablenamenumber)++;
	return ret;
}

string instruction::getnexttemplatepointervariablenumber(){
	string ret("PT");
	char c[10];
	_itoa_s(*templatepointervariablenumber,c,sizeof(c),10);
	ret.insert(2,c);
	(*templatepointervariablenumber)++;
	return ret;
}

vector<string> instruction:: split_string(string s,char c){
	vector<string> data;
	unsigned int lastindex = 0;
	unsigned int i =0 ;
	for(; i<s.length();i++)
		if(s[i]==c)
		{
			data.push_back(s.substr(lastindex,i-lastindex));
			lastindex = i+1;
		}

	data.push_back(s.substr(lastindex,i-lastindex));
	return data;
}

string instruction:: split_first_string(string s,char c){
	for(unsigned int i=0; i<s.length();i++)
		if(s[i]==c)
		{
			return s.substr(0,i);
		}
		return "nofound";
}

void instruction::loadinstruction(string ins){
	ini = ins;
	deleteComments();
	int location = ini.find("opcode:");
	if(location != string::npos)
	{
		code = ini.substr(location+7);
		ini = ini.substr(0,location);
	}
	location = ini.find(":");
	if(location != string::npos){
		if(location!=ini.length()-1)
			opcode = deletespace(split_first_string(ini.substr(location+2),' '));
	}else{
		if(ini.length()!=0)
			opcode = deletespace(split_first_string(ini,' '));
	}
    calculateDefandUseID();
    deletesamenameinVector(inivar);
    deletesamenameinVector(D);
    deletesamenameinVector(U);
	this->GetaCopyFromEnviromnents();
}

bool instruction::isa_Valid_instruction(){
	return valid;
}

void instruction::print_inis(){
	cout<<linenumber<<"\t"<<ini<<"\n";
}

bool instruction::isBranch(string* branches,int len)
{
	for(int i=0;i<len;i++)
	{
		int location = ini.find(branches[i]);
		if(location != string::npos)
		{
			return true;
		}
	}
	return false;
}

bool instruction::islableform(){
	int location = ini.find(":");
	if(location != string::npos)
		return true;
	return false;
}

string instruction::getlablename(){
	int location = ini.find(":");
	if(location != string::npos)
	{
		int i=0;
		while((ini[i]==' '||ini[i]=='\t')&&(i<=location)) i++;
		if(i==location)
			return "";
		return ini.substr(i,location);
	}
	return "";
}

string instruction::gotowhere(string* branches,int len,int* returnindexbranches){
	for(int i=0;i<len;i++)
	{
		int location = ini.find(branches[i]);
		if(location>=0){
			*returnindexbranches = i;
			if(10==i)
				return "";
			if(location != string::npos)
			{
				string temp = ini.substr(opcode.length()+location);
				int i=0;
				while(temp[i]!=0 && (temp[i]==' ' || temp[i]=='\t' )) i++;
				return GetLocationToJump(temp.substr(i));
			}
		}
	}
	*returnindexbranches = -1;
	return "";
}

long instruction::StringHexTointConverter(string hex) {
	if(hex[hex.length()-1]=='h'||hex[hex.length()-1]=='H')
		hex = hex.substr(0,hex.length()-1);
    char * p;
    long n = strtol( hex.c_str(), & p, 16 );
    if ( * p != 0 ) { //my bad edit was here
        cout << "not a number" << endl;
    }
	return n;
}

short instruction::StringHexToshortConverter(string hex) {
	if(hex[hex.length()-1]=='h'||hex[hex.length()-1]=='H')
		hex = hex.substr(0,hex.length()-1);
    char * p;
	short n = strtol( hex.c_str(), & p, 16 );
    if ( * p != 0 ) { //my bad edit was here
        cout << "not a number" << endl;
    }
	return n;
}

std::string instruction::int_to_hex( int i )
{
  stringstream s;
  s << std::setfill ('0') << std::setw(sizeof(int)*2) 
         << std::hex << i;
  string str = s.str();
  transform(str.begin(), str.end(),str.begin(), ::toupper);
  return str;
}
string instruction::deleteComments(string ss){
	string s = ss;
	int index = s.find(';');
    if (index > 0) {
        s = s.substr(0, index);
    } else if (index == 0) {
        valid = false;
    }
	return s;
}
string instruction::GetLocationToJump(string jumpto){
	jumpto = deletespace(deleteComments(jumpto));
	int index = 0;
	if((index = jumpto.find("far")) != string::npos){
		int thislabeloffset = (StringHexTointConverter(this->getlablename()));
		index+=3;
		string s = jumpto.substr(index);
		string sorg ;
		int j = 0;
		for(int i=0;i<s.length();i++)
		{	if(s[i]!=' ')
			{
				sorg.insert(sorg.end(),s[i]);
				if(s[i]=='h')
					break;
				j++;
			}
		}
		short shortvalueis = StringHexTointConverter(sorg);
		return int_to_hex(thislabeloffset + shortvalueis);
	}else if ((index =  jumpto.find("near")) != string::npos){
		int thislabeloffset = (StringHexTointConverter(this->getlablename()));
		index+=4;
		string s = jumpto.substr(index);
		string sorg ;
		int j = 0;
		for(int i=0;i<s.length();i++)
		{	if(s[i]!=' ')
			{
				sorg.insert(sorg.end(),s[i]);
				if(s[i]=='h')
					break;
				
				j++;
			}
		}
		short shortvalueis = StringHexTointConverter(sorg);
		return int_to_hex(thislabeloffset + shortvalueis);
	}else if((index = jumpto.find("short")) != string::npos){
		int thislabeloffset = (StringHexTointConverter(this->getlablename()));
		index+=5;
		string s = jumpto.substr(index);
		string sorg ;
		int j = 0;
		for(int i=0;i<s.length();i++)
		{	if(s[i]!=' ')
			{
				sorg.insert(sorg.end(),s[i]);
				if(s[i]=='h')
					break;
				j++;
			}
		}
		short shortvalueis = StringHexToshortConverter(sorg);
		return int_to_hex(thislabeloffset + shortvalueis);
	}
	if(isregister(jumpto))
	{
		Register* r = getRegister_affter_me(jumpto);
		if(r!=NULL){
			string ret = r->value;
			if(ret.compare("")!=0){
				if(ret[ret.length()-1]=='h'||ret[ret.length()-1]=='H')
					ret = ret.substr(0,ret.length()-1);
				return ret;
			}
		}
		return jumpto;
	}
	else if(isID(jumpto)){
		return jumpto;
	}
	else if(isptr(jumpto)){
		int l1 = jumpto.find('['),l2=jumpto.find(']');
		if(l1!= string::npos && l2!= string::npos){
			vector<string> E = split_string_with_E(jumpto.substr(l1+1,l2-l1-1));
			for(int i=0;i<E.size();i++)
				if(!isID(E.at(i))){
					if(isregister(E.at(i))){
						Register* r = getRegister_affter_me(E.at(i));
						if(r!=NULL){
							string res = r->value;
							if(res.compare("NULL")!=0&&res.compare("")!=0)
								E.at(i) = res;
						}
					}
				}
		}
	}
	return jumpto;
}

string instruction::to_write_iniss(){
	char* C = new char[10];
	int size = 10;
	_itoa_s(linenumber,C,size,10);
	string out = C;
	out.insert(out.length(),"\t");
	out.insert(out.length(),ini);
	out.insert(out.length(),"\t\tDef : [");
	out.insert(out.length(),getString(D));
	out.insert(out.length(),"] used : [");
	out.insert(out.length(),getString(U));
	out.insert(out.length(),"]");
	delete[] C;
	return out;
}

string instruction::to_write_inis(){
	char* C = new char[10];
	_itoa_s(linenumber,C,10,10);
	string out = C;
	out.insert(out.length(),"\t");
	out.insert(out.length(),ini);
	out.insert(out.length(),"\t\tDef : [");
	out.insert(out.length(),getString(D));
	out.insert(out.length(),"] used : [");
	out.insert(out.length(),getString(U));
	out.insert(out.length(),"]\r\n");
	delete[] C;
	return out;
}

string instruction::getString(vector<string> s){
	string ret = "";
	for(unsigned int i=0;i<s.size();i++)
	{
		ret.insert(ret.length(),s[i]);
		ret.insert(ret.length(),", ");
	}
	return ret;
}

Register* instruction::getRegister(string name){
	for(int i=0;i<registertabel->size();i++)
		if(registertabel->at(i)->registername.compare(name)==0)
			return registertabel->at(i);
	return NULL;
}

Register* instruction::getRegister_affter_me(string name){
	for(int i=0;i<registertabel_affter_me->size();i++)
		if(registertabel_affter_me->at(i)->registername.compare(name)==0)
			return registertabel_affter_me->at(i);
	return NULL;
}

Valriables* instruction::getVariable(string name){
	for(int i=0;i<variabletabel->size();i++)
		if(variabletabel->at(i)->variablename.compare(name)==0||variabletabel->at(i)->offset.compare(name)==0)
			return variabletabel->at(i);
	return NULL;
}

Valriables* instruction::getVariable_affter_me(string name){
	for(int i=0;i<variabletabel_affter_me->size();i++)
		if(variabletabel_affter_me->at(i)->variablename.compare(name)==0||variabletabel->at(i)->offset.compare(name)==0)
			return variabletabel_affter_me->at(i);
	return NULL;
}

string instruction::setDEFUSE(bool use,bool def,string arg){
	arg = deletespace(arg);
	if(isregister(arg)){
		inivar.push_back(arg);
		if(use)
			U.push_back(arg);
		if(def)
		{
			D.push_back(arg);
		}
		Register* reg = getRegister(arg);
			if(reg == NULL)
			{
				reg = new Register();
				reg->registername = arg;
				registertabel->push_back(reg);
			}
		return arg;
	}
	else if (isID(arg))
	{
		return arg;
	}else if(isHexnumber(arg))
	{
		return arg;
	}
	else if(isptr(arg)){
		int l1 = arg.find('['),l2=arg.find(']');
		if(l1!= string::npos && l2!= string::npos){
			stringCalculator* s = new stringCalculator();
			string _E_ = arg.substr(l1+1,l2-l1-1);
			vector<string> E = s->split_string_with_E(_E_);
			string Expr = "";
			bool can_do_calculate = true;
			for(unsigned int i=0;i<E.size();i++)
			{
				if(E[i].compare("+")==0||E[i].compare("-")==0||E[i].compare("*")==0||E[i].compare("/")==0)
				{
					Expr.insert(Expr.length(), E[i]);
				}
				else if(!isHexnumber(E[i]))
				{
					setDEFUSE(true,false,E[i]);
					if(isregister(E[i])){
						Register* r = getRegister(E[i]);
						if(r!=NULL&&r->value.compare("")!=0)
						{
							char c[20];
							_itoa_s(StringHexTointConverter(r->value),c,sizeof(c),10);
							Expr.insert(Expr.length(), c);
						}else can_do_calculate = false;
					}else can_do_calculate = false;
				}else{
					char c[20];
					_itoa_s(StringHexTointConverter(E[i]),c,sizeof(c),10);
					Expr.insert(Expr.length(), c);
				}
			}
			string result;
			if(can_do_calculate){
				result = s->int_to_hex(s->calculate(Expr));
				ini.insert(ini.length(),"; [");
				ini.insert(ini.length(),_E_);
				ini.insert(ini.length(),"=");
				ini.insert(ini.length(),result);
				ini.insert(ini.length(),"] ");
			}
			///use def ptr must be add
			Valriables* var = (can_do_calculate)?getVariable(result):getVariable(arg);
			if(var==NULL)
			{
				var = new Valriables();
				var->ispointer = true;
				var->offset = (can_do_calculate)? result : _E_;
				//var->pointerreferto = "";
				var->value = "";
				var->vtype = none;
				var->variablename = getnexttemplatepointervariablenumber();
				variabletabel->push_back(var);
			}
			delete s;
			return var->variablename;
		}
	}
	return "NULL";
}

bool instruction::isHexnumber(string s){
	char c;
    for (unsigned int i = 0; i < s.length(); i++) {
        c = s[i];
        if (!((c >= 'a' && c <= 'f') ||(c >= 'A' && c <= 'F') || c == 'h'|| c == 'H' || (c >= '0' && c <= '9'))) {
            return false;
        }
    }
	return true;
}

bool instruction::isregister(string s){
	if(
		///x86 registers
		s.compare("eax")==0||s.compare("ebx")==0||s.compare("ecx")==0||s.compare("edx")==0
		||s.compare("ax")==0||s.compare("bx")==0||s.compare("cx")==0||s.compare("dx")==0
		||s.compare("ah")==0||s.compare("bh")==0||s.compare("ch")==0||s.compare("dh")==0
		||s.compare("al")==0||s.compare("bl")==0||s.compare("cl")==0||s.compare("dl")==0
		||s.compare("cs")==0||s.compare("ds")==0||s.compare("es")==0||s.compare("fs")==0||s.compare("gs")==0||s.compare("ss")==0
		||s.compare("edi")==0||s.compare("ebp")==0||s.compare("eip")==0||s.compare("esp")==0||s.compare("esi")==0
		||s.compare("sil")==0||s.compare("dil")==0||s.compare("bpl")==0||s.compare("spl")==0
		||s.compare("si")==0||s.compare("di")==0||s.compare("bp")==0||s.compare("sp")==0
		//flags registers
		||s.compare("cf")==0||s.compare("pf")==0||s.compare("af")==0||s.compare("zf")==0||s.compare("sf")==0
		||s.compare("tf")==0||s.compare("if")==0||s.compare("df")==0||s.compare("of")==0||s.compare("iopl")==0
		||s.compare("nt")==0||s.compare("rf")==0||s.compare("vm")==0||s.compare("ac")==0||s.compare("vif")==0||s.compare("id")==0
		//x64 register
		||s.compare("rax")==0||s.compare("rdx")==0||s.compare("rcx")==0||s.compare("rdx")==0
		||s.compare("rsi")==0||s.compare("rdi")==0||s.compare("rbp")==0||s.compare("rsp")==0
		||s.compare("r8")==0||s.compare("r9")==0||s.compare("r10")==0||s.compare("r11")==0||s.compare("r12")==0||s.compare("r13")==0||s.compare("r14")==0||s.compare("r15")==0
		||s.compare("r8d")==0||s.compare("r9d")==0||s.compare("r10d")==0||s.compare("r11d")==0||s.compare("r12d")==0||s.compare("r13d")==0||s.compare("r14d")==0||s.compare("r15d")==0
		||s.compare("r8w")==0||s.compare("r9w")==0||s.compare("r10w")==0||s.compare("r11w")==0||s.compare("r12w")==0||s.compare("r13w")==0||s.compare("r14w")==0||s.compare("r15w")==0
		||s.compare("r8b")==0||s.compare("r9b")==0||s.compare("r10b")==0||s.compare("r11b")==0||s.compare("r12b")==0||s.compare("r13b")==0||s.compare("r14b")==0||s.compare("r15b")==0
		)
			return true;
		return false;
}

bool instruction::isptr(string s){
	int location = s.find("ptr");
	if(location != string::npos)
	{
		return true;
	}
	return false;
}

instruction* instruction::Copy(){
	instruction* ret = new instruction();
	ret->opcode = this->opcode;
	ret->valid = this->valid;
	ret->ini= this->ini;
	ret->code = this->code;
	ret->linenumber = this->linenumber;
	for(int i=0;i<D.size();i++)
		ret->D.push_back(D[i]);
	for(int i=0;i<U.size();i++)
		ret->U.push_back(U[i]);
	for(int i=0;i<inivar.size();i++)
		ret->inivar.push_back(inivar[i]);
	return ret;
}

void instruction::calculateDefandUseID() {
	string s = ini.substr(ini.find(opcode)+opcode.length());
	vector<string> arg;
	switch (getindex())
	{
	case 1:////////////////////////////////////////////////////////////////////////////////////////////////  /* 2args:first arg use and def and sec arg onle use. 3args: first def and 2,3 use */ used in and,adc,addbtr,bts
		arg = split_string(s,',');
		if (arg.size() == 2) {
			string arg1 = setDEFUSE(true,true,arg[0]);
			string arg2 = setDEFUSE(true,false,arg[1]);
			if(opcode.compare("sub")==0){
				if(arg1.compare("NULL")!=0&&arg2.compare("NULL")!=0){
					if(isregister(arg1)&&isregister(arg2)){
						Register* r1 = getRegister(arg1);
						Register* r2 = getRegister(arg2);
						if(r1!=NULL&& r2!=NULL &&r1->value.compare("")!=0&&r2->value.compare("")!=0)
						{
							int valuer1 = this->StringHexTointConverter(r1->value);
							int valuer2 = this->StringHexTointConverter(r2->value);
							r1->value = this->int_to_hex(valuer1-valuer2);
							ini.insert(ini.length()," ;");
							ini.insert(ini.length(),r1->registername);
							ini.insert(ini.length(),"=");
							ini.insert(ini.length(),r1->value);
						}
					}else if(isregister(arg1)&&isptr(arg2)){
						Register* r1 = getRegister(arg1);
						Valriables* r2 = getVariable(arg2);
						if(r1!=NULL&& r2!=NULL &&r1->value.compare("")!=0&&r2->value.compare("")!=0)
						{
							int valuer1 = this->StringHexTointConverter(r1->value);
							int valuer2 = this->StringHexTointConverter(r2->value);
							r1->value = this->int_to_hex(valuer1-valuer2);
							ini.insert(ini.length()," ;");
							ini.insert(ini.length(),r1->registername);
							ini.insert(ini.length(),"=");
							ini.insert(ini.length(),r1->value);
						}
					}else if(isregister(arg1)){
						Register* r1 = getRegister(arg1);
						if(r1!=NULL &&r1->value.compare("")!=0)
						{
							int valuer1 = this->StringHexTointConverter(r1->value);
							int valuer2 = this->StringHexTointConverter(arg2);
							r1->value = this->int_to_hex(valuer1-valuer2);
							ini.insert(ini.length()," ;");
							ini.insert(ini.length(),r1->registername);
							ini.insert(ini.length(),"=");
							ini.insert(ini.length(),r1->value);
						}
					}else if(isptr(arg1)&&isregister(arg2)){
						Register* r1 = getRegister(arg2);
						Valriables* r2 = getVariable(arg1);
						if(r1!=NULL&& r2!=NULL &&r1->value.compare("")!=0&&r2->value.compare("")!=0)
						{
							int valuer2 = this->StringHexTointConverter(r1->value);
							int valuer1 = this->StringHexTointConverter(r2->value);
							r2->value = this->int_to_hex(valuer1-valuer2);
							ini.insert(ini.length()," ;");
							ini.insert(ini.length(),r2->offset);
							ini.insert(ini.length(),"=");
							ini.insert(ini.length(),r2->value);
						}
					}else if(isptr(arg1)&&isptr(arg2)){
						Valriables* r1 = getVariable(arg1);
						Valriables* r2 = getVariable(arg2);
						if(r1!=NULL&& r2!=NULL &&r1->value.compare("")!=0&&r2->value.compare("")!=0)
						{
							int valuer1 = this->StringHexTointConverter(r1->value);
							int valuer2 = this->StringHexTointConverter(r2->value);
							r1->value = this->int_to_hex(valuer1-valuer2);
							ini.insert(ini.length()," ;");
							ini.insert(ini.length(),r1->offset);
							ini.insert(ini.length(),"=");
							ini.insert(ini.length(),r1->value);
						}
					}else if(isptr(arg1)){
						Valriables* r1 = getVariable(arg1);
						if(r1!=NULL &&r1->value.compare("")!=0)
						{
							int valuer1 = this->StringHexTointConverter(r1->value);
							int valuer2 = this->StringHexTointConverter(arg2);
							r1->value = this->int_to_hex(valuer1-valuer2);
							ini.insert(ini.length()," ;");
							ini.insert(ini.length(),r1->offset);
							ini.insert(ini.length(),"=");
							ini.insert(ini.length(),r1->value);
						}
					}
				}
			}
			else if(opcode.compare("add")==0){
				if(arg1.compare("NULL")!=0&&arg2.compare("NULL")!=0){
					if(isregister(arg1)&&isregister(arg2)){
						Register* r1 = getRegister(arg1);
						Register* r2 = getRegister(arg2);
						if(r1!=NULL&& r2!=NULL &&r1->value.compare("")!=0&&r2->value.compare("")!=0)
						{
							int valuer1 = this->StringHexTointConverter(r1->value);
							int valuer2 = this->StringHexTointConverter(r2->value);
							r1->value = this->int_to_hex(valuer1+valuer2);
							ini.insert(ini.length()," ;");
							ini.insert(ini.length(),r1->registername);
							ini.insert(ini.length(),"=");
							ini.insert(ini.length(),r1->value);
						}
					}else if(isregister(arg1)&&isptr(arg2)){
						Register* r1 = getRegister(arg1);
						Valriables* r2 = getVariable(arg2);
						if(r1!=NULL&& r2!=NULL &&r1->value.compare("")!=0&&r2->value.compare("")!=0)
						{
							int valuer1 = this->StringHexTointConverter(r1->value);
							int valuer2 = this->StringHexTointConverter(r2->value);
							r1->value = this->int_to_hex(valuer1+valuer2);
							ini.insert(ini.length()," ;");
							ini.insert(ini.length(),r1->registername);
							ini.insert(ini.length(),"=");
							ini.insert(ini.length(),r1->value);
						}
					}else if(isregister(arg1)){
						Register* r1 = getRegister(arg1);
						if(r1!=NULL &&r1->value.compare("")!=0)
						{
							int valuer1 = this->StringHexTointConverter(r1->value);
							int valuer2 = this->StringHexTointConverter(arg2);
							r1->value = this->int_to_hex(valuer1+valuer2);
							ini.insert(ini.length()," ;");
							ini.insert(ini.length(),r1->registername);
							ini.insert(ini.length(),"=");
							ini.insert(ini.length(),r1->value);
						}
					}else if(isptr(arg1)&&isregister(arg2)){
						Register* r1 = getRegister(arg2);
						Valriables* r2 = getVariable(arg1);
						if(r1!=NULL&& r2!=NULL &&r1->value.compare("")!=0&&r2->value.compare("")!=0)
						{
							int valuer2 = this->StringHexTointConverter(r1->value);
							int valuer1 = this->StringHexTointConverter(r2->value);
							r2->value = this->int_to_hex(valuer1+valuer2);
							ini.insert(ini.length()," ;");
							ini.insert(ini.length(),r2->offset);
							ini.insert(ini.length(),"=");
							ini.insert(ini.length(),r2->value);
						}
					}else if(isptr(arg1)&&isptr(arg2)){
						Valriables* r1 = getVariable(arg1);
						Valriables* r2 = getVariable(arg2);
						if(r1!=NULL&& r2!=NULL &&r1->value.compare("")!=0&&r2->value.compare("")!=0)
						{
							int valuer1 = this->StringHexTointConverter(r1->value);
							int valuer2 = this->StringHexTointConverter(r2->value);
							r1->value = this->int_to_hex(valuer1+valuer2);
							ini.insert(ini.length()," ;");
							ini.insert(ini.length(),r1->offset);
							ini.insert(ini.length(),"=");
							ini.insert(ini.length(),r1->value);
						}
					}else if(isptr(arg1)){
						Valriables* r1 = getVariable(arg1);
						if(r1!=NULL &&r1->value.compare("")!=0)
						{
							int valuer1 = this->StringHexTointConverter(r1->value);
							int valuer2 = this->StringHexTointConverter(arg2);
							r1->value = this->int_to_hex(valuer1+valuer2);
							ini.insert(ini.length()," ;");
							ini.insert(ini.length(),r1->offset);
							ini.insert(ini.length(),"=");
							ini.insert(ini.length(),r1->value);
						}
					}
				}
			}
		} else if (arg.size() == 3) {
			setDEFUSE(false,true,arg[0]);
			setDEFUSE(true,false,arg[1]);
			setDEFUSE(true,false,arg[2]);
		}
		break;
	case 2://////////////////////////////////////////////////////////////////////////////////////////////// /* used al and def al */  /* AAA --> Changes contents of AL to valid unpacked decimal. The high order nibble is zeroed. ,daa*/
		U.push_back("al");
		D.push_back("al");
		break;
	case 3://////////////////////////////////////////////////////////////////////////////////////////////// /* used al,ah def al,ah */ /* AAD Used before dividing unpacked decimal numbers. Multiplies AH by 10 and the adds result into AL. Sets AH to zero. This instruction is also known to have an undocumented behavior. */
		U.push_back("al");
		D.push_back("al");
		U.push_back("ah");
		D.push_back("ah");
		break;
	case 4://////////////////////////////////////////////////////////////////////////////////////////////// /* used al */ /* aas-> Corrects result of a previous unpacked decimal subtraction in AL. High order nibble is zeroed.*/
		U.push_back("al");
		break;
	case 5://////////////////////////////////////////////////////////////////////////////////////////////// /*used arg 1 and arg 2*/ /* arpl,bound,bsr,bt,btc,cmp*/
		arg = split_string(s,',');
		if (arg.size() == 2) {
			setDEFUSE(true,false,arg[0]);
			setDEFUSE(true,false,arg[1]);
		}
		break;
	case 6://////////////////////////////////////////////////////////////////////////////////////////////// /* have one arg and use and def it */ /* bswap,dec */
		arg = split_string(s,',');
		if (arg.size() == 1)
			setDEFUSE(true,true,arg[0]);
		break;
	case 7://////////////////////////////////////////////////////////////////////////////////////////////// /* have one arg and use it*/  /* call ,int, push*/
		arg = split_string(s,',');
		if (arg.size() == 1)
		{
			string retarg = setDEFUSE(true,false,arg[0]);
			if(opcode.compare("call")==0&&retarg.compare("NULL")!=0){
				if(isregister(retarg)){
					Register* r= getRegister(retarg);
					if(r->value.compare("")!=0)
					{
						ini.insert(ini.length()," ;");
						ini.insert(ini.length(),r->registername);
						ini.insert(ini.length(),"=");
						ini.insert(ini.length(),r->value);
					}
				}
			}
			else if(opcode.compare("push")==0&&retarg.compare("NULL")!=0){
				setDEFUSE(true,true,"esp");
				Register* espr= getRegister("esp");
				if(espr!=NULL&&espr->value.compare("")!=0)
					espr->value = int_to_hex(StringHexTointConverter(espr->value) - 4);
				if(isregister(retarg)){
					Register* r= getRegister(retarg);
					if(r!=NULL)
						stack->push_back(r->value);
				}else if(isptr(retarg)){
					Valriables* v = getVariable(retarg);
					if(v!=NULL)
						stack->push_back(v->value);
				}else {
					stack->push_back(retarg);
				}
			}
		}
		break;
	case 8://////////////////////////////////////////////////////////////////////////////////////////////// /* use al and def al,ax */ /* cbw */
		U.push_back("al");
		D.push_back("al");
		D.push_back("ax");
		break;
	case 9://////////////////////////////////////////////////////////////////////////////////////////////// /*use eax and def edx */ /*cdq*/
		U.push_back("eax");
		D.push_back("adx");
		break;
	case 10://////////////////////////////////////////////////////////////////////////////////////////////// /* use args and def esi,edi */ /*cmps  comapre string*/
		arg = split_string(s,',');
		if (arg.size() == 2) {
			setDEFUSE(true,false,arg[0]);
			setDEFUSE(true,false,arg[1]);
		}
		D.push_back("esi");
		D.push_back("adi");
		break;
	case 11://////////////////////////////////////////////////////////////////////////////////////////////// /* use arg1,arg2, accumulator def grg1 */ /*cmpxchg , cmpxchg8b*/
		arg = split_string(s,',');
		if (arg.size() == 2) {
			setDEFUSE(true,true,arg[0]);
			setDEFUSE(true,false,arg[1]);
		}
		U.push_back("eax");
		break;
	case 12://////////////////////////////////////////////////////////////////////////////////////////////// /*use ax and def ax, dx*/ /*cwd*/
		U.push_back("ax");
		D.push_back("ax");
		D.push_back("dx");
		break;
	case 13://////////////////////////////////////////////////////////////////////////////////////////////// /*use ax and def eax*/ /* cwde*/
		U.push_back("ax");
		D.push_back("eax");
		break;
	case 14://////////////////////////////////////////////////////////////////////////////////////////////// /* have one arg use arg and ax def ax ,dx*/
		arg = split_string(s,',');
		if (arg.size() == 1)
			setDEFUSE(true,false,arg[0]);
		U.push_back("ax");
		D.push_back("ax");
		D.push_back("dx");
		break;
	case 15://////////////////////////////////////////////////////////////////////////////////////////////// /* have one arg use arg and ax def ax ,dx*/ /* imul*/
		arg = split_string(s,',');
		if (arg.size() == 1)
			setDEFUSE(true,false,arg[0]);
		else if(arg.size() == 2){
			setDEFUSE(true,true,arg[0]);
			setDEFUSE(true,false,arg[1]);
		}
		else if(arg.size() == 3){
			setDEFUSE(true,true,arg[0]);
			setDEFUSE(true,false,arg[1]);
			setDEFUSE(true,false,arg[2]);
		}
		U.push_back("ax");
		D.push_back("ax");
		D.push_back("dx");
		break;
	case 16:////////////////////////////////////////////////////////////////////////////////////////////////
		arg = split_string(s,',');
		if (arg.size() == 2) {
			setDEFUSE(true,true,arg[0]);
			setDEFUSE(true,false,arg[1]);
		}
		D.push_back("edi");
		break;
	case 17:////////////////////////////////////////////////////////////////////////////////////////////////
		arg = split_string(s,',');
		if (arg.size() == 1)
			setDEFUSE(true,false,arg[0]);
		U.push_back("cx");
		break;
	case 18:////////////////////////////////////////////////////////////////////////////////////////////////
		U.push_back("ah");
		break;
	case 19:////////////////////////////////////////////////////////////////////////////////////////////////
		arg = split_string(s,',');
		if (arg.size() == 1)
			setDEFUSE(true,false,arg[0]);
		U.push_back("ds");
		U.push_back("si");
		D.push_back("si");
		D.push_back("eax");
		break;
	case 20:////////////////////////////////////////////////////////////////////////////////////////////////
		arg = split_string(s,',');
		if (arg.size() == 1)
			setDEFUSE(true,false,arg[0]);
		U.push_back("cx");
		D.push_back("cx");
		break;
	case 21:////////////////////////////////////////////////////////////////////////////////////////////////
		arg = split_string(s,',');
		if (arg.size() == 2) {
			string arg1 = setDEFUSE(false,true,arg[0]);
			string arg2 = setDEFUSE(true,false,arg[1]);
			if(opcode.compare("mov")==0){
				if(arg2.compare("NULL")!=0&&arg1.compare("NULL")!=0)
				{
					if(isregister(arg1)){
						Register* r= getRegister(arg1);
						if(isregister(arg2)){
							Register* r2= getRegister(arg2);
							r->value = r2->value;
							r->refmomory = r2->refmomory;
							r->refpointer = r2->refpointer;
							r->refregister = r2->refregister;
							r->rtype = r2->rtype;
						}
						else if(isHexnumber(arg2)||isID(arg2)){
							r->rtype = 0;
							r->value = arg2;
						}else
						{
							r->clear();
							Valriables* var = getVariable(arg2);
							r->value = var->value;
							if(var->ispointer) r->refpointer = arg2; else r->refmomory = arg2;
							r->rtype = var->vtype;
						}
					}else{
						Valriables* var = getVariable(arg1);
						if(isregister(arg2)){
							Register* r2= getRegister(arg2);
							var->value = r2->value;
							var->vtype = r2->rtype;
						}
						else if(isID(arg2)||isHexnumber(arg2)){
							var->value = arg2;
							var->ispointer = true;
							var->vtype = 0;
						}
						else{
							Valriables* var2 = getVariable(arg2);
							var->value = var2->value;
							var->ispointer = var2->ispointer;
							var->vtype = var->vtype;
						}
					}
				}
				else if(arg1.compare("NULL")!=0)
				{
					if(isregister(arg1)){
						Register* r= getRegister(arg1);
						r->value = arg2;
					}
					else{
						Valriables* var = getVariable(arg1);
						var->value = arg2;
					}
				}
			}
		}
		break;
	case 22:////////////////////////////////////////////////////////////////////////////////////////////////
		arg = split_string(s,',');
		if (arg.size() == 2) {
			setDEFUSE(false,true,arg[0]);
			setDEFUSE(true,false,arg[1]);
		}
		U.push_back("ds");
		U.push_back("si");
		D.push_back("si");
		D.push_back("eax");
		break;
	case 23:////////////////////////////////////////////////////////////////////////////////////////////////
		arg = split_string(s,',');
		if (arg.size() == 2) {
			setDEFUSE(true,false,arg[0]);
			setDEFUSE(true,false,arg[1]);
		}
		D.push_back("eax");
		U.push_back("dx");
		break;
	case 24:////////////////////////////////////////////////////////////////////////////////////////////////
		arg = split_string(s,',');
		if (arg.size() == 2) {
			setDEFUSE(true,false,arg[0]);
			setDEFUSE(true,false,arg[1]);
		}
		D.push_back("eax");
		U.push_back("dx");
		U.push_back("si");
		D.push_back("si");
		break;
	case 25://////////////////////////////////////////////////////////////////////////////////////////////// /* one arg def*/  /*pop*/
		arg = split_string(s,',');
		if (arg.size() == 1)
		{
			string value = setDEFUSE(false,true,arg[0]);
			if(opcode.compare("pop")==0 && value.compare("NULL")!=0){
				if(isregister(value)){
					Register* r = getRegister(value);
					r->value = (stack->size()>0)?stack->at(stack->size()-1):"";
					if(r->value.compare("")!=0){
						ini.insert(ini.length()," ;");
						ini.insert(ini.length(),r->value);
					}
				}else if(isptr(value)){
					Valriables* v = getVariable(value);
					v->value = (stack->size()>0)?stack->at(stack->size()-1):"";
					if(v->value.compare("")!=0){
						ini.insert(ini.length()," ;");
						ini.insert(ini.length(),v->value);
					}
				}
				if(stack->size()>0)
					stack->erase(stack->end()-1);
			}
		}
		break;
	case 26:///////////////////////////////////////////////////////////////////////////////////////////////
		D.push_back("di");
		D.push_back("si");
		D.push_back("bp");
		D.push_back("dx");
		D.push_back("cx");
		D.push_back("ax");
		break;
	case 27:///////////////////////////////////////////////////////////////////////////////////////////////
		D.push_back("edi");
		D.push_back("esi");
		D.push_back("ebp");
		D.push_back("edx");
		D.push_back("ecx");
		D.push_back("eax");
		break;
	case 28:///////////////////////////////////////////////////////////////////////////////////////////////
		U.push_back("edi");
		U.push_back("esi");
		U.push_back("ebp");
		U.push_back("edx");
		U.push_back("ecx");
		U.push_back("eax");
		break;
	case 29:///////////////////////////////////////////////////////////////////////////////////////////////
		U.push_back("di");
		U.push_back("si");
		U.push_back("bp");
		U.push_back("dx");
		U.push_back("cx");
		U.push_back("ax");
		break;
	case 30:///////////////////////////////////////////////////////////////////////////////////////////////
		U.push_back("cx");
		D.push_back("cx");
		break;
	case 31:///////////////////////////////////////////////////////////////////////////////////////////////
		D.push_back("ah");
		break;
	case 32:///////////////////////////////////////////////////////////////////////////////////////////////
		arg = split_string(s,',');
		if (arg.size() == 1)
		{U.push_back("eax");setDEFUSE(true,false,arg[0]);}
		U.push_back("di");
		D.push_back("di");
		break;
	case 33:///////////////////////////////////////////////////////////////////////////////////////////////
		arg = split_string(s,',');
		if (arg.size() == 1)
		{
				setDEFUSE(true,false,arg[0]);
				D.push_back("eax");
		}
		U.push_back("di");
		D.push_back("di");
		break;
		break;
	case 34://///////////////////////////////////////////////////////////////////////////////////////////// /* arg 1 and 2 use and def */
		arg = split_string(s,',');
		if (arg.size() == 2) {
			setDEFUSE(false,true,arg[0]);
			setDEFUSE(true,false,arg[1]);
		}
		break;
		break;
	case 35:///////////////////////////////////////////////////////////////////////////////////////////////
		U.push_back("al");
		U.push_back("bx");
		break;
	default:///////////////////////////////////////////////////////////////////////////////////////////////
		break;
	}
}

string instruction::deletespace(string s) {
    string r = "";
	for (unsigned int i = 0; i < s.length(); i++) {
        if (s[i] != ' ' && s[i] != '\t' && s[i] != '\n' && s[i] != '\r') {
            r += s[i];
        }
    }
    return r;
}

bool instruction::isDef(string s) {
        for (unsigned int i = 0; i < D.size(); i++) {
			if (D.at(i).compare(s)==0) {
                return true;
            }
        }
        return false;
}

bool instruction::isUse(string s) {
        for (unsigned int i = 0; i < U.size(); i++) {
            if (U.at(i).compare(s)==0) {
                return true;
            }
        }
        return false;
    }

bool instruction::isID(string s) {
        if (s.length() == 9) {
			char c;
            for (unsigned int i = 0; i < s.length(); i++) {
                c = s[i];
                if (!((c >= 'a' && c <= 'z') ||(c >= 'A' && c <= 'Z') || c == '_' || (c >= '0' && c <= '9'))) {
                    return false;
                }
            }
			return true;
		}
        return false;
}

void instruction::deleteComments() {
    int index = ini.find(';');
    if (index > 0) {
        ini = ini.substr(0, index);
    } else if (index == 0) {
        valid = false;
    }
}

void instruction:: deletesamenameinVector(vector<string> X){
        for(unsigned int i=0;i<X.size();i++)
        {
			string s = X.at(i);
            for(unsigned int j=i+1;j<X.size();j++)
				if(s.compare(X.at(j))==0)
                {
					X.erase(X.begin()+j);
                    j--;
                }
        }
    }

int  instruction:: getindex(){
	for(unsigned int i=0;i<codesindex->size();i++)
		if(codesindex->at(i)->compare(opcode)==0)
			return codesindex->at(i)->index;
	return -1;
}