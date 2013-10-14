#include <string>
#define BEA_ENGINE_STATIC
#define BEA_USE_STDCALL
#include ".\HEADERS\BeaEngine.h"

class Instruction
{
public:
	DISASM DisAsm;
	char Original_opcode[100];
	Instruction* next;
	Instruction(DISASM DisAsm_arg,char* opcode_arg)
	{
		DisAsm=DisAsm_arg;
		int counter=0;
		while(opcode_arg[counter]!=NULL)
		{
			Original_opcode[counter]=opcode_arg[counter];
			counter++;
		}
		Original_opcode[counter]=NULL;
		next=NULL;
	}
};

class DisAssemble
{
	Instruction* first;
public:
	DisAssemble()
	{
		first=NULL;
	}
	bool IsEmpty()
	{
		return first==NULL?true:false;
	}
	void Insert_Instruction(DISASM DisAsm_arg,char* opcode_arg)
	{
		Instruction* new_instruction=new Instruction(DisAsm_arg,opcode_arg);
		if(first==NULL)
			first=new_instruction;
		else
		{
			Instruction* q=first;
			while(q->next!=NULL)
				q=q->next;
			q->next=new_instruction;
		}
	}
	Instruction get(int Instruction_Number)
	{
		int counter=1;
		Instruction* getted_instruction=first;
		while(counter<Instruction_Number && getted_instruction!=NULL)
		{
			getted_instruction=getted_instruction->next;
			counter++;
		}
		return *getted_instruction;
	}
	int Count_Instructions()
	{
		Instruction* q=first;
		int counter=0;
		while(q!=NULL)
		{
			counter++;
			q=q->next;
		}
		return counter;
	}
};