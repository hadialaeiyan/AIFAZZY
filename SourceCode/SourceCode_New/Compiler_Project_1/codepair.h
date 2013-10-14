#pragma once
#include <string>
using namespace std;

class codepair{
public:
	
	codepair(string _opcode,int _index){
		opcode = _opcode;
		index = _index;
	}

	codepair(){
	}

	~codepair(){
	}

	string opcode;
	int index;

	int compare(codepair op){
		return opcode.compare(op.opcode);
	}

	int compare(string op){
		return opcode.compare(op);
	}

	bool operator==(codepair op){
		return opcode.compare(op.opcode)==0;
	}
	bool operator>=(codepair op){
		return opcode.compare(op.opcode)>=0;
	}
	bool operator<=(codepair op){
		return opcode.compare(op.opcode)<=0;
	}
	bool operator!=(codepair op){
		return opcode.compare(op.opcode)!=0;
	}
	bool operator<(codepair op){
		return opcode.compare(op.opcode)<0;
	}
	bool operator>(codepair op){
		return opcode.compare(op.opcode)>0;
	}
};
