#pragma once
#include "block.h"
#include "ENV.h"

class State
{
public:
	State(void);
	~State(void);

	void freeenvandlastenv(){if(env!=NULL) delete env;if(lastenv!=NULL) delete lastenv;}
	void setENV(ENV* env){if(lastenv!=NULL) delete lastenv; if(this->env!=NULL) this->lastenv = this->env; this->env = env; }
	ENV* getenv(){return this->env;}
	ENV* getlastenv(){return this->lastenv;}
	int getlaststateKind();
	void setstatekind(int statekind);
	int getstateKind();
	
	block* block;
	vector<State*> nextstates;
	
private:
	ENV* env;
	ENV* lastenv;
	int StateKind;
	int lastStateKind;
};

