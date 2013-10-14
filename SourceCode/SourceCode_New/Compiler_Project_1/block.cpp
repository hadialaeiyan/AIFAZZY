
#include "block.h"

block::block(){
	issamplevalue = false;
	blocktype = 0;
}

block* block::Copy(){
	block* cp = new block(Bname);
	cp->issamplevalue = this->issamplevalue;
	for(unsigned int i=0;i<this->insbuf.size();i++)
		cp->add_new_instruction(insbuf[i]);
	for(unsigned int i=0;i<this->_IN.size();i++)
		cp->add_new_IN(this->_IN[i]);
	for(unsigned int i=0;i<this->_OUT.size();i++)
		cp->add_new_OUT(this->_OUT[i],_OUTTEXT[i]);
	return cp;
}

block* block::Copy_with_ini_Copy(){
	block* cp = new block(Bname);
	cp->issamplevalue = this->issamplevalue;
	for(unsigned int i=0;i<this->insbuf.size();i++)
		cp->add_new_instruction(insbuf[i]->Copy());
	for(unsigned int i=0;i<this->_IN.size();i++)
		cp->add_new_IN(this->_IN[i]);
	for(unsigned int i=0;i<this->_OUT.size();i++)
		cp->add_new_OUT(this->_OUT[i],_OUTTEXT[i]);
	return cp;
}

block::block(string Blockname){
	Bname = Blockname;
	issamplevalue = false;
	blocktype = 0;
}

block::~block(){
	for(unsigned int i=0;i<insbuf.size();i++)
		free(insbuf.at(i));
	insbuf.clear();
	_IN.clear();
	_OUT.clear();
	_OUTTEXT.clear();
}

void block::setsample(){
	issamplevalue = true;
}

vector<block*>* block::succ_t(){
	vector<block*>* x = new  vector<block*>();
	for(unsigned int i=0;i<_OUTTEXT.size();i++)
	{
		if(_OUTTEXT.at(i).find("true")!= string::npos)
			x->push_back(_OUT.at(i));
	}
	return x;
}

vector<block*>* block::succ_f(){
	vector<block*>* x = new  vector<block*>();
	for(unsigned int i=0;i<_OUTTEXT.size();i++)
	{
		if(_OUTTEXT.at(i).find("false")!= string::npos)
			x->push_back(_OUT.at(i));
	}
	return x;
}

bool block::issample(){
	return issamplevalue;
}

void block::add_new_instruction(instruction* ins){
	insbuf.push_back(ins);
}

void block::print_ins_out(){
	cout<<"BLOCK "<<Bname<<"\n";
	for(unsigned int i=0;i<insbuf.size();i++)
		insbuf[i]->print_inis();
	cout<<"IN : ";
	for(unsigned int i=0;i<_IN.size();i++)
		if(i<_IN.size()-1)
		cout<<_IN[i]->getname()<<", ";
		else cout<<_IN[i]->getname();
	cout<<"\nout : ";
	for(unsigned int i=0;i<_OUT.size();i++)
		if(i<_OUT.size()-1)
		cout<<_OUT[i]->getname()<<", ";
		else cout<<_OUT[i]->getname();
	cout<<"\nend BLOCK "<<Bname<<"\n";
}

string block::StringForGV(){
	string out = "\"node";
	out.insert(out.length(),Bname.substr(2));
	out.insert(out.length(),"\" [\nlabel = \"<f0>");
	out.insert(out.length(),Bname);
	out.insert(out.length(),"|<f1>");
	if(insbuf.size()>=50){
		for(unsigned int i=0;i<44;i++)
		{
			out.insert(out.length(),insbuf[i]->to_write_iniss());
			out.insert(out.length(),"\\n");
		}
		out.insert(out.length(),"...\\n...\\n...\\n");
		for(unsigned int i=insbuf.size()-3;i<insbuf.size();i++)
		{
			out.insert(out.length(),insbuf[i]->to_write_iniss());
			out.insert(out.length(),"\\n");
		}
	}else{
		for(unsigned int i=0;i<insbuf.size();i++)
		{
			out.insert(out.length(),insbuf[i]->to_write_iniss());
			out.insert(out.length(),"\\n");
		}
	}
	out.insert(out.length(),"|<f2> IN : ");
	for(unsigned int i=0;i<_IN.size();i++)
	{
		out.insert(out.length(),"\\n");
		out.insert(out.length(),_IN[i]->getname());
	}
	out.insert(out.length(),"|<f3> OUT : ");
	for(unsigned int i=0;i<_OUT.size();i++)
	{		
		out.insert(out.length(),"\\n");
		out.insert(out.length(),_OUT[i]->getname());	
	}
	out.insert(out.length(),"\"\nshape = \"Mrecord\"");
	if(getname().compare("BB0")==0){
		out.insert(out.length(),",color = \"red\"");
	}
	else{
		if(issample()){
			out.insert(out.length(),",color = \"orange\"");
		}
		else if(_OUT.size()==0 && _IN.size()==0){
			out.insert(out.length(),",color = \"limegreen\"");
		}
		else if(_OUT.size() == 0)
		{
			out.insert(out.length(),",color = \"peru\"");
		}else if(_IN.size() == 0){
			out.insert(out.length(),",color = \"lightseagreen\"");
		}
	}
	out.insert(out.length(),"\n];\n\n");
	return out;
}

string block::write_to_ins_out(){
	
	string out = "BLOCK ";
	out.insert(out.length(),Bname);
	out.insert(out.length(),"\r\n");
	for(unsigned int i=0;i<insbuf.size();i++)
	{
		out.insert(out.length(),insbuf[i]->to_write_inis());
	}
	out.insert(out.length(),"IN : ");
	for(unsigned int i=0;i<_IN.size();i++)
		if(i<_IN.size()-1)
		{
			out.insert(out.length(),_IN[i]->getname());
			out.insert(out.length(),", ");
		}
		else out.insert(out.length(),_IN[i]->getname());
	out.insert(out.length(),"\r\nout : ");
	for(unsigned int i=0;i<_OUT.size();i++)
		if(i<_OUT.size()-1)
		{
			out.insert(out.length(),_OUT[i]->getname());
			out.insert(out.length(),", ");
		}
		else out.insert(out.length(),_OUT[i]->getname());

	out.insert(out.length(),"\nEND ");
	out.insert(out.length(),"BLOCK ");
	out.insert(out.length(),Bname);
	out.insert(out.length(),"\r\n\r\n");
	return out;
}

bool block::iniisemply(){
	return insbuf.empty();
}

instruction* block::init(int index){
	return insbuf[index];
}

string block::getname(){
	return Bname;
}

int block::size(){
	return insbuf.size();
}

instruction* block::first_init_pop(){
	instruction* temp =	insbuf.front();
	insbuf.erase (insbuf.begin());
	return temp;
}

instruction* block::init_pop_at(int index){
	instruction* temp =	insbuf[index];
	insbuf.erase(insbuf.begin()+index);
	return temp;
}

void block::add_new_OUT(block* b,string text){
	if(notfound(_OUT,b))
	{
		_OUT.push_back(b);
		_OUTTEXT.push_back(text);
	}
}

bool block::notfound(vector<block*> bs,block* b){
	for(unsigned int i=0;i<bs.size();i++)
		if(b->Bname.compare(bs.at(i)->Bname)==0)
			return false;
	return true;
}

void block::add_new_IN(block* b){
	if(notfound(_IN,b))
		_IN.push_back(b);
}

void block::deleteout(string blockname){
	for(unsigned int i=0;i<_OUT.size();i++)
	{
		block* b = _OUT.at(i);
		if((b->Bname).compare(blockname)==0)
		{
			_OUT.erase(_OUT.begin()+i);
			_OUTTEXT.erase(_OUTTEXT.begin()+i);
			break;
		}
	}
}

void block::DELETERELATIONIN(){
	for(unsigned int i=0;i<_IN.size();i++)
	{
		block* b = _IN.at(i);
		b->deleteout(Bname);
	}
}

void block::change_label_with(string newLabel){
	this->insbuf.at(0)->chenge_label_with(newLabel);
}

int block::is_def_a_ptr_with_address(string address){
	for(unsigned int i=0; i<insbuf.size();i++)
	{
		if(insbuf.at(i)->isDef(address))
		return i;
	}
	return -1;
}

string block::getlablename(){
	instruction* ins = insbuf.at(0);
	return ins->getlablename();
}

void block::clone_pop_OUT(vector<block*>* A){
	while(A->size()>0){
		block* b = (block*)(A->at(0));
		_OUT.push_back(b);
		A->erase(A->begin());
	}
}

vector<block*>* block::getOUT(){
	return &_OUT;
}

vector<string>* block::getOUTTEXT(){
	return &_OUTTEXT;
}

vector<block*>* block::getIN(){
	return &_IN;
}

void block::remove_IN(string BBname){
	for(unsigned  int i=0;i<_IN.size();i++){
		if(_IN.at(i)->getname().compare(BBname)==0)
		{
			_IN.erase(_IN.begin()+i);
			break;
		}
	}
}
void block::remove_OUT(string BBname){
	for(unsigned int i=0;i<_OUT.size();i++){
		if(_OUT.at(i)->getname().compare(BBname)==0)
		{
			_OUT.erase(_OUT.begin()+i);
			_OUTTEXT.erase(_OUTTEXT.begin()+i);
			break;
		}
	}
}

void block::Copyoutsinto(block* nb){
	for(unsigned int i=0;i<_OUT.size();i++)
	{
		nb->add_new_OUT((_OUT.at(i)),_OUTTEXT.at(i));
		_OUT.at(i)->remove_IN(getname());
		_OUT.at(i)->add_new_IN(nb);
	}
}

string block::getOUTTEXT(block* b){
	for(unsigned int i=0;i<_OUTTEXT.size();i++)
		if(_OUT.at(i)->getname().compare(b->getname())==0)
			return _OUTTEXT.at(i);
	return "NULL";
}

void block::Copyinsinto(block* nb){
	for(unsigned int i=0;i<_IN.size();i++)
	{
		block* b1 = _IN.at(i);
		string text = b1->getOUTTEXT(this);
		while (text.compare("NULL")==0)
		{
			if(b1->getOUT()->size()>0){
				b1 = b1->getOUT()->at(0);
				text = b1->getOUTTEXT(this);
			}else break;
		}
		nb->add_new_IN(b1);
		b1->remove_OUT(getname());
		b1->add_new_OUT(nb,text);
	}
}

bool block::jumpisfor(string s){
	if(insbuf.size()>=1)
		return insbuf.at(insbuf.size()-1)->opcode.compare(s)==0;
	return false;
}

bool block::samejumpisfor(string s){
	if(insbuf.size()>=1)
		return insbuf.at(insbuf.size()-1)->opcode.find(s) != string::npos;
	return false;
}

block* block::getOUTContinue(){
	for(unsigned int i=0;i<_OUT.size();i++)
		if(_OUTTEXT.at(i).compare("continue")==0)
		{
			return _OUT.at(i);
		}
	return NULL;
}

block* block::getOUTfor(string call){
	for(unsigned int i=0;i<_OUTTEXT.size();i++)
		if(_OUTTEXT.at(i).compare(call)==0)
			return _OUT.at(i);
	return NULL;
}

void block::Delete_IT_return_IN(){
	block* BL =getOUTfor("call");
	if(BL!=NULL)
	{
		if(BL->issample())
		{
			remove_IN(BL->getname());
			BL->remove_OUT(getname());
		}
		else{
			for(unsigned int i=0;i<_IN.size();i++)
				if(_IN.at(i)->jumpisfor("return")){
					block* bIN = _IN.at(i)->getOUTfor("return");
					if(bIN!=NULL){
						remove_IN(bIN->getname());
						bIN->remove_OUT(getname());
					}
				}
		}
	}
}

void block::change_continue_OUT_to(string text){
	for(unsigned int i=0;i<_OUT.size();i++)
		if(_OUTTEXT.at(i).compare("continue")==0)
		{
			_OUTTEXT.insert(_OUTTEXT.begin() + i,text);
			_OUTTEXT.erase(_OUTTEXT.begin()+1+i);
			break;
		}
}

void block::clearout()
{
	_OUT.clear(); _OUTTEXT.clear(); 
}


void block::clearin()
{
	_IN.clear();
}


void block::clearoutelse(string elsestring)
{
	for(int i=_OUT.size()-1;i>=0;i--)
	{
		if(_OUTTEXT.at(i).compare(elsestring)==0)
			continue;
		_OUT.erase(_OUT.begin()+i);
		_OUTTEXT.erase(_OUTTEXT.begin()+i);
	}
}

void block::Copyoutsintoelse(block* nb,string elsestring){
	for(unsigned int i=0;i<_OUT.size();i++)
	{
		if(_OUTTEXT.at(i).compare(elsestring)==0)
			continue;
		nb->add_new_OUT((_OUT.at(i)),_OUTTEXT.at(i));
		_OUT.at(i)->remove_IN(getname());
		_OUT.at(i)->add_new_IN(nb);
	}
}

bool block::isnewerthan(block * b){
	string s1 = b->getname().substr(2),s2 = getname().substr(2);
	int nb,nme;
	if(b->issample())
		s1 = s1.substr(0,s1.length()-10);
	if(issample())
		s2 = s2.substr(0,s2.length()-10);
	nb = atoi(s1.c_str());
	nme = atoi(s2.c_str());
	return nme>nb;
}

vector<block*> block::getOUTsfor(string call){
	vector<block*> bls;
	for(unsigned int i=0;i<_OUTTEXT.size();i++)
		if(_OUTTEXT.at(i).compare(call)==0)
			bls.push_back( _OUT.at(i));
	return bls;
}