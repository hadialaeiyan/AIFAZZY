#include "Range_Analysis.h"


Range_Analysis::Range_Analysis(void)
{
	string myDirectory="";//jai ke file exe dar anja gharar darad.
	string quary = "";
	char myPath[_MAX_PATH+1];

	//baraye daryafte masire feli:
	GetModuleFileName(NULL,myPath,_MAX_PATH);
	quary.insert(quary.length(),myPath);
	int locationfind = quary.find_last_of('\\');
	if(locationfind >=0)
	{
		quary = quary.substr(0,locationfind+1);
		myDirectory=quary;
	}
	myDirectory.insert(myDirectory.length(),"Logs\\Range_Analysis");
	
	DEBUG_CONF(myDirectory, Logger::file_on|Logger::screen_on, DBG_DEBUG, DBG_ERROR);//config logger.

}

bool FIND(vector<string>* s,string text){
	for(int i=0;i<s->size();i++)
		if (text.compare(s->at(i)) == 0)
			return true;
	return false;
}

Range_Analysis::~Range_Analysis(void)
{
	
}

/**
* return 0 -> successfull
* return 1 -> Start basic block Not found.
*/
int Range_Analysis::DOANALYSIS(){
	this->stateTree = make_states();
	if(this->stateTree == NULL)
		return 1;
	____________ANALIYSIS(this->stateTree);
	vector<string> bbnamesaver;
	____________ANALIYSIS_UPOperation(this->stateTree,&bbnamesaver);
	bbnamesaver.clear();
	return 0;
}

void Range_Analysis::____________ANALIYSIS_UPOperation(State* Statetree_root,vector<string>* bbnamesaver){
	if(Statetree_root->block->blocktype ==1) // entery
	{
		____________ANALIYSIS_UPOperation(Statetree_root->nextstates[0],bbnamesaver);
	}
	else if(Statetree_root->block->blocktype == 2)// assignment
	{
		____________Merge_Environment_Junctions__UPOPERATION(Statetree_root);
		//this->OPcsm.DOsimulation(Statetree_root);
		____________ANALIYSIS_UPOperation(Statetree_root->nextstates[0],bbnamesaver);
	}
	else if(Statetree_root->block->blocktype == 3) // jumps
	{
		____________Merge_Environment_Junctions__UPOPERATION(Statetree_root);
		____________ANALIYSIS_CONDITION_(Statetree_root);
		____________ANALIYSIS_UPOperation(Statetree_root->nextstates[0],bbnamesaver);
		____________ANALIYSIS_UPOperation(Statetree_root->nextstates[1],bbnamesaver);
	}
	else if(Statetree_root->block->blocktype == 4) // junction
	{
		if(!FIND(bbnamesaver,Statetree_root->block->getname())){
			bbnamesaver->push_back(Statetree_root->block->getname());
			____________Merge_Environment_Junctions__UPOPERATION(Statetree_root);
			____________ANALIYSIS_UPOperation(Statetree_root->nextstates[0],bbnamesaver);
		}
	}
	else if(Statetree_root->block->blocktype == 5) // exit
	{
		//exit
	}
}

void Range_Analysis::____________Merge_Environment_Junctions__UPOPERATION(State* _State){
	vector<State*>* states = Get_ALL_Parent_OF_State(this->stateTree,_State);
	if(states->size()>0){
		ENV* e = states->at(0)->getenv();
		if(e!=NULL){
			e = e->Copy();
			for(int i=1;i<states->size();i++)
			{
				e->UNION(states->at(i)->getenv());
			}
			if(_State->getenv()!=NULL){
				_State->getenv()->operatorUP(e);
			}
		}
		states->clear();
	}
	free(states);
}

void Range_Analysis::loadFLAGSANDREGISTERS(ENV* env){
	Variable* temp = new Variable("is");//Stack overflow or underflow
	temp->Ranges.push_back(new Range(0,0));
	env->addtoregister(temp);
	temp = new Variable("ia");//- Invalid arithmetic operation
	temp->Ranges.push_back(new Range(0,0));
	env->addtoregister(temp);
	temp = new Variable("z");//Floating-point divide-by-zero
	temp->Ranges.push_back(new Range(0,0));
	env->addtoregister(temp);
	temp = new Variable("d");//Floating-point denormal operand
	temp->Ranges.push_back(new Range(0,0));
	env->addtoregister(temp);
	temp = new Variable("o");//Floating-point numeric overflow
	temp->Ranges.push_back(new Range(0,0));
	env->addtoregister(temp);
	temp = new Variable("u");//Floating-point numeric underflow
	temp->Ranges.push_back(new Range(0,0));
	env->addtoregister(temp);
	temp = new Variable("p");//Floating-point inexact result (precision)
	temp->Ranges.push_back(new Range(0,0));
	env->addtoregister(temp);
	temp = new Variable("af");
	temp->Ranges.push_back(new Range(0,0));
	env->addtoregister(temp);
	temp = new Variable("cf");
	temp->Ranges.push_back(new Range(0,0));
	env->addtoregister(temp);
	string namesa[] = {"rax","eax","ax","ah","al"};
	temp = new Variable(namesa,5);
	temp->Ranges.push_back(new Range(0,0));
	env->addtoregister(temp);
	string namesb[] = {"rbx","ebx","bx","bh","bl"};
	temp = new Variable(namesb,5);
	temp->Ranges.push_back(new Range(0,0));
	env->addtoregister(temp);
	string namesc [] = {"rcx","ecx","cx","ch","cl"};
	temp = new Variable(namesc,5);
	temp->Ranges.push_back(new Range(0,0));
	env->addtoregister(temp);
	string namesd[] = {"rdx","edx","dx","dh","dl"};
	temp = new Variable(namesd,5);
	temp->Ranges.push_back(new Range(0,0));
	env->addtoregister(temp);
}

void Range_Analysis::____________ANALIYSIS(State* Statetree_root){
	if(Statetree_root->block->blocktype ==1) // entery
	{
		loadFLAGSANDREGISTERS(Statetree_root->getenv());
		Statetree_root->nextstates[0]->setENV( Statetree_root->getenv()->Copy());
		____________ANALIYSIS(Statetree_root->nextstates[0]);
	}
	else if(Statetree_root->block->blocktype == 2)// assignment
	{
		this->OPcsm.DOsimulation(Statetree_root);
		Statetree_root->nextstates[0]->setENV( Statetree_root->getenv()->Copy());
		____________ANALIYSIS(Statetree_root->nextstates[0]);
	}
	else if(Statetree_root->block->blocktype == 3) // jumps
	{
		if(____________ANALIYSIS_CONDITION_(Statetree_root))
		{
			if(Statetree_root->getstateKind()==0)
			{
				block* succ_f = Statetree_root->block->succ_f()->at(0);
				State* S_succ_f = ((succ_f->getname().compare(Statetree_root->nextstates[0]->block->getname())==0)? Statetree_root->nextstates.at(0) : Statetree_root->nextstates.at(1));
				S_succ_f->setENV(Statetree_root->getlastenv()->Copy());
				____________ANALIYSIS_until_state(Statetree_root,S_succ_f);
			}
			block* succ_t = Statetree_root->block->succ_t()->at(0);
			State* S_succ_t = ((succ_t->getname().compare(Statetree_root->nextstates[0]->block->getname())==0)? Statetree_root->nextstates.at(0) : Statetree_root->nextstates.at(1));
			S_succ_t->setENV(Statetree_root->getenv()->Copy());
			Statetree_root->setstatekind(1);
			____________ANALIYSIS(S_succ_t);
		}else {
			if(Statetree_root->getstateKind()==1)
			{
				block* succ_t = Statetree_root->block->succ_t()->at(0);
				State* S_succ_t = ((succ_t->getname().compare(Statetree_root->nextstates[0]->block->getname())==0)? Statetree_root->nextstates.at(0) : Statetree_root->nextstates.at(1));
				S_succ_t->setENV(Statetree_root->getlastenv()->Copy());
				____________ANALIYSIS_until_state(Statetree_root,S_succ_t);
			}
			block* succ_f = Statetree_root->block->succ_f()->at(0);
			State* S_succ_f = ((succ_f->getname().compare(Statetree_root->nextstates[0]->block->getname())==0)? Statetree_root->nextstates.at(0) : Statetree_root->nextstates.at(1));
			S_succ_f->setENV(Statetree_root->getenv()->Copy());
			Statetree_root->setstatekind(0);
			____________ANALIYSIS(S_succ_f);
		}
	}
	else if(Statetree_root->block->blocktype == 4) // junction
	{
		____________Merge_Environment_Junctions(Statetree_root);
		Statetree_root->nextstates[0]->setENV( Statetree_root->getenv()->Copy());
		____________ANALIYSIS(Statetree_root->nextstates[0]);
	}
	else if(Statetree_root->block->blocktype == 5) // exit
	{
		//exit
	}
}

void Range_Analysis::____________ANALIYSIS_until_state(State* P,State* c){
	if(P->block->getname().compare(c->block->getname())==0)
		return;
	if(c->block->blocktype ==1) // entery
	{
		if(P->block->getname().compare(c->nextstates[0]->block->getname())!=0){
			c->nextstates[0]->setENV( c->getenv()->Copy());
			____________ANALIYSIS_until_state(P,c->nextstates[0]);
		}
	}
	else if(c->block->blocktype == 2)// assignment
	{
		this->OPcsm.DOsimulation(c);
		if(P->block->getname().compare(c->nextstates[0]->block->getname())!=0){
			c->nextstates[0]->setENV( c->getenv()->Copy());
			____________ANALIYSIS_until_state(P,c->nextstates[0]);
		}
	}
	else if(c->block->blocktype == 3){//jumps
		if(____________ANALIYSIS_CONDITION_(c))
		{
			if(c->getstateKind()==0)
			{
				block* succ_f = c->block->succ_f()->at(0);
				State* S_succ_f = ((succ_f->getname().compare(c->nextstates[0]->block->getname())==0)? c->nextstates.at(0) : c->nextstates.at(1));
				if(P->block->getname().compare(S_succ_f->block->getname())!=0){
				S_succ_f->setENV(c->getlastenv()->Copy());
				____________ANALIYSIS_until_state(c,S_succ_f);
				}
			}
			block* succ_t = c->block->succ_t()->at(0);
			State* S_succ_t = ((succ_t->getname().compare(c->nextstates[0]->block->getname())==0)? c->nextstates.at(0) : c->nextstates.at(1));
			if(P->block->getname().compare(S_succ_t->block->getname())!=0){
				S_succ_t->setENV(c->getenv()->Copy());
				c->setstatekind(1);
				____________ANALIYSIS_until_state(P,S_succ_t);
			}
		}else {
			if(c->getstateKind()==1)
			{
				block* succ_t = c->block->succ_t()->at(0);
				State* S_succ_t = ((succ_t->getname().compare(c->nextstates[0]->block->getname())==0)? c->nextstates.at(0) : c->nextstates.at(1));
				if(P->block->getname().compare(S_succ_t->block->getname())!=0){
					S_succ_t->setENV(c->getlastenv()->Copy());
					____________ANALIYSIS_until_state(c,S_succ_t);
				}
			}
			block* succ_f = c->block->succ_f()->at(0);
			State* S_succ_f = ((succ_f->getname().compare(c->nextstates[0]->block->getname())==0)? c->nextstates.at(0) : c->nextstates.at(1));
			if(P->block->getname().compare(S_succ_f->block->getname())!=0){
				S_succ_f->setENV(c->getenv()->Copy());
				c->setstatekind(0);
				____________ANALIYSIS_until_state(P,S_succ_f);
			}
		}
	}
	else if(c->block->blocktype == 4) // junction
	{
		____________Merge_Environment_Junctions(c);
		if(P->block->getname().compare(c->nextstates[0]->block->getname())!=0){
			c->nextstates[0]->setENV( c->getenv()->Copy());
			____________ANALIYSIS_until_state(P,c->nextstates[0]);
		}
	}
	else if(c->block->blocktype == 5) // exit
	{
		//exit
	}
}

void Range_Analysis::____________Merge_Environment_Junctions(State* _State){
	vector<string> bbnamesaver;
	vector<State*>* states = Get_ALL_Parent_OF_State(this->stateTree,_State,&bbnamesaver);
	if(states->size()>0){
		ENV* e = states->at(0)->getenv()->Copy();
		for(int i=1;i<states->size();i++)
		{
			e->UNION(states->at(i)->getenv());
		}
		if(_State->getlastenv()!=NULL){
			_State->getlastenv()->operatorDOWN(e);
			_State->setENV(_State->getlastenv()->Copy());
		}
		states->clear();
	}
	free(states);
}

void Range_Analysis::merge2vector(vector<State*>* v1,vector<State*>* v2){
	for(int i=0;i<v2->size();i++)
	{
		v1->push_back(v2->at(i));
	}
}

vector<State*>* Range_Analysis::Get_ALL_Parent_OF_State(State* Statetree_root,State* _State){
	vector<string> bbnamesaver;
	vector<State*>* states = Get_ALL_Parent_OF_State(Statetree_root,_State,&bbnamesaver);
	return states;
}

vector<State*>* Range_Analysis::Get_ALL_Parent_OF_State(State* Statetree_root,State* _State,vector<string>* bbnamesaver){
	vector<State*>* states = new vector<State*>();
	if(Statetree_root->block->blocktype ==1) // entery
	{
		if(Statetree_root->nextstates[0]->block->getname().compare(_State->block->getname())==0) states->push_back(Statetree_root);
		vector<State*>* states1 = Get_ALL_Parent_OF_State(Statetree_root->nextstates[0],_State,bbnamesaver); 
		merge2vector(states,states1); states1->clear(); free(states1);
	}
	else if(Statetree_root->block->blocktype == 2)// assignment
	{
		if(Statetree_root->nextstates[0]->block->getname().compare(_State->block->getname())==0) states->push_back(Statetree_root);
		vector<State*>* states1 = Get_ALL_Parent_OF_State(Statetree_root->nextstates[0],_State,bbnamesaver); 
		merge2vector(states,states1); states1->clear(); free(states1);
	}
	else if(Statetree_root->block->blocktype == 3) // test
	{
		if(Statetree_root->nextstates[0]->block->getname().compare(_State->block->getname())==0) states->push_back(Statetree_root);
		vector<State*>* states1 = Get_ALL_Parent_OF_State(Statetree_root->nextstates[0],_State,bbnamesaver); 
		merge2vector(states,states1); states1->clear(); free(states1);
		if(Statetree_root->nextstates[1]->block->getname().compare(_State->block->getname())==0) states->push_back(Statetree_root);
		states1 = Get_ALL_Parent_OF_State(Statetree_root->nextstates[1],_State,bbnamesaver); 
		merge2vector(states,states1); states1->clear(); free(states1);
	}
	else if(Statetree_root->block->blocktype == 4) // junction
	{
		if(!FIND(bbnamesaver,Statetree_root->block->getname())){
			bbnamesaver->push_back(Statetree_root->block->getname());
			if(Statetree_root->nextstates[0]->block->getname().compare(_State->block->getname())==0) states->push_back(Statetree_root);
			vector<State*>* states1 = Get_ALL_Parent_OF_State(Statetree_root->nextstates[0],_State,bbnamesaver);
			merge2vector(states,states1); states1->clear(); free(states1);
		}
	}
	else if(Statetree_root->block->blocktype == 5) // exit
	{
		//exit
	}
	return states;
}

bool Range_Analysis::____________ANALIYSIS_CONDITION_(State* state){
	return OPcsm.Cmp_OR_test(state);
}

/*
dar in ghesmate baraye har black ke shamele yek dastor ast yek state ra baraye black dar nazar mighirad
*/
State* Range_Analysis::make_states(){
	HDICTIONARY<State*> bbnamesaver;
	block* cur = findbasicBlockStart();
	if(cur==NULL)
		return NULL;
	HDICTIONARYNODEKEY<State*>* wk = new HDICTIONARYNODEKEY<State*>();
	bbnamesaver.add(cur->getname(),wk);
	State* cur_st = new State();
	wk->_value_ = cur_st;
	cur_st->block = cur;
	cur_st->setENV(new ENV());

	vector<block*>* childs = cur->getOUT();
	for(unsigned int i=0;i<childs->size();i++)
	{
		State* child_st = make_states(childs->at(i),&bbnamesaver);
		cur_st->nextstates.push_back(child_st);
	}
	return cur_st;
}

State* Range_Analysis::make_states(block* cur,HDICTIONARY<State*>* bbnamesaver){
	if(!(bbnamesaver->find(cur->getname()))){
		HDICTIONARYNODEKEY<State*>* wk = new HDICTIONARYNODEKEY<State*>();
		bbnamesaver->add(cur->getname(),wk);
		State* cur_st = new State();
		wk->_value_ = cur_st;
		cur_st->block = cur;
		cur_st->setENV( NULL);
		vector<block*>* childs = cur->getOUT();
		for(unsigned int i=0;i<childs->size();i++)
		{
			State* child_st = make_states(childs->at(i),bbnamesaver);
			cur_st->nextstates.push_back(child_st);
		}
		return cur_st;
	}else{
		HDICTIONARYNODEKEY<State*>* wk = bbnamesaver->getkey(cur->getname());
		return wk->_value_;
	}
}

/**
basic block start ra peyda mikonad va an ra barmighardanad.
block ra az for_each_ini mighirad.
*/
block* Range_Analysis::findbasicBlockStart(){
	for(unsigned int i=0;i<for_each_ini->size();i++)
		if(for_each_ini->at(i)->blocktype == 1)
			return for_each_ini->at(i);
	return NULL;
}

void Range_Analysis::setBasicBlocks(vector<block*>* for_each_ini){
	this->for_each_ini = for_each_ini;
}

vector<block*>* Range_Analysis::getBasicBlocks(){
	return this->for_each_ini;
}