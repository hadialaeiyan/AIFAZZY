
#include "FGD.h"

bool draw_cfg;//moteghaieri ke neshan midahad aya cfg bayad rasm shavad ya kheir.
bool cfg_pdf;
bool cfg_openpdf;

CFGD::CFGD(void)
{
	setcurrent_env_path();
	stack = new vector<string>();
	Bnamecounter =0;
	bnum = 39;
	templatevariablenamenumber=templatepointervariablenumber=0;
	wtofile = true ;
	stocons = false;
	branchesset = false;
	codesset = false;
	makefilenameforout();
	makefilenameforgvout();
	loadOPCodes();
}

CFGD::~CFGD(void)
{
	clear();
}

void CFGD::getallbb(char* return_value){
	string out;
	vector<block*> inss = bbuf;
	out.insert(out.length(),"Start BLOCKING\n");
	for(unsigned int i=0;i<inss.size();i++)
	{
		out.insert(out.length(),inss.at(i)->write_to_ins_out());
	}
	out.insert(out.length(),"END BLOKING");
	strcpy(return_value,out.c_str());
}

vector<string> CFGD:: split_string(string s,char c){
	vector<string> data;
	unsigned int lastindex = 0;
	unsigned int i = 0;
	for (; i < s.length(); i++)
		if (s[i] == c)
		{
			string d = s.substr(lastindex, i - lastindex);
			if(d.compare("")!=0){
				data.push_back(d);
			}
			lastindex = i + 1;
		}
	data.push_back(s.substr(lastindex, i - lastindex));
	return data;
}

string CFGD::maketruelable(string s){
	if(s.length()>1 && ( s[s.length()-1]=='\r' && (s[s.length()-2]=='h' || s[s.length()-2]=='H')))
		return s.substr(0,s.length()-2);
	else if(s.length()>1 && ( s[s.length()-1]==' ' && (s[s.length()-2]=='h' || s[s.length()-2]=='H')))
		return s.substr(0,s.length()-2);
	else if(s.length()>0 && ( s[s.length()-1]=='h' || s[s.length()-1]=='H'))
		return s.substr(0,s.length()-1);
	return s;
}

vector<block*> CFGD::getindirectjumpblocks(vector<block*> bbuf){
	vector<block*> result;
	for(unsigned int i=0;i<bbuf.size();i++)
		if(bbuf.at(i)->issample()&&!isID(bbuf.at(i)->init(0)->getlablename())){
			result.push_back(bbuf.at(i));
		}
	return result;
}

bool  CFGD::isID(string s) {
        if (s.length() == 8) {
			char c;
            for (unsigned int i = 0; i < s.length(); i++) {
                c = s[i];
                if (!((c >= '0' && c <= '9' )||(c>='A'&& c<='Z')||(c>='a'&& c<='z') )) {
                    return false;
				}
            }
			return true;
		}
		else if (s.length() == 9) {
			char c;
            for (unsigned int i = 0; i < s.length()-1; i++) {
                c = s[i];
                if (!((c >= '0' && c <= '9' )||(c>='A'&& c<='Z')||(c>='a'&& c<='z') )) {
                    return false;
				}
            }
			if(s[s.length()-1] =='h')
				return true;
		}
        return false;
}

vector<string>  CFGD:: split_string_with_E(string s){
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

vector<string> CFGD::Get_in_prt_block_valuse(string s){
	vector<string> E;
	int l1 = s.find('['),l2=s.find(']');
		if(l1!= string::npos && l2!= string::npos){
			E = split_string_with_E(s.substr(l1+1,l2-l1-1));
		}
		return E;
}

bool CFGD::ispointertomemory(string label){
	int location = label.find("ptr");
	if(location != string::npos)
	{
		return true;
	}
	return false;
}

bool CFGD::checked_this_block_in_Get_pointer_definition(block* B){
	for(unsigned int j=0;j<bbchecked.size();j++)
		if(bbchecked.at(j)->getname().compare(B->getname())==0)
		{
			return true;
		}
	return false;
}

vector<block*> CFGD::get_pointer_definition(string address,block* B){////////// باید چک شود برای اولین باری که در حال آمدن است  بافر چک ها خالی باشد.
	vector<block*> bb;
	if(!checked_this_block_in_Get_pointer_definition(B)){
		bbchecked.push_back(B);
		vector<block*>* INS = B->getIN();
		for(unsigned int i=0;i<INS->size();i++)
		{
			vector<block*> bs = get_pointer_definition(address,INS->at(i));
			for(unsigned int i=0;i<bs.size();i++)
			{
				bool find_ = false;
				for(unsigned int j=0;j<bb.size();j++)
					if(bb.at(j)->getname().compare(bs.at(i)->getname())==0)
					{
						find_ = true;
						break;
					}
				if(!find_){
					bb.push_back(bs.at(i));
				}
			}
		}

		if(B->is_def_a_ptr_with_address(address)>=0){
			bb.push_back(B);
		}
	}
	return bb;
}

string CFGD::value_of_memory(string address,block* B){
	return "NULL";
}

string CFGD::FindlastDefptr(string address,block* B){
	bbchecked.clear();
	vector<block*> succ_def_address = get_pointer_definition(address,B);
	if(succ_def_address.size()==1){
		int index = succ_def_address.at(0)->is_def_a_ptr_with_address(address);
		instruction* ini_def = succ_def_address.at(0)->init(index);
	}
	return "NULL";
}

void CFGD::make_call_flow_graph_pdf(){
	//make a file in path fileforgvout
	//write to the gv files
	//call dot.exe to make a pdf
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

	if(cfg_pdf==true ){//agar gharar bashad file pdf zakhire konim.
		///////////////////////////////////////make Ghraph in to pdf in path ../pdfs/CFGi.pdf
		
		TCHAR szFileName[MAX_PATH];
		//MakeGVfile(bbuf);
		GetPrivateProfileString("Path","CFGPDF_PATH","",szFileName,MAX_PATH,"disassemblerconfig.ini");
		if(strcmp(szFileName,"")==0){//agar karbar masiri entekhab nakarde bashad,masire pishfarz ra entekhab mikonim:
			saveDirectory=myDirectory;
			saveDirectory.insert(saveDirectory.length(),"pdfs\\CallFG");
		}
		else{
			saveDirectory=szFileName;
			saveDirectory.insert(saveDirectory.length(),"\\");
		}
		//baraye ejraye narmafzare rasme grph va zakhire khorooji:
		quary.insert(0,"\"\"");
		quary.insert(quary.length(),"Graph\\bin\\dot.exe\" -Tpdf \"");
		quary.insert(quary.length(),myDirectory);
		quary.insert(quary.length(),"Graph\\bin\\CallFG.gv\" -o \"");
		quary.insert(quary.length(),saveDirectory);
		_itoa_s(++pdfnumbercfg,c,sizeof(c),10);
		quary.insert(quary.length(),c);
		quary.insert(quary.length(),".pdf\"\"");

		system(quary.c_str());
	
		if(cfg_openpdf){//agar bayad file pdf ra ejra konim.
			//open a pdf file
			quary=saveDirectory;
			quary.insert(quary.length(),c);
			quary.insert(quary.length(),".pdf");
			ShellExecute(NULL, "OPEN", quary.c_str() , NULL,NULL, SW_SHOWNORMAL);
		}
	}
}

string CFGD::find_value_of_ptr(string indirto,block* B){
	string valueofpointer = FindlastDefptr(indirto,B);
	return value_of_memory(valueofpointer,B);
}

vector<string> CFGD::get_next_address(){
	vector<string> s;
	for(unsigned int i=0;i<bbuf.size();i++)
		if(bbuf.at(i)->issample()&&isID(bbuf.at(i)->init(0)->getlablename())){
			s.push_back(bbuf.at(i)->getlablename());
		}
	return s;
}

long CFGD::StringHexTointConverter(string hex) {
	if(hex[hex.length()-1]=='h'||hex[hex.length()-1]=='H')
		hex = hex.substr(0,hex.length()-1);
    char * p;
    long n = strtol( hex.c_str(), & p, 16 );
    if ( * p != 0 ) { //my bad edit was here
        cout << "not a number" << endl;
    }
	return n;
}

string CFGD::isinIAT(string hex){
	int value = this->StringHexTointConverter(hex);
	for(unsigned int i=0;i<this->dlimdlls.size();i++){
		IMDLLS* DLL = this->imdlls.at(i);
		for(unsigned int j=0;j<DLL->functions.size();j++){
			int offset = this->StringHexTointConverter(DLL->offsets.at(j));
			if(offset==value){
				string ret = DLL->functions.at(j);
				ret.insert(ret.length(),"@");
				ret.insert(ret.length(),DLL->dllname);
				return ret;
			}
		}
	}

	for(unsigned int i=0;i<this->imdlls.size();i++)
	{
		IMDLLS* DLL = this->imdlls.at(i);
		int offset = this->StringHexTointConverter(DLL->dllIAT);
		if(offset<=value){
			int index = (value-offset)/4;
			if(index<DLL->functions.size())
			{
				string ret = DLL->functions.at(index);
				ret.insert(ret.length(),"@");
				ret.insert(ret.length(),DLL->dllname);
				return ret;
			}
		}
	}
	return "NULL";
}

void CFGD::setcurrent_env_path(){
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
	this->current_env_path = myDirectory;
}

void CFGD::findIndirectjumsPlace( vector<block*> indirectjumps){
	for(unsigned int i=0;i<indirectjumps.size();i++){
		block* B = indirectjumps.at(i);
		string label = B->getlablename();
		if(ispointertomemory(label)){
			vector<string> E = Get_in_prt_block_valuse(label);
			switch (E.size())
			{
			case 1: ///////////////////// Dword ptr [0040F254h]
				if(isID(E.at(0))){
					int l1 = label.find('['),l2=label.find(']');
					if(l1!= string::npos && l2!= string::npos){
						string indirto =label.substr(l1+1,l2-l1-1);
						string value = isinIAT(indirto);
						if(value.compare("") != 0 && value.compare("NULL") != 0)
							B->change_label_with(value);
					}
				}
				break;
			default:
				break;
			}
		}
	}
}

/**
این تابع برای دریافت مقدار رجیسترها در ابتدای پردازه است.
*/
void CFGD::load_INfirst_registers()
{
	STARTUPINFO si;
	ZeroMemory( &si, sizeof(si) );
    si.cb  = sizeof(si);
	PROCESS_INFORMATION pi;
    ZeroMemory( &pi, sizeof(pi) );
	DWORD lastErr;
	CONTEXT t={0};
	if(!CreateProcess(exefilename.c_str(),NULL,NULL,NULL,FALSE,DEBUG_PROCESS+ DEBUG_ONLY_THIS_PROCESS+NORMAL_PRIORITY_CLASS,NULL,NULL,&si,&pi))
	{
		lastErr=GetLastError();
		return;
	}
	DEBUG_EVENT dbgr;
	if(WaitForDebugEvent(&dbgr,INFINITE))
	{
		if(dbgr.dwDebugEventCode==CREATE_PROCESS_DEBUG_EVENT)
		{
			t.ContextFlags=CONTEXT_FULL;
			GetThreadContext(pi.hThread,&t);
			Register* r = new Register();
			r->registername = "eax";
			r->value = int_to_hex(t.Eax);
			registertabel.push_back(r);
			r = new Register();
			r->registername = "eflags";
			r->value = int_to_hex(t.EFlags);
			registertabel.push_back(r);
			r = new Register();
			r->registername = "esi";
			r->value = int_to_hex(t.Esi);
			registertabel.push_back(r);
			r = new Register();
			r->registername = "edx";
			r->value = int_to_hex(t.Edx);
			registertabel.push_back(r);
			r = new Register();
			r->registername = "ebx";
			r->value = int_to_hex(t.Ebx);
			registertabel.push_back(r);
			r = new Register();
			r->registername = "ecx";
			r->value = int_to_hex(t.Ecx);
			registertabel.push_back(r);
			r = new Register();
			r->registername = "edx";
			r->value = int_to_hex(t.Edx);
			registertabel.push_back(r);
			r = new Register();
			r->registername = "esp";
			r->value = int_to_hex(t.Esp);
			registertabel.push_back(r);
			r = new Register();
			r->registername = "eip";
			r->value = int_to_hex(t.Eip);
			registertabel.push_back(r);
			r = new Register();
			r->registername = "ebp";
			r->value = int_to_hex(t.Ebp);
			registertabel.push_back(r);
			r = new Register();
			r->registername = "edi";
			r->value = int_to_hex(t.Edi);
			registertabel.push_back(r);
		}
	}
}

long CFGD::toint(string s){
	return atol(s.c_str());
}

std::string CFGD::int_to_hex( long i )
{
  stringstream s;
  s << std::setfill ('0') << std::setw(sizeof(int)*2) 
         << std::hex << i;
  string str = s.str();
  transform(str.begin(), str.end(),str.begin(), ::toupper);
  return str;
}

/*copy in to file ACM.txt and call DOCFG.*/
void CFGD::DOCFG(const char* in,bool make_pdf,string _exefilename){
	this->exefilename = _exefilename;
	string s = in;
	bbuf.clear();
	labels.clear();
	registertabel.clear();
	variabletabel.clear();
	vector<string> inp = split_string(s,'\n');
	load_INfirst_registers();													//load fir time register
	//////load first block list.
	/// in this part the proc... try to read line by line from
	/// file and save it in memmory in first block that it's name is main
	block * newblock = new block(getnextbname());								//init first block
	bbuf.push_back(newblock);													//push first block to block lists.
	int max_line_nnumner = inp.size()-1;										//save max line number and set inp.size()-1 beca.. the inp.size()-1 th element of string is empty ("");
	for(unsigned int linenumber = 0; linenumber<max_line_nnumner;)
	{
		string line = inp.at(linenumber);
		if(line.length() > 0)													//if the line is not empty, the line add to the first block
		{
			linenumber++; 														// enter new line.
			instruction* ins = new instruction(line,linenumber,codes,&variabletabel,&registertabel,&templatevariablenamenumber,&templatepointervariablenumber,stack);				// define new instruction and set its line number.
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
	
	vector<block*> indirectjumps = getindirectjumpblocks(bbuf);
	if(indirectjumps.size()>0){
		LoadIATCALLS();
	}
	findIndirectjumsPlace(indirectjumps);

	Abstract_interpretation * ai = new Abstract_interpretation(&(this->bbuf),this->codes);
	ai->start_Abstract_interpretation();

	delete ai;
	//Delete_Not_Use_Block(&bbuf); // in blocking the ASM code some of block in progressing make that have not any instruction. that must be delete.
	
	if(wtofile)
		write_to_file(bbuf); // write the result to file
	if(stocons){
		cout<<"///////////////result\\\\\\\\\\\\\\\\";
		print_block_out(bbuf); // print in CMD 
	}
}

string CFGD::delete_space_in_first_of_string(string s){
	while (s.at(0) == ' '){
		s = s.substr(1);
	}
	return s;
}

/**
این قطعه کد برای نتظیم گرفتن جدول 
IAT 
با استفاده از 
dumpbin.exe
بدست می آورد.
این آن برنامه را صدا می زند و خروجی را که در فایل 
import.txt 
وجود دارد را می خواند و نتیجه را در  
imdlls
میریزد.
*/
void CFGD::LoadIATCALLS(){
	string importfilepath = this->current_env_path;
	importfilepath.insert(importfilepath.length(),"IMPORTS.txt");
	remove(importfilepath.c_str());
	string s = this->current_env_path;
	s.insert(s.length(),"dumpbin.exe /OUT:\"");
	s.insert(s.length(),importfilepath);
	s.insert(s.length(),"\" /IMPORTS ");
	s.insert(s.length(),"\"");
	s.insert(s.length(),exefilename);
	s.insert(s.length(),"\"");
	system(s.c_str());
	std::ifstream input(importfilepath.c_str());
	std::string line;
	if (input.is_open())
	{
		while (std::getline(input, line)) {
			if (line.find("Section contains the following imports:") != string::npos){
				break;
			}
		}
		IMDLLS* DLLS = NULL;
		bool finish = false;
		bool section_delay_load_imports = false;
		while (!finish && std::getline(input, line))
		{
			if (line.compare("") == 0){
				if (DLLS != NULL){
					this->imdlls.push_back(DLLS);
				}
				std::getline(input, line);
				if(line.find("Section contains the following delay load imports:")!= string::npos){
					section_delay_load_imports = true;
					break;
				}
				if (line.compare("  Summary") != 0){
					line = delete_space_in_first_of_string(line);
					DLLS = new IMDLLS();
					DLLS->dllname = line;
					std::getline(input, line);
					line = delete_space_in_first_of_string(line);
					vector<string> s = split_string(line, ' ');
					if (s.size() == 4){
						DLLS->dllIAT = s.at(0);
					}
					std::getline(input, line);
					std::getline(input, line);
					std::getline(input, line);
					std::getline(input, line);
				}
				else{
					finish = true;
				}
			}
			else{
				line = delete_space_in_first_of_string(line);
				vector<string> s = split_string(line, ' ');
				if (s.size() == 2 && s.at(0).compare("Ordinal") != 0)
				{
					DLLS->functions.push_back(s.at(1));
					DLLS->offsets.push_back(s.at(0));
				}else{
					DLLS->functions.push_back("error on read");
					DLLS->offsets.push_back("-1");
				}
			}
		}
		if(section_delay_load_imports){
			DLLS = NULL;
			while (!finish && std::getline(input, line))
			{
				if (line.compare("") == 0){
					if (DLLS != NULL){
						this->dlimdlls.push_back(DLLS);
					}
					std::getline(input, line);
					if (line.compare("  Summary") != 0){
						line = delete_space_in_first_of_string(line);
						DLLS = new IMDLLS();
						DLLS->dllname = line;
						std::getline(input, line);
						std::getline(input, line);
						std::getline(input, line);
						line = delete_space_in_first_of_string(line);
						vector<string> s = split_string(line, ' ');
						if (s.size() == 4){
							DLLS->dllIAT = s.at(0);
						}
						std::getline(input, line);
						std::getline(input, line);
						std::getline(input, line);
						std::getline(input, line);
						std::getline(input, line);
					}
					else{
						finish = true;
					}
				}
				else{
					line = delete_space_in_first_of_string(line);
					vector<string> s = split_string(line, ' ');
					if (s.size() == 3)
					{
						DLLS->functions.push_back(s.at(2));
						DLLS->offsets.push_back(s.at(0));
					}else{
						DLLS->functions.push_back("error on read");
						DLLS->offsets.push_back("-1");
					}
				}
			}
		}
	}
	input.close();
}

void CFGD::make_jpg(){
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

	if(cfg_pdf==true ){//agar gharar bashad file pdf zakhire konim.
		///////////////////////////////////////make Ghraph in to pdf in path ../pdfs/CFGi.pdf
		
		TCHAR szFileName[MAX_PATH];
		MakeGVfile(bbuf);
		GetPrivateProfileString("Path","CFGPDF_PATH","",szFileName,MAX_PATH,"disassemblerconfig.ini");
		if(strcmp(szFileName,"")==0){//agar karbar masiri entekhab nakarde bashad,masire pishfarz ra entekhab mikonim:
			saveDirectory=myDirectory;
			saveDirectory.insert(saveDirectory.length(),"pdfs\\CFG");
		}
		else{
			saveDirectory=szFileName;
			saveDirectory.insert(saveDirectory.length(),"\\");
		}
		//baraye ejraye narmafzare rasme grph va zakhire khorooji:
		quary.insert(0,"\"\"");
		quary.insert(quary.length(),"Graph\\bin\\dot.exe\" -Tbmp \"");
		quary.insert(quary.length(),myDirectory);
		quary.insert(quary.length(),"Graph\\bin\\CFG.gv\" -o \"");
		quary.insert(quary.length(),saveDirectory);
		_itoa_s(++pdfnumbercfg,c,sizeof(c),10);
		quary.insert(quary.length(),c);
		quary.insert(quary.length(),".bmp\"\"");

		system(quary.c_str());
	
		if(cfg_openpdf){//agar bayad file pdf ra ejra konim.
			//open a pdf file
			quary=saveDirectory;
			quary.insert(quary.length(),c);
			quary.insert(quary.length(),".bmp");
			ShellExecute(NULL, "OPEN", quary.c_str() , NULL,NULL, SW_SHOWNORMAL);
		}
	}
}

///////////////////////////////////////make Ghraph in to pdf in path ../pdfs/CFGi.pdf
void CFGD::make_pdf(){
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

	if(cfg_pdf==true ){//agar gharar bashad file pdf zakhire konim.
		///////////////////////////////////////make Ghraph in to pdf in path ../pdfs/CFGi.pdf
		
		TCHAR szFileName[MAX_PATH];
		MakeGVfile(bbuf);
		GetPrivateProfileString("Path","CFGPDF_PATH","",szFileName,MAX_PATH,"disassemblerconfig.ini");
		if(strcmp(szFileName,"")==0){//agar karbar masiri entekhab nakarde bashad,masire pishfarz ra entekhab mikonim:
			saveDirectory=myDirectory;
			saveDirectory.insert(saveDirectory.length(),"pdfs\\CFG");
		}
		else{
			saveDirectory=szFileName;
			saveDirectory.insert(saveDirectory.length(),"\\");
		}
		//baraye ejraye narmafzare rasme grph va zakhire khorooji:
		quary.insert(0,"\"\"");
		quary.insert(quary.length(),"Graph\\bin\\dot.exe\" -Tpdf \"");
		quary.insert(quary.length(),myDirectory);
		quary.insert(quary.length(),"Graph\\bin\\CFG.gv\" -o \"");
		quary.insert(quary.length(),saveDirectory);
		_itoa_s(++pdfnumbercfg,c,sizeof(c),10);
		quary.insert(quary.length(),c);
		quary.insert(quary.length(),".pdf\"\"");

		system(quary.c_str());
	
		if(cfg_openpdf){//agar bayad file pdf ra ejra konim.
			//open a pdf file
			quary=saveDirectory;
			quary.insert(quary.length(),c);
			quary.insert(quary.length(),".pdf");
			ShellExecute(NULL, "OPEN", quary.c_str() , NULL,NULL, SW_SHOWNORMAL);
		}
	}
}
void CFGD::makeGraphinPDF(){
	///////////////////////////////////////make Ghraph in to pdf in path ../pdfs/CFGi.pdf
	char myPath[_MAX_PATH+1];
	string myDirectory="";
	GetModuleFileName(NULL,myPath,_MAX_PATH);
	string quary = "";
	quary.insert(quary.length(),myPath);
	int locationfind = quary.find_last_of('\\');
	if(locationfind >=0)
	{
		quary = quary.substr(0,locationfind+1);
		myDirectory=quary;
	}
	quary.insert(quary.length(),"Graph\\bin\\dot.exe -Tpdf ");
	quary.insert(quary.length(),myDirectory);
	quary.insert(quary.length(),"Graph\\bin\\CFG.gv -o ");
	quary.insert(quary.length(),myDirectory);
	quary.insert(quary.length(),"pdfs\\CFG");
	char c[10];
	_itoa_s(++pdfnumbercfg,c,sizeof(c),10);
	quary.insert(quary.length(),c);
	quary.insert(quary.length(),".pdf");

	system(quary.c_str());
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
	//load_INfirst_registers();													//load registers value in first time;
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
			instruction* ins = new instruction(line,linenumber,codes,&variabletabel,&registertabel,&templatevariablenamenumber,&templatepointervariablenumber,stack);				// define new instruction and set its line number.
			if(ins->islableform()) //if have a lable to instruction save it in block and make a object Label and save it in to the label buffer
			{
				if(!Find_this_label_in_last_labels(ins->getlablename())){
					newblock->add_new_instruction(ins);
					lable* newlabel = new lable(ins->getlablename(),newblock,newblock->size());
					labels.push_back(newlabel);
				}
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

bool CFGD::Find_this_label_in_last_labels(string inslabel){
	for (unsigned int i = 0; i < labels.size(); i++)
	{
		if(labels.at(i)->name.compare(inslabel)==0)
			return true;
	}
	return false;
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
	if(!branchesset){
		branches = new string[bnum];
		branches[10] = "ret";
		branches[1] = "call";
		branches[9] = "jump";//no sharty
		branches[3] = "jecxz";// sharty
		branches[4] = "jne";// sharty
		branches[5] = "jle";// sharty
		branches[6] = "jl";// sharty
		branches[7] = "jb";// sharty
		branches[11] = "jz";// sharty
		branches[12] = "jnz";// sharty
		branches[13] = "jnle";// sharty
		branches[14] = "jnge";// sharty
		branches[15] = "jnae";// sharty
		branches[16] = "jna";// sharty
		branches[17] = "jnc";// sharty
		branches[18] = "jcxz";// sharty
		branches[19] = "jns";// sharty
		branches[20] = "jge";// sharty
		branches[21] = "jng";// sharty
		branches[22] = "jnl";// sharty
		branches[23] = "jg";// sharty
		branches[24] = "js";// sharty
		branches[25] = "jae";// sharty
		branches[26] = "jnbe";// sharty
		branches[27] = "ja";// sharty
		branches[28] = "jnb";// sharty
		branches[0] = "jbe";// sharty
		branches[2] = "jmp";//no sharty
		branches[8] = "goto"; //no sharty
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
		branchesset = true;
	}
}

/*get a block b and seperate from instruction startindex to end and store they to tonb (to new block)*/
/* label ha ro ham moratab mikonad. dar shekastane block adress va block label ha momken azt taghir konad in moratb sazy ro anjam midahad*/
bool CFGD::sep_block(block* b,int startindex,block* tonb){
	int k=0;
	string namel;
	//int iniadded =0;
	if(startindex>=b->size())
		return false;
	if(b->jumpisfor(call_farakhony)){
		b->Delete_IT_return_IN();
	}
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
					labels.at(p)->blockini = k;
					labels.at(p)->blockL = tonb;
					break;
				}
			}
		}
	}
	return true;
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
					case 10: // for ret or return
						{
							blocklabel = new block(getnextbname());
							if(sep_block(b,j,blocklabel)){
								inss->push_back(blocklabel);
								
								b->Copyoutsintoelse(blocklabel,call_return);/*آن هایی که تا حالا از بلاک اولی خروج می شدند باید از بلام دومی ایجاد شده حال خارج شوند.*/
								b->clearoutelse(call_return);/*..*/
							}
						}
						break;
					case 1: // for call
					// dar call miravad dar block call shode va bar mighardad va edame midahad
						blocklabel =  get_block_lable(labelname,&bbuf,true);
						if(blocklabel!=0){/* بلاک پیدا دشده*/
							block* nb = new block(getnextbname());/* بلاک را از هم جدا می کند از فراخوانی به بعد را در این بلاک می ریزد*/
							if(sep_block(b,j,nb)){/* در صورتی که بتواند بعد از این بلاک بلاک جدید را ایجاد کند وارد شرط می شود.*/
								inss->push_back(nb);/*بلاک را به مجموعه بلاک ها اضافه می کنیم*/
								b->Copyoutsinto(nb);/*آن هایی که تا حالا از بلاک اولی خروج می شدند باید از بلام دومی ایجاد شده حال خارج شوند.*/
								b->clearout();/*..*/
								nb->add_new_IN(b);
								b->add_new_OUT(nb,con_after_call);
							}else{
								nb = b->getOUTContinue();
								if(nb == NULL || blocklabel->getname().compare(nb->getname())!=0)
								{
									b->change_continue_OUT_to(con_after_call);
								}
							}
							blocklabel->add_new_IN(b);
							b->add_new_OUT(blocklabel,call_farakhony);
							if(blocklabel->isnewerthan(b)){
								b->add_new_IN(blocklabel);
								blocklabel->add_new_OUT(b,call_return);
							}else{
								/*vector<block*>* rets = TraceForFindReturn(blocklabel);
								for(unsigned int i=0;i<rets->size();i++){
									block* retfrom = rets->at(i);
									b->add_new_IN(retfrom);
									retfrom->add_new_OUT(b,call_return);
								}
								rets->clear();
								delete rets;*/
							}
						}
						else{
							//یک بلاک جدید می کنیم و یک دستور با نام لیبل آن در آن قرار می دهیم. و لیبل جدید را در لیست لیبل ها نیز اضافه می کنیم اگر وجود نداشته باشد.
							blocklabel = new block(getnextbnameoutsilde());
							blocklabel->setsample();
							lable* l = new lable(labelname,blocklabel,1);
							labelname.insert(labelname.length(),":");
							labels.push_back(l);
							blocklabel->add_new_instruction(new instruction(labelname,-1,codes,&variabletabel,&registertabel,&templatevariablenamenumber,&templatepointervariablenumber,stack));
							inss->push_back(blocklabel);
							//////////////
							block* nb = new block(getnextbname());
							if(sep_block(b,j,nb)){
								inss->push_back(nb);
								b->Copyoutsinto(nb);
								b->clearout();
								nb->add_new_IN(b);
								b->add_new_OUT(nb,con_after_call);
							}else{
								nb = b->getOUTContinue();
								if(nb == NULL || blocklabel->getname().compare(nb->getname())!=0)
								{
									b->change_continue_OUT_to(con_after_call);
								}
							}
							blocklabel->add_new_IN(b);
							b->add_new_IN(blocklabel);
							b->add_new_OUT(blocklabel,call_farakhony);
							blocklabel->add_new_OUT(b,call_return);
						}
						break;
					case 2: case 9: case 8: // branch ghayre sharty
					//dar jump gheyre sharty miravad be block label va bar nemighardad
						blocklabel =  get_block_lable(labelname,&bbuf,false);
						if(blocklabel!=0){
							block* nb = new block(getnextbname());
							if(sep_block(b,j,nb)){
								b->Copyoutsinto(nb);
								b->clearout();
								inss->push_back(nb);
								nb->add_new_IN(b);
								b->add_new_OUT(nb,continue_code);
							}
							blocklabel->add_new_IN(b);
							b->add_new_OUT(blocklabel,jumping);
						}else{
							blocklabel = new block(getnextbnameoutsilde());
							blocklabel->setsample();
							lable* l = new lable(labelname,blocklabel,1);
							labelname.insert(labelname.length(),":");
							blocklabel->add_new_instruction(new instruction(labelname,-1,codes,&variabletabel,&registertabel,&templatevariablenamenumber,&templatepointervariablenumber,stack));
							labels.push_back(l);
							inss->push_back(blocklabel);
							//////////////////////////////////////////////////
							block* nb = new block(getnextbname());
							if(sep_block(b,j,nb)){
								b->Copyoutsinto(nb);
								b->clearout();
								inss->push_back(nb);
								nb->add_new_IN(b);
								b->add_new_OUT(nb,continue_code);
							}
							blocklabel->add_new_IN(b);
							b->add_new_OUT(blocklabel,jumping);
						}
						break;
					case 3:case 29:case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 28:case 27:case 26:case 25:case 24:case 23:case 22:case 21:case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 20:case 11: case 12: case 4: case 5: case 6: case 7: case 0: // jump haye sharty.
					// dar in mored aghar shart sadegh bashad miravad be block label else edeme block ke be nb tabdil shode ast
						blocklabel =  get_block_lable(labelname,&bbuf,true);
						if(blocklabel!=0){
							block* nb = new block(getnextbname());
							string btonb = False_condition,btoblocklabel=True_condition;
							if(sep_block(b,j,nb)){
								inss->push_back(nb);
								b->Copyoutsinto(nb);
								b->clearout();
								nb->add_new_IN(b);
								b->add_new_OUT(nb,btonb);
								blocklabel->add_new_IN(b);
								b->add_new_OUT(blocklabel,btoblocklabel);
							}else{
								nb = b->getOUTContinue();
								if(nb == NULL || blocklabel->getname().compare(nb->getname())!=0)
								{
									b->change_continue_OUT_to(btonb);
									blocklabel->add_new_IN(b);
									b->add_new_OUT(blocklabel,btoblocklabel);
								}
								else b->change_continue_OUT_to(true_and_false);
							}
						}else{
							blocklabel = new block(getnextbnameoutsilde());
							blocklabel->setsample();
							lable* l = new lable(labelname,blocklabel,1);
							labels.push_back(l);
							labelname.insert(labelname.length(),":");
							blocklabel->add_new_instruction(new instruction(labelname,-1,codes,&variabletabel,&registertabel,&templatevariablenamenumber,&templatepointervariablenumber,stack));
							inss->push_back(blocklabel);
							////////////////////////////
							block* nb = new block(getnextbname());
							string btonb = False_condition,btoblocklabel=True_condition;
							if(sep_block(b,j,nb)){
								inss->push_back(nb);
								b->Copyoutsinto(nb);
								b->clearout();
								nb->add_new_IN(b);
								b->add_new_OUT(nb,btonb);
								blocklabel->add_new_IN(b);
								b->add_new_OUT(blocklabel,btoblocklabel);
							}else{
								nb = b->getOUTContinue();
								if(nb == NULL || blocklabel->getname().compare(nb->getname())!=0)
								{
									b->change_continue_OUT_to(btonb);
									blocklabel->add_new_IN(b);
									b->add_new_OUT(blocklabel,btoblocklabel);
								}
								else b->change_continue_OUT_to(true_and_false);
							}
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
block* CFGD::get_block_lable(string _name,vector<block*>* inss,bool docontinue){
	for(unsigned int i=0;i<labels.size();i++)
	{
		if(_name.compare( labels.at(i)->name)==0)
		{
			//cout<<"label "<<labels.at(i)->name<<" found in line "<<labels.at(i)->blockini<<" of "<< labels.at(i)->blockL->Bname <<"\n";
			block* b = labels.at(i)->blockL; // این بلاکی را که این لیبل در آن وجود دارد را در متغیر b قرار می دهد
			if(labels.at(i)->blockini==1) // if this block is for this label return this block
				return b;
			// if the label is in the center of instruction the block must broken and make a new block that the fisrt instruction of 
			// that is label
			int j = labels.at(i)->blockini-1;
			block * nb = new block(getnextbname());
			if(sep_block(b,j,nb)){
				inss->push_back(nb);
				b->Copyoutsinto(nb);
				b->clearout();
				instruction * ini = b->init(b->size()-1);
				if(ini->isBranch(branches,bnum))
				{
					int index;
					string labelname = ini->gotowhere(branches,bnum,&index);
					switch(index){
					case 10: //for ret not need the continue
						break;
					case 1: // for call
						nb->add_new_IN(b);
						b->add_new_OUT(nb,con_after_call);
						break;
					case 2: case 9: case 8: // branch ghayre sharty
						nb->add_new_IN(b);
						b->add_new_OUT(nb,continue_code);
						break;
					case 3:case 29:case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 28:case 27:case 26:case 25:case 24:case 23:case 22:case 21:case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 20:case 11: case 12: case 4: case 5: case 6: case 7: case 0: // jump haye sharty.
						nb->add_new_IN(b);
						b->add_new_OUT(nb,continue_cl);
						break;
					}
				}
				else{
					nb->add_new_IN(b);
					b->add_new_OUT(nb,continue_cl);
				}
				return nb;
			}
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

void CFGD::loadOPCodes(){
	if(!codesset){
	codes = new vector<codepair*>();
	codes->push_back(new codepair("and",1));
	codes->push_back(new codepair("aaa",2));
	codes->push_back(new codepair("aad",3));
	codes->push_back(new codepair("aas",4));
	codes->push_back(new codepair("adc",1));
	codes->push_back(new codepair("add",1));
	codes->push_back(new codepair("arpl",5));
	codes->push_back(new codepair("bound",5));
	codes->push_back(new codepair("bsr",5));
	codes->push_back(new codepair("bswap",6));
	codes->push_back(new codepair("bt",5));
	codes->push_back(new codepair("btc",5));
	codes->push_back(new codepair("btr",1));
	codes->push_back(new codepair("bts",1));
	codes->push_back(new codepair("call",7));
	codes->push_back(new codepair("cbw",8));
	codes->push_back(new codepair("cdq",9));
	codes->push_back(new codepair("cmp",5));
	codes->push_back(new codepair("cmps",10));
	codes->push_back(new codepair("cmpsb",10));
	codes->push_back(new codepair("cmpsw",10));
	codes->push_back(new codepair("cmpsd",10));
	codes->push_back(new codepair("cmpxchg",11));
	codes->push_back(new codepair("cmpxchg8b",11));
	codes->push_back(new codepair("cwd",12));
	codes->push_back(new codepair("cwde",13));
	codes->push_back(new codepair("daa",2));
	codes->push_back(new codepair("das",2));
	codes->push_back(new codepair("dec",6));
	codes->push_back(new codepair("div",14));
	codes->push_back(new codepair("idiv",14));
	codes->push_back(new codepair("enter",5));
	codes->push_back(new codepair("esc",5));
	codes->push_back(new codepair("imul",15));
	codes->push_back(new codepair("in",5));
	codes->push_back(new codepair("inc",6));
	codes->push_back(new codepair("ins",16));
	codes->push_back(new codepair("insb",16));
	codes->push_back(new codepair("insw",16));
	codes->push_back(new codepair("insd",16));
	codes->push_back(new codepair("int",7));
	codes->push_back(new codepair("jea",7));
	codes->push_back(new codepair("jnb",7));
	codes->push_back(new codepair("jb",7));
	codes->push_back(new codepair("jnae",7));
	codes->push_back(new codepair("jbe",7));
	codes->push_back(new codepair("jna",7));
	codes->push_back(new codepair("jc",7));
	codes->push_back(new codepair("jcxz",17));
	codes->push_back(new codepair("jecxz",17));
	codes->push_back(new codepair("je",7));
	codes->push_back(new codepair("jz",7));
	codes->push_back(new codepair("jg",7));
	codes->push_back(new codepair("jnle",7));
	codes->push_back(new codepair("jge",7));
	codes->push_back(new codepair("jnl",7));
	codes->push_back(new codepair("jl",7));
	codes->push_back(new codepair("jnge",7));
	codes->push_back(new codepair("jle",7));
	codes->push_back(new codepair("jng",7));
	codes->push_back(new codepair("jmp",7));
	codes->push_back(new codepair("jnc",7));
	codes->push_back(new codepair("jne",7));
	codes->push_back(new codepair("jnz",7));
	codes->push_back(new codepair("jno",7));
	codes->push_back(new codepair("jns",7));
	codes->push_back(new codepair("jnp",7));
	codes->push_back(new codepair("jpo",7));
	codes->push_back(new codepair("jo",7));
	codes->push_back(new codepair("jp",7));
	codes->push_back(new codepair("jpe",7));
	codes->push_back(new codepair("js",7));
	codes->push_back(new codepair("lahf",18));
	codes->push_back(new codepair("lar",21));
	codes->push_back(new codepair("lds",21));
	codes->push_back(new codepair("lea",21));
	codes->push_back(new codepair("les",21));
	codes->push_back(new codepair("lfs",21));
	codes->push_back(new codepair("lgdt",7));
	codes->push_back(new codepair("lidt",7));
	codes->push_back(new codepair("lgs",21));
	codes->push_back(new codepair("lldt",7));
	codes->push_back(new codepair("lmsw",7));
	codes->push_back(new codepair("lods",19));
	codes->push_back(new codepair("lodsb",19));
	codes->push_back(new codepair("lodsw",19));
	codes->push_back(new codepair("lodsd",19));
	codes->push_back(new codepair("loop",20));
	codes->push_back(new codepair("loopnz",20));
	codes->push_back(new codepair("loopne",20));
	codes->push_back(new codepair("lsl",21));
	codes->push_back(new codepair("lss",21));
	codes->push_back(new codepair("ltr",7));
	codes->push_back(new codepair("mov",21));
	codes->push_back(new codepair("movs",22));
	codes->push_back(new codepair("movsb",22));
	codes->push_back(new codepair("movsw",22));
	codes->push_back(new codepair("movsd",22));
	codes->push_back(new codepair("movsx",21));
	codes->push_back(new codepair("movzx",21));
	codes->push_back(new codepair("mul",15));
	codes->push_back(new codepair("neg",6));
	codes->push_back(new codepair("not",6));
	codes->push_back(new codepair("or",1));
	codes->push_back(new codepair("out",23));
	codes->push_back(new codepair("outs",24));
	codes->push_back(new codepair("outsb",24));
	codes->push_back(new codepair("outsw",24));
	codes->push_back(new codepair("outsd",24));
	codes->push_back(new codepair("pop",25));
	codes->push_back(new codepair("popa",26));
	codes->push_back(new codepair("popaw",27));
	codes->push_back(new codepair("push",7));
	codes->push_back(new codepair("pusha",29));
	codes->push_back(new codepair("pushaw",28));
	codes->push_back(new codepair("rcl",1));
	codes->push_back(new codepair("rcr",1));
	codes->push_back(new codepair("rep",30));
	codes->push_back(new codepair("repe",30));
	codes->push_back(new codepair("repz",30));
	codes->push_back(new codepair("repne",30));
	codes->push_back(new codepair("repnz",30));
	codes->push_back(new codepair("ret",7));
	codes->push_back(new codepair("rol",1));
	codes->push_back(new codepair("ror",1));
	codes->push_back(new codepair("sahf",31));
	codes->push_back(new codepair("shl",1));
	codes->push_back(new codepair("sal",1));
	codes->push_back(new codepair("rar",1));
	codes->push_back(new codepair("sbb",1));
	codes->push_back(new codepair("setbe",25));
	codes->push_back(new codepair("setna",25));
	codes->push_back(new codepair("sete",25));
	codes->push_back(new codepair("setz",25));
	codes->push_back(new codepair("setne",25));
	codes->push_back(new codepair("setnz",25));
	codes->push_back(new codepair("setl",25));
	codes->push_back(new codepair("setnge",25));
	codes->push_back(new codepair("setge",25));
	codes->push_back(new codepair("setnl",25));
	codes->push_back(new codepair("setle",25));
	codes->push_back(new codepair("setng",25));
	codes->push_back(new codepair("setg",25));
	codes->push_back(new codepair("setnle",25));
	codes->push_back(new codepair("sets",25));
	codes->push_back(new codepair("setns",25));
	codes->push_back(new codepair("setc",25));
	codes->push_back(new codepair("setnc",25));
	codes->push_back(new codepair("seto",25));
	codes->push_back(new codepair("setno",25));
	codes->push_back(new codepair("setp",25));
	codes->push_back(new codepair("setpe",25));
	codes->push_back(new codepair("setnp",25));
	codes->push_back(new codepair("setpo",25));
	codes->push_back(new codepair("sgdt",25));
	codes->push_back(new codepair("sidt",25));
	codes->push_back(new codepair("shr",1));
	codes->push_back(new codepair("shld",1));
	codes->push_back(new codepair("shrd",1));
	codes->push_back(new codepair("sldt",1));
	codes->push_back(new codepair("sldt",25));
	codes->push_back(new codepair("smsw",25));
	codes->push_back(new codepair("stos",33));
	codes->push_back(new codepair("stosb",33));
	codes->push_back(new codepair("stosw",33));
	codes->push_back(new codepair("stosd",33));
	codes->push_back(new codepair("str",25));
	codes->push_back(new codepair("sub",1));
	codes->push_back(new codepair("test",21));
	codes->push_back(new codepair("verr",7));
	codes->push_back(new codepair("verw",7));
	codes->push_back(new codepair("xchg",34));
	codes->push_back(new codepair("xlat",35));
	codes->push_back(new codepair("xlatb",35));
	codes->push_back(new codepair("xor",1));
	codesset = true;
	}
}

void CFGD::clear(){
	for(unsigned int i=0;i<bbuf.size();i++)
		free(bbuf.at(i));
	bbuf.clear();
	for(unsigned int i=0;i<labels.size();i++)
		free(labels.at(i));
	labels.clear();
	for(unsigned int i=0;i<imdlls.size();i++)
		free(imdlls.at(i));
	imdlls.clear();
	for(unsigned int i=0;i<dlimdlls.size();i++)
		free(dlimdlls.at(i));
	dlimdlls.clear();
	stack->clear();
	Bnamecounter =0;
}

void CFGD::MakeGVfile(vector<block*> inss){
	
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

vector<block*>* CFGD::TraceForFindReturn(block* blocklabel){
	vector<block*>* rets = new vector<block*>();
	if(blocklabel!=NULL){
		instruction * ini = blocklabel->init(blocklabel->size()-1);
		if(ini->isBranch(branches,bnum))
		{
			int index;
			string labelname = ini->gotowhere(branches,bnum,&index);
			switch(index){
				case 10: //for ret not need the continue
					rets->push_back(blocklabel);
					break;
				case 1: // for call
					{
						merge2vector(rets,TraceForFindReturn(blocklabel->getOUTfor(con_after_call)));
					}
					break;
				case 2: case 9: case 8: // branch ghayre sharty
					merge2vector(rets,TraceForFindReturn(blocklabel->getOUTfor(continue_code)));
					break;
				case 3:case 29:case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 28:case 27:case 26:case 25:case 24:case 23:case 22:case 21:case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 20:case 11: case 12: case 4: case 5: case 6: case 7: case 0: // jump haye sharty.
					{
						vector<block*> bls = blocklabel->getOUTsfor(True_condition);
						for(unsigned int i=0;i<bls.size();i++)
							merge2vector(rets,TraceForFindReturn(bls.at(i)));
						bls.clear();
						bls = blocklabel->getOUTsfor(False_condition);
						for(unsigned int i=0;i<bls.size();i++)
							merge2vector(rets,TraceForFindReturn(bls.at(i)));
						bls.clear();
						bls = blocklabel->getOUTsfor(true_and_false);
						for(unsigned int i=0;i<bls.size();i++)
							merge2vector(rets,TraceForFindReturn(bls.at(i)));
						bls.clear();
					}
					break;
			}
		}
	}
	return rets;
}

void CFGD::makefilenameforgvout(){
	char myPath[_MAX_PATH+1];
	GetModuleFileName(NULL,myPath,_MAX_PATH);
	fileforgvout.insert(0,myPath);
	int locationfind = fileforgvout.find_last_of('\\');
	if(locationfind >=0)
	{
		fileforgvout = fileforgvout.substr(0,locationfind+1);
	}
	fileforgvout.insert(fileforgvout.length(),GVoutfilename);
}

void CFGD::makefilenameforout(){
	char myPath[_MAX_PATH+1];
		GetPrivateProfileString("Path","CFG_PATH","",myPath,MAX_PATH,"disassemblerconfig.ini");
		if(strcmp(myPath,"")==0)//agar karbar masire zakhireye file cfg ra moshakhas nakarde bashad:
		{
			GetModuleFileName(NULL,myPath,_MAX_PATH);
			fileforout.insert(0,myPath);
			int locationfind = fileforout.find_last_of('\\');
			if(locationfind >=0)
			{
				fileforout = fileforout.substr(0,locationfind+1);
			}
			fileforout.insert(fileforout.length(),outfilename);
		}
		else//agar karbar mahale zakhireye file cfg ra entekhab karde bashad.
		{
			fileforout=myPath;
		}
}

void CFGD::merge2vector(vector<block*>* v1,vector<block*>* v2){
	bool find = false;
	for(unsigned int i=0;i<v2->size();i++)
	{
		for(unsigned int j=0;j<v1->size();j++)
		{
			if(v1->at(j)->getname().compare(v2->at(i)->getname())==0)
			{
				find = true;
				break;
			}
		}
		if(!find)
		{
			v1->push_back(v2->at(i));
		}else find = false;
	}
	v2->clear();
	delete v2;
}
