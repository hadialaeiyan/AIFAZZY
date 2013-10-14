
#include "FGD.h"


CFGD::CFGD(void)
{
	Bnamecounter =0;
	bnum = 30;
	wtofile = true ;
	stocons = false;
	makefilenameforout();
}

CFGD::~CFGD(void)
{

}

vector<string> split_string(string s,char c){
	vector<string> data;
	int lastindex = 0;
	for(int i =0 ; i<s.length();i++)
		if(s[i]==c)
		{
			data.push_back(s.substr(lastindex,i-lastindex));
			lastindex = i+1;
		}
	return data;
}

string CFGD::maketruelable(string s){
	if(s.length()>1 && ( s[s.length()-1]=='\r' && (s[s.length()-2]=='h' || s[s.length()-2]=='H')))
		return s.substr(0,s.length()-2);
	else if(s.length()>0 && ( s[s.length()-1]=='h' || s[s.length()-1]=='H'))
		return s.substr(0,s.length()-1);
	return s;
}

/*copy in to file ACM.txt and call DOCFG.*/
void CFGD::DOCFG(char* in){
	string s = in;
	bbuf.clear();
	labels.clear();
	vector<string> inp = split_string(s,'\n');
	//////load first block list.
	/// in this part the proc... try to read line by line from
	/// file and save it in memmory in first block that it's name is main
	block * newblock = new block(getnextbname());								//init first block
	bbuf.push_back(newblock);													//push first block to block lists.
	for(int linenumber = 0; linenumber<inp.size();)
	{
		string line = inp.at(linenumber);
		if(line.length() > 0)													//if the line is not empty, the line add to the first block
		{
			linenumber++; 														// enter new line.
			instruction* ins = new instruction(line,linenumber);				// define new instruction and set its line number.
			if(ins->islableform()) //if have a lable to instruction save it in block and make a object Label and save it in to the label buffer
			{
				newblock->add_new_instruction(ins);
				lable* newlabel = new lable(ins->getlablename(),newblock,newblock->size());
				labels.push_back(newlabel);
			}
			else newblock->add_new_instruction(ins); 								//add the instruction in first 
		}
	}

	setbranchkind();
	Do_Blocking(&bbuf); ///in this function the first block proken to n block that program need.
	
	Delete_Not_Use_Block(&bbuf); // in blocking the ASM code some of block in progressing make that have not any instruction. that must be delete.
	if(wtofile)
		write_to_file(bbuf); // write the result to file
	if(stocons){
		cout<<"///////////////result\\\\\\\\\\\\\\\\";
		print_block_out(bbuf); // print in CMD 
	}
}

/**                                                           
start the create CFG from ACM code.
read the ASM.txt code and final result save to file CFGoutput.txt 
for get it from memory the CFG saved in bbuf
*/
void CFGD:: DOCFG(){
	bbuf.clear();
	labels.clear();
	string line;																//line is a string that save a new information when read a line from file
	ifstream file;																//define a file
	file.open(filename);														//open the file to read the file name is a define filename the up is defined.
	
	int linenumber = 0;															//this is a counter for count the line number of file that reading.
	block * newblock = new block(getnextbname());								//init first block
	//////load first block list.
	/// in this part the proc... try to read line by line from
	/// file and save it in memmory in first block that it's name is main
	bbuf.push_back(newblock);													//push first block to block lists.
	while(getline(file, line))													//read one line after another one line 
	{
		if(line.length() > 0)													//if the line is not empty, the line add to the first block
		{
			linenumber++; 														// enter new line.
			instruction* ins = new instruction(line,linenumber);				// define new instruction and set its line number.
			if(ins->islableform()) //if have a lable to instruction save it in block and make a object Label and save it in to the label buffer
			{
				newblock->add_new_instruction(ins);
				lable* newlabel = new lable(ins->getlablename(),newblock,newblock->size());
				labels.push_back(newlabel);
			}
			else newblock->add_new_instruction(ins); 								//add the instruction in first 
		}
	}
	
	setbranchkind();
	Do_Blocking(&bbuf); ///in this function the first block proken to n block that program need.
	
	Delete_Not_Use_Block(&bbuf); // in blocking the ASM code some of block in progressing make that have not any instruction. that must be delete.
	if(wtofile)
		write_to_file(bbuf); // write the result to file
	if(stocons){
		cout<<"///////////////result\\\\\\\\\\\\\\\\";
		print_block_out(bbuf); // print in CMD 
		
	}
		
}


/*return the BBn. use to create new block*/
string CFGD::getnextbname(){
	string ret("BB");
	char c[10];
	_itoa_s(Bnamecounter,c,sizeof(c),10);
	ret.insert(2,c);
	Bnamecounter++;
	return ret;
}
/*return the BBoutofrangen. use to create new block
use for not found labels*/
string CFGD::getnextbnameoutsilde(){
	string ret("BBoutofrange");
	char c[10];
	_itoa_s(Bnamecounter,c,sizeof(c),10);
	ret.insert(2,c);
	Bnamecounter++;
	return ret;
}
/*enter the branches syntax to the String branches*/
/*if jump is sharty index es bayad dar case sharty ha gharar dade shavad
hamin kar ba gheyre sharty bayad anjam shavad 
aghar ret ast bayad a case ret gharadade beshavad va dar tabeye gotowhere az
class insruction if(10==i) vojod dare bayad if(10==i && ?==i) neez gharar beghire
dar morede call niz dar case call bayad index gharar beghirad*/
void CFGD::setbranchkind(){
	branches = new string[bnum];
	branches[10] = "ret";
	branches[1] = "call";
	branches[9] = "jump";//no sharty
	branches[3] = "je";// sharty
	branches[4] = "jne";// sharty
	branches[5] = "jle";// sharty
	branches[6] = "jl";// sharty
	branches[7] = "jbe";// sharty
	branches[11] = "jz";// sharty
	branches[12] = "jnz";// sharty
	branches[13] = "jnl";// sharty
	branches[14] = "jng";// sharty
	branches[15] = "jnae";// sharty
	branches[16] = "jna";// sharty
	branches[17] = "jnc";// sharty
	branches[18] = "jc";// sharty
	branches[19] = "jns";// sharty
	branches[20] = "jge";// sharty
	branches[21] = "jnge";// sharty
	branches[22] = "jnle";// sharty
	branches[23] = "jg";// sharty
	branches[24] = "js";// sharty
	branches[25] = "ja";// sharty
	branches[26] = "jnb";// sharty
	branches[27] = "jae";// sharty
	branches[28] = "jnbe";// sharty
	branches[0] = "jb";// sharty
	branches[2] = "jmp";//no sharty
	branches[8] = "goto"; //no sharty
    branches[29] = "loop";// sharty
}

/*get a block b and seperate from instruction startindex to end and store they to tonb (to new block)*/
/* label ha ro ham moratab mikonad. dar shekastane block adress va block label ha momken azt taghir konad in moratb sazy ro anjam midahad*/
void CFGD::sep_block(block* b,int startindex,block* tonb){
	int k=0;
	string namel;
	while(startindex<b->size())
	{
		instruction* ini = b->init_pop_at(startindex);
		tonb->add_new_instruction(ini);
		k++;
		if(ini->islableform())
		{
			namel = ini->getlablename();
			for(unsigned int p=0;p < labels.size();p++)
			{
				if(namel.compare(labels.at(p)->name)==0)
				{
					//cout<<"label "<<namel<<" must be change to "<<k<<"\n";
					labels.at(p)->blockini = k;
					labels.at(p)->blockL = tonb;
				}
			}
		}
	}
}

// blocking ro anjam midahad. halat haye return va call va jump haye sharty va ghayre sharty dar har case moshakhash shode hastand
void CFGD::Do_Blocking(vector<block*>* inss){
	for(unsigned int i=0;i<inss->size();i++)///////be ezaye block ha. Anhaye ke dar teye shekaste shodan tolid mishavand ham chek mishavand
	{
		block* b = inss->at(i); // get the block
		// from fisrt instruction to last instruction if in it found a branche code seperate it to an other block;
		int j=0;
		instruction * ini = NULL;
		for(;j<b->size();j++)
		{
			ini = b->init(j);
			if(ini->isBranch(branches,bnum))
			{
				j++;
				// if this instruction have a branch the block must broken after the branches. the instructions have in the next of 
				// branch save in new block. in the next progress continue from the next block.
				int index;
				string labelname = ini->gotowhere(branches,bnum,&index);
				labelname = maketruelable(labelname);
				block* blocklabel;
				switch(index){
					case 10: // for ret or return no need to do any work
						break;
					case 1: // for call
					// dar call miravad dar block call shode va bar mighardad va edame midahad
						blocklabel =  get_block_lable(labelname,&bbuf);
						if(blocklabel!=0){
							block* nb = new block(getnextbname());
							inss->push_back(nb);
							sep_block(b,j,nb);
							blocklabel->add_new_IN(b);
							b->add_new_IN(blocklabel);
							b->add_new_OUT(blocklabel);
							blocklabel->add_new_OUT(b);
							nb->add_new_IN(b);
							b->add_new_OUT(nb);
						}
						else{
							//یک بلاک جدید می کنیم و یک دستور با نام لیبل آن در آن قرار می دهیم. و لیبل جدید را در لیست لیبل ها نیز اضافه می کنیم اگر وجود نداشته باشد.
							blocklabel = new block(getnextbnameoutsilde());
							blocklabel->setsample();
							lable* l = new lable(labelname,blocklabel,1);
							labelname.insert(labelname.length(),":");
							blocklabel->add_new_instruction(new instruction(labelname,-1));
							inss->push_back(blocklabel);
							//////////////
							block* nb = new block(getnextbname());
							inss->push_back(nb);
							sep_block(b,j,nb);
							blocklabel->add_new_IN(b);
							b->add_new_IN(blocklabel);
							b->add_new_OUT(blocklabel);
							blocklabel->add_new_OUT(b);
							nb->add_new_IN(b);
							b->add_new_OUT(nb);
						}
						break;
					case 2: case 9: case 8: // branch ghayre sharty
					//dar jump gheyre sharty miravad be block label va bar nemighardad
						blocklabel =  get_block_lable(labelname,&bbuf);
						if(blocklabel!=0){
							block* nb = new block(getnextbname());
							inss->push_back(nb);
							sep_block(b,j,nb);
							blocklabel->add_new_IN(b);
							b->add_new_OUT(blocklabel);
							nb->add_new_OUT(blocklabel);
						}else{
							blocklabel = new block(getnextbnameoutsilde());
							blocklabel->setsample();
							lable* l = new lable(labelname,blocklabel,1);
							labelname.insert(labelname.length(),":");
							blocklabel->add_new_instruction(new instruction(labelname,-1));
							inss->push_back(blocklabel);
							//////////////////////////////////////////////////
							block* nb = new block(getnextbname());
							inss->push_back(nb);
							sep_block(b,j,nb);
							blocklabel->add_new_IN(b);
							b->add_new_OUT(blocklabel);
							nb->add_new_OUT(blocklabel);
						}
						break;
					case 3:case 29:case 28:case 27:case 26:case 25:case 24:case 23:case 22:case 21:case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 20:case 11: case 12: case 4: case 5: case 6: case 7: case 0: // jump haye sharty.
					// dar in mored aghar shart sadegh bashad miravad be block label else edeme block ke be nb tabdil shode ast
						blocklabel =  get_block_lable(labelname,&bbuf);
						if(blocklabel!=0){
							block* nb = new block(getnextbname());
							inss->push_back(nb);
							sep_block(b,j,nb);
							nb->add_new_IN(b);
							b->add_new_OUT(nb);
							blocklabel->add_new_IN(b);
							b->add_new_OUT(blocklabel);
						}else{
							blocklabel = new block(getnextbnameoutsilde());
							blocklabel->setsample();
							lable* l = new lable(labelname,blocklabel,1);
							labelname.insert(labelname.length(),":");
							blocklabel->add_new_instruction(new instruction(labelname,-1));
							inss->push_back(blocklabel);
							////////////////////////////
							block* nb = new block(getnextbname());
							inss->push_back(nb);
							sep_block(b,j,nb);
							nb->add_new_IN(b);
							b->add_new_OUT(nb);
							blocklabel->add_new_IN(b);
							b->add_new_OUT(blocklabel);
						}
						break;
				}
			}
		}
	}
}

// print inss to CMD
void CFGD::print_block_out(vector<block*> inss){
	cout<<"Start BLOCKING\n";
	for(unsigned int i=0;i<inss.size();i++)
		inss.at(i)->print_ins_out();
	cout<<"END BLOKING";
}



///delete blocks that no instructions
void CFGD::Delete_Not_Use_Block(vector<block*>* bbuf){
	for(unsigned int i=0;i<bbuf->size();i++)
		if(bbuf->at(i)->iniisemply() && !bbuf->at(i)->issample()) 
			bbuf->erase(bbuf->begin()+i);
}

//esme label ra mighirad va block ra tahvil midahad. if label dar vasataye blocky bashad on ra mishkanad va block shekaste shode ro mifreste
block* CFGD::get_block_lable(string _name,vector<block*>* inss){
	for(unsigned int i=0;i<labels.size();i++)
	{
		if(_name.compare( labels.at(i)->name)==0)
		{
			//cout<<"label "<<labels.at(i)->name<<" found in line "<<labels.at(i)->blockini<<" of "<< labels.at(i)->blockL->Bname <<"\n";
			block* b = labels.at(i)->blockL;
			if(labels.at(i)->blockini==1) // if this block is for this label return this block
				return b;
			// if the label is in the center of instruction the block must broken and make a new block that the fisrt instruction of 
			// that is label
			int j = labels.at(i)->blockini-1;
			block * nb = new block(getnextbname());
			sep_block(b,j,nb);
			inss->push_back(nb);
			nb->add_new_IN(b);
			b->add_new_OUT(nb);
			return nb;
		}
	}
	return 0;
}

/* write the inss with its blocks to file CFGoutput.txt*/
void CFGD::write_to_file(vector<block*> inss){
	ofstream out;
	out.open(fileforout);
	if (out.is_open())
	{
		out<<"Start BLOCKING\n";
		for(unsigned int i=0;i<inss.size();i++)
		{
			out<<inss.at(i)->write_to_ins_out();
		}
		out<<"END BLOKING";
		out.close();
	}
}
