#include "State.h"


State::State(void)
{
	lastenv = NULL;
	env = NULL;
	block = NULL;
	this->lastStateKind = -1;
	this->StateKind = -1;
}


State::~State(void)
{
	delete block;
	delete lastenv;
	delete env;
}

void State::setstatekind(int statekind){
	this->lastStateKind = this->StateKind;
	this->StateKind = statekind;
}

int State::getlaststateKind(){
	return this->lastStateKind;
}
int State::getstateKind(){
	return this->StateKind;
}