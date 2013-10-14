#include <windows.h>
#include <stdio.h>
#include <string>
#include "LinkedList.h"

using namespace std;

#ifdef __cplusplus
#define EXPORT extern "C" __declspec (dllexport)
#else
#define EXPORT __declspec (dllexport)
#endif

EXPORT BOOL CALLBACK AntiAntiDisassemble (PTCHAR,PTCHAR,int (*)(void),PTCHAR,void (*)(char *,char *, int (*)(void),char*,bool),void (*) (PTCHAR,PTCHAR),int,PTCHAR) ;


linklist locations;//Mahale Dasturat ra negahdari mikonad.
linklist jmp_targets;//Adrese Pareshha ra negahdari mikonad.
linklist jmp_targets_bads;//Adrese jmp targethaye moshkeldar

char ConvertOneDigit(char adad)//In tabe ye character az vorudi migirad va an ra be adad tabdil mikonad. in tabe farz mikonad ke vorudi be surate hex ast.
{
	if(adad=='F' || adad=='f')
		return 15;
	else if(adad=='E' || adad=='e')
		return 14;
	else if(adad=='D' || adad=='d')
		return 13;
	else if(adad=='C' || adad=='c')
		return 12;
	else if(adad=='B' || adad=='b')
		return 11;
	else if(adad=='A' || adad=='a')
		return 10;
	else
		return adad-48;
}


void GetLocations(PTCHAR disassembled_code)
{
	int cur_location=0,cur_location_old=0;
	string new_loc="";

	locations.clear();
	while(disassembled_code[cur_location]!=NULL)
	{
		new_loc="";
		while(disassembled_code[cur_location]!=':' && disassembled_code[cur_location]!=NULL && disassembled_code[cur_location]!='\r')
		{
			new_loc.append(1,disassembled_code[cur_location]);
			cur_location++;
		}
		if(disassembled_code[cur_location]==':')
		{
			locations.insert((char*)new_loc.c_str());
			while(disassembled_code[cur_location]!='\n' && disassembled_code[cur_location]!=NULL) 
				cur_location++;
			if(disassembled_code[cur_location]!='\n')
				return;
			cur_location++;
		}
		else
			return;
	}
}

void GetJmpTargets(PTCHAR disassembled_code)
{
	
	int cur_location=0;
	string dasturate_pareshi[]={"jmp","ja", "jae", "jb", "jbe", "jc", "jcxz", "je", "jg", "jge", "jl", "jle", "jna", "jnae", "jnb", "jnbe", "jnc", "jne", "jng", "jnge", "jnl", "jnle", "jno", "jnp", "jns", "jnz", "jo", "jp", "jpe", "jpo", "js", "jz"};
	jmp_targets.clear();
	while(disassembled_code[cur_location]!=NULL)
	{
		while(disassembled_code[cur_location]!=':' && disassembled_code[cur_location]!=NULL && disassembled_code[cur_location]!='\r')
			cur_location++;
		if(disassembled_code[cur_location]==':')
		{
			string name_dastur="";
			cur_location+=2;
			while(disassembled_code[cur_location]!=' ')
			{
				name_dastur.append(1,disassembled_code[cur_location]);
				cur_location++;
			}
			//aya in dastur yek dasture pareshi ast?
			int i=0;
			for(i=0;i<32;i++)
				if(dasturate_pareshi[i]==name_dastur)
				{
					string mahal="";
					cur_location++;
					while(disassembled_code[cur_location]!=' ')
					{
						mahal.append(1,disassembled_code[cur_location]);
						cur_location++;
					}
					if(mahal[mahal.length()-1]=='h')
					{
						mahal=mahal.substr(0,mahal.length()-1);
						//agar in mahal tekrari nabashad,an ra be list ezafe mikonim:
						if(jmp_targets.find((char*)mahal.c_str())==false)
							jmp_targets.insert((char*)mahal.c_str());
					}
					break;
				}
			//jahate raftan be dasture badi:
			while(disassembled_code[cur_location]!='\n' && disassembled_code[cur_location]!=NULL) 
				cur_location++;
			if(disassembled_code[cur_location]!='\n')
				return;
		}
		else
			return;
		
	}
}

EXPORT BOOL CALLBACK AntiAntiDisassemble (PTCHAR StartCodeSection,PTCHAR EndCodeSection,int (*Virtual_Address)(void),PTCHAR disassembled_code,void (*Disassembler)(char *,char *, int (*)(void),char*,bool),void (*CreateCFG) (PTCHAR,PTCHAR),int code_length,PTCHAR output)
{
	int i=0,temp_int=0;
	char loc[256],temp[256],*p;
	GetLocations(disassembled_code);
	GetJmpTargets(disassembled_code);
	jmp_targets.sortElements();
	while(jmp_targets.first!=NULL && jmp_targets.GetNodeData(i,loc)==true)//pareshhaye bad ra peida mikonim.
	{
		if(locations.find(loc)==false)
		{
			//aya in paresh dar dakhele codehayee ke ma disassemble kardim gharar darad,ya kheir?
			int ebteda=(int)Virtual_Address;
			int enteha=(int)Virtual_Address+code_length;
			int mahal=strtol(loc,&p,16);
			if(mahal>=ebteda && mahal<=enteha)
				jmp_targets_bads.insert(loc);
		}
		i++;
	}

	int count=jmp_targets_bads.element_count();
	unsigned int shoroo=0;
	unsigned int shoroo_badi;
	unsigned int returned_length=0;
	for(i=0;i<count+1;i++)
	{
		char temp[256];
		if(!jmp_targets_bads.GetNodeData(i,temp))//aya tahe list ast?
			shoroo_badi=code_length;
		else
			shoroo_badi=(ConvertOneDigit(temp[0])*268435456+ConvertOneDigit(temp[1])*16777216+ConvertOneDigit(temp[2])*1048576+ConvertOneDigit(temp[3])*65536+ConvertOneDigit(temp[4])*4096+ConvertOneDigit(temp[5])*256+ConvertOneDigit(temp[6])*16+ConvertOneDigit(temp[7])*1)-(long long)Virtual_Address;
		Disassembler(StartCodeSection+shoroo,StartCodeSection+shoroo_badi,(int(*)())((long long)Virtual_Address+shoroo),output+returned_length,true);
		returned_length=strlen(output);
		shoroo=shoroo_badi;
	}
	return TRUE;
}
