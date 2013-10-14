#include "Abstract_interpretation.h"




Abstract_interpretation::Abstract_interpretation(void)
{
	setbranchkind();
	PDFNUMBER=0;
}

Abstract_interpretation::Abstract_interpretation(vector<block*>* _basicBlocks,vector<codepair*>* _opcodes)
{
	PDFNUMBER=0;
	setbranchkind();
	setOpcodes(_opcodes);
	setBasicBlocks(_basicBlocks);
}

Abstract_interpretation::~Abstract_interpretation(void)
{

}

/*
return 0 : job completed.
return 1 : if opcode is NULL.
return 2 : if opcode size less than equal ziro.
return 3 : if basicBlocks is NULL.
return 4 : if basicBlocks size less than equal ziro.
return 5 : if(basicBlocks->at(0)!=NULL &&basicBlocks->at(0)->size()<=0 )
*/
int Abstract_interpretation::start_Abstract_interpretation(){
	if(opcodes==NULL)
		return 1;
	if(opcodes->size()<=0)
		return 2;
	if(basicBlocks==NULL)
		return 3;
	if(basicBlocks->size()<=0)
		return 4;
	if(basicBlocks->at(0)==NULL &&basicBlocks->at(0)->size()<=0 )
		return 5;
	///////////////////////////////////////////////////////////////////// end checks
	vector<block*>* copyofBasicBlocks = getACopyofBasicBlocks();
	vector<block*>* for_each_ini = foreachinstructionmakeabasicblock(copyofBasicBlocks);
	Range_Analysis_(for_each_ini);

	////////////////////////////////////////////////////////////////////delete unuse memory
	for(unsigned int i=0;i<copyofBasicBlocks->size();i++)
		delete copyofBasicBlocks->at(i);
	copyofBasicBlocks->clear();
	delete copyofBasicBlocks;
	for(unsigned int i=0;i<for_each_ini->size();i++)
		delete for_each_ini->at(i);
	for_each_ini->clear();
	delete for_each_ini;
	return 0;
}

void Abstract_interpretation::Range_Analysis_(vector<block*>* for_each_ini){
	Range_Analysis* r = new Range_Analysis();
	r->setBasicBlocks(for_each_ini);
	r->DOANALYSIS();
	if(r->stateTree!=NULL)
		make_pdf(r->stateTree);
	delete r;
}

void Abstract_interpretation::make_pdf(State* statetree){
	string myDirectory="";//jai ke file exe dar anja gharar darad.
	string saveDirectory="";//mahali ke file pdf anja zakhire mishavad.
	string quary = "";
	char c[10];
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

		///////////////////////////////////////make Ghraph in to pdf in path ../pdfs/CFGi.pdf
		
		TCHAR szFileName[MAX_PATH];
		MakeGVfile(statetree);
		GetPrivateProfileString("Path","CFGPDF_PATH","",szFileName,MAX_PATH,"disassemblerconfig.ini");
		if(strcmp(szFileName,"")==0){//agar karbar masiri entekhab nakarde bashad,masire pishfarz ra entekhab mikonim:
			saveDirectory=myDirectory;
			saveDirectory.insert(saveDirectory.length(),"pdfs\\AICFG");
		}
		else{
			saveDirectory=szFileName;
			saveDirectory.insert(saveDirectory.length(),"\\");
		}
		//baraye ejraye narmafzare rasme grph va zakhire khorooji:
		quary.insert(0,"\"\"");
		quary.insert(quary.length(),"Graph\\bin\\dot.exe\" -Tpdf \"");
		quary.insert(quary.length(),myDirectory);
		quary.insert(quary.length(),"Graph\\bin\\AICFG.gv\" -o \"");
		quary.insert(quary.length(),saveDirectory);
		_itoa_s(++PDFNUMBER,c,sizeof(c),10);
		quary.insert(quary.length(),c);
		quary.insert(quary.length(),".pdf\"\"");
		system(quary.c_str());
		//open a pdf file
		quary=saveDirectory;
		quary.insert(quary.length(),c);
		quary.insert(quary.length(),".pdf");
		ShellExecute(NULL, "OPEN", quary.c_str() , NULL,NULL, SW_SHOWNORMAL);
}

void Abstract_interpretation::make_pdf(vector<block*>* b){
	string myDirectory="";//jai ke file exe dar anja gharar darad.
	string saveDirectory="";//mahali ke file pdf anja zakhire mishavad.
	string quary = "";
	char c[10];
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

		///////////////////////////////////////make Ghraph in to pdf in path ../pdfs/CFGi.pdf
		
		TCHAR szFileName[MAX_PATH];
		MakeGVfile(*b);
		GetPrivateProfileString("Path","CFGPDF_PATH","",szFileName,MAX_PATH,"disassemblerconfig.ini");
		if(strcmp(szFileName,"")==0){//agar karbar masiri entekhab nakarde bashad,masire pishfarz ra entekhab mikonim:
			saveDirectory=myDirectory;
			saveDirectory.insert(saveDirectory.length(),"pdfs\\AICFG");
		}
		else{
			saveDirectory=szFileName;
			saveDirectory.insert(saveDirectory.length(),"\\");
		}
		//baraye ejraye narmafzare rasme grph va zakhire khorooji:
		quary.insert(0,"\"\"");
		quary.insert(quary.length(),"Graph\\bin\\dot.exe\" -Tpdf \"");
		quary.insert(quary.length(),myDirectory);
		quary.insert(quary.length(),"Graph\\bin\\AICFG.gv\" -o \"");
		quary.insert(quary.length(),saveDirectory);
		_itoa_s(++PDFNUMBER,c,sizeof(c),10);
		quary.insert(quary.length(),c);
		quary.insert(quary.length(),".pdf\"\"");
		system(quary.c_str());
		//open a pdf file
		quary=saveDirectory;
		quary.insert(quary.length(),c);
		quary.insert(quary.length(),".pdf");
		ShellExecute(NULL, "OPEN", quary.c_str() , NULL,NULL, SW_SHOWNORMAL);
}

void Abstract_interpretation::MakeGVfile(State* statetree){
	string fileforgvout;
	char myPath[_MAX_PATH+1];
	GetModuleFileName(NULL,myPath,_MAX_PATH);
	fileforgvout.insert(0,myPath);
	int locationfind = fileforgvout.find_last_of('\\');
	if(locationfind >=0)
	{
		fileforgvout = fileforgvout.substr(0,locationfind+1);
	}
	fileforgvout.insert(fileforgvout.length(),GVoutfilename);
	ofstream outgv;
	outgv.open(fileforgvout);
	if (outgv.is_open())
	{
		outgv<<"digraph G {\ngraph [\nrankdir = \"TB\"\ncenter = \"true\"\n];\nnode [\nfontsize = \"16\"\nshape = \"box\"\n];\nedge [\ncolor = \"blue\"\n];\n\n";
		HDICTIONARY<State*> fortrace;
		outgv<<tracestatesforMakeGVfile(statetree,&fortrace,NULL);

		/////////////end
		outgv<<"\n}";
		outgv.close();
	}
}

string Abstract_interpretation::tracestatesforMakeGVfile(State* statetree,HDICTIONARY<State*>* fortrace,State* parrent){
	if(!fortrace->find(statetree->block->getname())){
		fortrace->add(statetree->block->getname(),new HDICTIONARYNODEKEY<State*>(parrent));
		string s = statetree->block->StringForGV();
		string edges = "";
		for(unsigned int i=0;i<statetree->nextstates.size();i++)
		{
			s.insert(s.length(),tracestatesforMakeGVfile(statetree->nextstates[i],fortrace,statetree));
			edges.insert(edges.length(),"node");
			edges.insert(edges.length(),statetree->block->getname().substr(2));
			edges.insert(edges.length()," -> node");
			edges.insert(edges.length(),statetree->nextstates[i]->block->getname().substr(2));
			edges.insert(edges.length()," [layer = \"all\", label = \"");
			edges.insert(edges.length(),statetree->block->getOUTTEXT()->at(i));
			if(statetree->getenv()!=NULL){
				edges.insert(edges.length(),"\\n");
				if(statetree->block->blocktype !=3)
					edges.insert(edges.length(),statetree->getenv()->INFOforGVfile());
				else {
					if(statetree->getstateKind() == -1) {edges.insert(edges.length(),"Never coming to this path.");}
					else if(statetree->getstateKind() == 1 && (statetree->getlaststateKind() ==1)){edges.insert(edges.length(),statetree->getenv()->INFOforGVfile());}
					else if(statetree->getstateKind() == 1 && statetree->getlaststateKind() ==-1){
						block* succ_t = statetree->block->succ_t()->at(0);
						if(statetree->nextstates[i]->block->getname().compare(succ_t->getname()) ==0 ) 
						edges.insert(edges.length(),statetree->getenv()->INFOforGVfile());
						else edges.insert(edges.length(),"Never coming to this path.");
					}
					else if(statetree->getstateKind() == 0 && statetree->getlaststateKind() ==-1) {
						block* succ_f = statetree->block->succ_f()->at(0);
						if(statetree->nextstates[i]->block->getname().compare(succ_f->getname()) ==0 ) 
						edges.insert(edges.length(),statetree->getenv()->INFOforGVfile());
						else edges.insert(edges.length(),"Never coming to this path.");
					}
					else if(statetree->getstateKind() == 0 && statetree->getlaststateKind() ==1) {
						block* succ_f = statetree->block->succ_f()->at(0);
						if(statetree->nextstates[i]->block->getname().compare(succ_f->getname()) ==0 ) 
							edges.insert(edges.length(),statetree->getenv()->INFOforGVfile());
						else edges.insert(edges.length(),statetree->getlastenv()->INFOforGVfile());
					}
					else if(statetree->getstateKind() == 1 && statetree->getlaststateKind() ==0) {
						block* succ_t = statetree->block->succ_t()->at(0);
						if(statetree->nextstates[i]->block->getname().compare(succ_t->getname()) ==0 ) 
							edges.insert(edges.length(),statetree->getenv()->INFOforGVfile());
						else edges.insert(edges.length(),statetree->getlastenv()->INFOforGVfile());
					}
					else if(statetree->getstateKind() == 0 && statetree->getlaststateKind() ==0) {edges.insert(edges.length(),statetree->getenv()->INFOforGVfile());}
					else{edges.insert(edges.length(),statetree->getenv()->INFOforGVfile());}
				}
			}
			edges.insert(edges.length(),"\"];\n");
		}
		s.insert(s.length(),edges);
		return s;
	}
	return "";
}

void Abstract_interpretation::MakeGVfile(vector<block*> inss){
	string fileforgvout;
	char myPath[_MAX_PATH+1];
	GetModuleFileName(NULL,myPath,_MAX_PATH);
	fileforgvout.insert(0,myPath);
	int locationfind = fileforgvout.find_last_of('\\');
	if(locationfind >=0)
	{
		fileforgvout = fileforgvout.substr(0,locationfind+1);
	}
	fileforgvout.insert(fileforgvout.length(),GVoutfilename);
	ofstream outgv;
	outgv.open(fileforgvout);
	if (outgv.is_open())
	{
		outgv<<"digraph G {\ngraph [\nrankdir = \"TB\"\ncenter = \"true\"\n];\nnode [\nfontsize = \"16\"\nshape = \"box\"\n];\nedge [\ncolor = \"blue\"\n];\n\n";

		///////////////////////////////make node
		for(unsigned int i=0;i<inss.size();i++)
		{
			outgv<<inss.at(i)->StringForGV();
		}

		///////////////////make edge
		for(unsigned int i=0;i<inss.size();i++)
		{
			vector<block*>* _out = inss.at(i)->getOUT();
			vector<string>* _outtext = inss.at(i)->getOUTTEXT();
			for(unsigned int j=0;j<_out->size();j++)
			{
				outgv<<"node"<<inss.at(i)->getname().substr(2)<<" -> node"<<_out->at(j)->getname().substr(2)<<" [layer = \"all\", label = \""<<_outtext->at(j)<<"\"];\n";
			}
		}

		/////////////end
		outgv<<"\n}";
		outgv.close();
	}
}

vector<block*>* Abstract_interpretation::getACopyofBasicBlocks(){
	vector<block*>* ret = new vector<block*>();
	for(unsigned int i=0;i<this->basicBlocks->size();i++)
		ret->push_back(basicBlocks->at(i)->Copy_with_ini_Copy());

	for(unsigned int i=0;i<ret->size();i++)
	{
		block* b = ret->at(i);
		for(unsigned int j =0;j<b->getIN()->size();j++)
		{
			for(unsigned int k=0;k<ret->size();k++)
				if(ret->at(k)->getname().compare(b->getIN()->at(j)->getname())==0)
				{
						b->getIN()->insert(b->getIN()->begin() +j,ret->at(k));
						b->getIN()->erase((b->getIN()->begin()+j+1));
				}
		}

		for(unsigned int j =0;j<b->getOUT()->size();j++){
			if(b->getOUTTEXT()->at(j).compare("continue code")==0)
			{
				block* bdeleted = b->getOUT()->at(j);
				for(unsigned int w=0;w<ret->size();w++)
				{
					if(ret->at(w)->getname().compare(bdeleted->getname())==0){
						bdeleted = ret->at(w);
						b->getOUTTEXT()->erase((b->getOUTTEXT()->begin()+j));
						b->getOUT()->erase((b->getOUT()->begin()+j));
						j--;
						for(unsigned int p=0;p<bdeleted->getIN()->size();p++){
							if(bdeleted->getIN()->at(p)->getname().compare(b->getname())==0)
							{
								bdeleted->getIN()->erase(bdeleted->getIN()->begin()+p);
							}
						}
						break;
					}
				}
			}
		}

		for(unsigned int j =0;j<b->getOUT()->size();j++)
		{
			for(unsigned int k=0;k<ret->size();k++)
				if(ret->at(k)->getname().compare(b->getOUT()->at(j)->getname())==0)
				{
					b->getOUT()->insert(b->getOUT()->begin() +j,ret->at(k));
					b->getOUT()->erase((b->getOUT()->begin()+j+1));
				}
		}
	}

	return ret;
}

/*
for each instrunction in basic blocks make a seprate basic block
*/
vector<block*>* Abstract_interpretation::foreachinstructionmakeabasicblock(vector<block*>* basicBlocks){
	vector<block*>* ret = new vector<block*>();
	vector<string> firstlabel;
	block *lastb = NULL; // مقدار بلاک اولیه ساخته شده قبلی را در نظر می گیرد.
	for(unsigned int i=0;i<basicBlocks->size();i++) /// برای هر بلاک اولیه
	{
		block* p = basicBlocks->at(i); // بلاک اولیه مورد نظر را در پی قرار می دهد
		for(int j =0 ;j<p->size();j++) // هر خط دستور در این بلاک در این حقله قرار می گیرد.
		{
			if(p->init(j)->getinstructin().find("add byte ptr [eax] , al")!=string::npos &&  p->size()>50){ // برای اینکه داده ها را از این بلاک حذف کنیم این را استفاده می کند.
				continue;
			}
			//////////////////////////////////////////make block name 
			// قرار است که درایجا هر یک از این بلاک ها به تعداد دستوری که دارد یک بلاک جدید ایجاد کند در این قسمت برای این مورد یک بلاک جدید ایجاد می کنیم.
			string blockname = p->getname();
			blockname.insert(blockname.length(),"_");
			char c[20];
			_itoa_s((j+1),c,sizeof(c),10);
			blockname.insert(blockname.length(),c);
			block * b = new block(blockname);
			/////////////////////////////////////////block maked;
			instruction * ini = p->init(j)->Copy(); // یک کپی از این خط دستور را استفاده می شود
			int index ;
			ini->gotowhere(branches,bnum,&index);
			b->add_new_instruction(ini); // این دستور را در بلاک قرار می دهیم.
			if(j==0 && p->size()-1 ==0){ // اگر اولی و آخری مورد بود
				for(unsigned int k=0;k<p->getIN()->size();k++)
					b->add_new_IN(p->getIN()->at(k));
				firstlabel.push_back(b->getname());
				for(unsigned int k=0;k<p->getOUT()->size();k++)
					b->add_new_OUT(p->getOUT()->at(k),p->getOUTTEXT()->at(k));
				if(lastb!=NULL){
					lastb->add_new_OUT(b,CONTINUE);
					b->add_new_IN(lastb);
				}
			}
			else if(j==0) // اگر اولین دستور بود
			{
				for(unsigned int k=0;k<p->getIN()->size();k++)
					b->add_new_IN(p->getIN()->at(k));
				firstlabel.push_back(b->getname());
			}
			else if(j == p->size()-1) // اگر آخرین دستور بود.
			{
				for(unsigned int k=0;k<p->getOUT()->size();k++)
					b->add_new_OUT(p->getOUT()->at(k),p->getOUTTEXT()->at(k));
				lastb->add_new_OUT(b,CONTINUE);
				b->add_new_IN(lastb);
			}
			else{ // اگر اولین و آخرین دستور نباشد در این صورت دستورات وسطی هستند.
				lastb->add_new_OUT(b,CONTINUE);
				b->add_new_IN(lastb);
			}
			
			//if(index<0){ // اگر index بزرگتر از صفر بوده یعنی این دستور شرطی است و در این قسمت دستورات شرطی باید حذف شوند.
				ret->push_back(b);
				lastb = b;
			/*}else {
				if(lastb!=NULL){
					lastb->remove_OUT(b->getname());
					b->Copyoutsinto(lastb);
					if(j==0){
						firstlabel.pop_back();
						firstlabel.push_back(lastb->getname());
					}
					if(sharty==this->getbranchkind(index))
					{
						string s = " ;@j ";// after change this text you must change the size of this in "s.insert(4,..." in next code.
						s.insert(5,b->init(0)->opcode);
						lastb->init(0)->addstringtoendofini(s);
					}
				}
			}*/
		}
	}

	for(unsigned int i=0;i<ret->size();i++)// در این قسمت out ها را درست می کنیم.
	{
		block* b = ret->at(i);
		for(unsigned int j=0;j<b->getOUT()->size();j++){
			if(string :: npos == b->getOUT()->at(j)->getname().find("_"))
			{
				string s = b->getOUT()->at(j)->getname();
				int value = -1;
				for(unsigned int u=0;u<basicBlocks->size();u++)
				{
					if(basicBlocks->at(u)->getname().compare(s)==0){
						value = u;
						break;
					}
				}
				if(value>=0){
					string newname = firstlabel.at(value);
					for(unsigned int r=0;r<ret->size();r++){
						if(ret->at(r)->getname().compare(newname)==0)
						{
							b->getOUT()->insert(b->getOUT()->begin()+j,ret->at(r));
							b->getOUT()->erase(b->getOUT()->begin()+j+1);
							break;
						}
					}
				}
			}
		}
	}

	for(unsigned int i=0;i<ret->size();i++)//در این قسمت in ها را درست می کنیم
	{
		block* b = ret->at(i);
		for(unsigned int j=0;j<b->getIN()->size();j++){
			if(string :: npos == b->getIN()->at(j)->getname().find("_"))
			{
				string s = b->getIN()->at(j)->getname();
				int value = -1;
				for(unsigned int u=0;u<basicBlocks->size();u++)
				{
					if(basicBlocks->at(u)->getname().compare(s)==0){
						value = u;
						break;
					}
				}
				if(value>=0){
					string newname = firstlabel.at(value);
					for(unsigned int r=0;r<ret->size();r++){
						if(ret->at(r)->getname().compare(newname)==0)
						{
							b->getIN()->erase(b->getIN()->begin()+j);
							b->add_new_IN(ret->at(r));
							j--;
							break;
						}
					}
				}
			}
		}
	}
	 
	// در این قسمت جانکشن ها را درست می کنیم.
	firstlabel.clear();
	int tempblocknumber=0;
	int ret_size = ret->size();
	for(int i=0;i<ret_size;i++){
		if(ret->at(i)->getIN()->size()>1){
			tempblocknumber++;
			char c[20];
			_itoa(tempblocknumber,c,10);
			string blockname = "Junction";
			blockname.insert(blockname.length(),c);
			block* temp = new block(blockname);
			ret->at(i)->Copyinsinto(temp);
			ret->at(i)->clearin();
			ret->at(i)->add_new_IN(temp);
			temp->add_new_OUT(ret->at(i),"continue");
			ret->push_back(temp);
		}
	}

	//// بلاک شروع را در اول قرار می دهیم.
	block* start = new block("BB0");
	ret->push_back(start);
	if(ret->size()>1){
		start->add_new_OUT(ret->at(0),"continue");
		ret->at(0)->add_new_IN(start);
	}

	// حالت های بلاک ها را تعیین می کنیم.
	int exit_block_number = 0;
	for(unsigned int i=0;i<(ret->size()-exit_block_number);i++)
	{
		if(ret->at(i)->pred()->size()==1 && ret->at(i)->succ()->size()==1)
			ret->at(i)->blocktype = 2; // assignment
		else if(ret->at(i)->pred()->size()==0 && ret->at(i)->succ()->size()==1)
			ret->at(i)->blocktype = 1; //entry point
		else if(ret->at(i)->pred()->size()==1 && ret->at(i)->succ()->size()==2)
			ret->at(i)->blocktype = 3; //tests
		else if(ret->at(i)->pred()->size()>1 && ret->at(i)->succ()->size()==1)
			ret->at(i)->blocktype = 4; //junctions
		else if(ret->at(i)->pred()->size()==1 && ret->at(i)->succ()->size()==0)
		{
			//exit
			/*این بلاک بلاک خروج است و برای انجام این کار باید یک بلاک خالی خروج ایجاد کرد که این کار را انجام دهد. و این بلاک نیز جزء بلاک های محاسباتی اعمال می شود.*/
			string blockname = "exit";
			char c[18];
			_itoa(++exit_block_number,c,10);
			blockname.insert(4,c);
			block* exit = new block(blockname);
			exit->add_new_IN(ret->at(i));
			exit->blocktype = 5;
			ret->at(i)->add_new_OUT(exit,"countinue");
			ret->at(i)->blocktype = 2; 
			ret->push_back(exit);
		}
	}
	return ret;
}

/**
*\return kid of jums, is call or other kind.
* it return sharty, ghyresharty, ret, call, notfound kinds
*/
brancheskind Abstract_interpretation::getbranchkind(int index){
	if(index == 10)
		return ret;
	else if(index == 1)
		return call;
	else if(index >=0 && index <=3)
		return ghyresharty;
	else if(index >=4 && index <=38) return sharty;
	return notfind;
}

void Abstract_interpretation::setbranchkind(){
		bnum = 39;
		branches = new string[bnum];
		branches[10] = "ret";
		branches[1] = "call";
		branches[0] = "jump";//no sharty
		branches[2] = "jmp";//no sharty
		branches[3] = "goto"; //no sharty

		branches[4] = "jecxz";// sharty
		branches[5] = "jne";// sharty
		branches[6] = "jle";// sharty
		branches[7] = "jl";// sharty
		branches[8] = "jb";// sharty
		branches[9] = "jz";// sharty
		branches[11] = "jnz";// sharty
		branches[12] = "jnle";// sharty
		branches[13] = "jnge";// sharty
		branches[14] = "jnae";// sharty
		branches[15] = "jna";// sharty
		branches[16] = "jnc";// sharty
		branches[17] = "jcxz";// sharty
		branches[18] = "jns";// sharty
		branches[19] = "jge";// sharty
		branches[20] = "jng";// sharty
		branches[21] = "jnl";// sharty
		branches[22] = "jg";// sharty
		branches[23] = "js";// sharty
		branches[24] = "jae";// sharty
		branches[25] = "jnbe";// sharty
		branches[26] = "ja";// sharty
		branches[27] = "jnb";// sharty
		branches[28] = "jbe";// sharty
		branches[29] = "loop";// sharty
		branches[30] = "rep";//sharty
		branches[31] = "jc";// sharty
		branches[32] = "je";// sharty
		branches[33] = "jpe";// sharty
		branches[34] = "jpo";// sharty
		branches[35] = "jnp";// sharty
		branches[36] = "jno";// sharty
		branches[37] = "jp";// sharty
		branches[38] = "jo";// sharty
}

void Abstract_interpretation::setBasicBlocks(vector<block*>* _basicBlocks){
	this->basicBlocks = _basicBlocks;
}

void Abstract_interpretation::setOpcodes(vector<codepair*>* _opcodes){
	this->opcodes = _opcodes;
}