#include "OPcodeSimulatetion.h"

OPcodeSimulatetion::OPcodeSimulatetion(void)
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
	myDirectory.insert(myDirectory.length(),"Logs\\OPcodeSimutaion");
	
	DEBUG_CONF(myDirectory, Logger::file_on|Logger::screen_on, DBG_DEBUG, DBG_ERROR);//config logger.
	loadOPcodes();//load opcodes and make trees;
	loadJUMPSKINDOPcodes();
	this->state = NULL;
	this->ini = NULL;
	this->env = NULL;
	
}


OPcodeSimulatetion::~OPcodeSimulatetion(void)
{

}

void OPcodeSimulatetion::DOsimulation(State* state){
	this->state = state;
	this->ini = state->block->init(0);
	this->env = state->getenv();
	HDICTIONARYNODEKEY<int>* key = opcodes_list.getkey(ini->opcode);
	if(key!=NULL){
		int opnum = key->_value_;
		switch (opnum)
		{
		case 1:
			call();
			break;
		case 2:
			ret();
			break;
		case 3:
			push();
			break;
		case 4:
			mov();
			break;
		case 5:
			_test();
			break;
		case 6:
			ADD();
			break;
		case 7:
			SUB();
			break;
		case 8:
			CMP();
			break;
		case 9:
			POP();
			break;
		case 10:
			AAA();
			break;
		case 11:
			AAD();
			break;

		default:
			break;
		}
	}else {
		
		DEBUG(DBG_DEBUG, "code \"" << ini->getinstructin() <<"\" opcode is not defined.");
	}
}

bool OPcodeSimulatetion::Cmp_OR_test(State* state){
	this->state = state;
	this->ini = state->block->init(0);
	this->env = state->getenv();
	HDICTIONARYNODEKEY<int>* key = opcodes_list.getkey(ini->opcode);
	if(key!=NULL){
		int opnum = key->_value_;
		switch (opnum)
		{
		case 5:
		//	return _test();
			break;
		case 8:
		//	return CMP();
		default:
			break;
		}
	}else {
		DEBUG(DBG_DEBUG, "code \"" << ini->getinstructin() <<"\" opcode is not define or it is not a test code.");
	}
	return false;
}

void OPcodeSimulatetion::loadOPcodes(){
	opcodes_list.add("call",new HDICTIONARYNODEKEY<int>(1));
	opcodes_list.add("ret",new HDICTIONARYNODEKEY<int>(2));
	opcodes_list.add("push",new HDICTIONARYNODEKEY<int>(3));
	opcodes_list.add("mov",new HDICTIONARYNODEKEY<int>(4));
	opcodes_list.add("test",new HDICTIONARYNODEKEY<int>(5));
	opcodes_list.add("add",new HDICTIONARYNODEKEY<int>(6));
	opcodes_list.add("sub",new HDICTIONARYNODEKEY<int>(7));
	opcodes_list.add("cmp",new HDICTIONARYNODEKEY<int>(8));
	opcodes_list.add("pop",new HDICTIONARYNODEKEY<int>(9));
	opcodes_list.add("aaa",new HDICTIONARYNODEKEY<int>(10));
	opcodes_list.add("aad",new HDICTIONARYNODEKEY<int>(11));
}

void OPcodeSimulatetion::call(){

}

void OPcodeSimulatetion::CMP(){
	 _test();
}

void OPcodeSimulatetion::AAD(){
	if(!is64_BitMode()){
		Variable* AL = this->env->getregister("al")->Copy();
		Variable* AH = this->env->getregister("ah")->Copy();

	}
}

void OPcodeSimulatetion::ADD(){
	string inis = ini->getinstructin();
	inis = ini->deleteComments(inis);
	string s = inis.substr(inis.find(ini->opcode)+ini->opcode.length());
	vector<string> arg = ini->split_string(s,',');
	arg[0] = ini->deletespace(arg[0]);
	arg[1] = ini->deletespace(arg[1]);
	Range R1 = GetArg(arg[0]),R2=GetArg(arg[1]);
	setvariable(arg[0],R1.operatorADD(R2));
}

void OPcodeSimulatetion::AAA(){// page 78 intel book.
	/*IF 64-Bit Mode
		THEN
		#UD;
		ELSE
		IF ((AL AND 0FH) > 9) or (AF = 1)
		THEN
		AL ← AL + 6;
		AH ← AH + 1;
		AF ← 1;
		CF ← 1;
		AL ← AL AND 0FH;
		ELSE
		AF ← 0;
		CF ← 0;
		AL ← AL AND 0FH;
		FI;
		FI;
	*/
	if(!is64_BitMode()){
		Variable* AL = this->env->getregister("al"); 
		Variable* AF = this->env->getregister("af");
		Range* rangeAL = AL->getvalue(8,0); 
		Range* res = (*(rangeAL) & (Range (15,15))); // 15 = 0fh
		bool ifALandOFHB9orAFeq1 = *res>9 || *(AF->Ranges[0])==1;
		if(ifALandOFHB9orAFeq1){
			Range* newrangeal = rangeAL->operatorADD(6);
			AL->setvalue(8,0,*newrangeal);
			delete newrangeal;
			Variable* AH =  this->env->getregister("ah");
			AH->Ranges[0]->operatorADDwithselt(1);
			AF->freeRanges();
			AF->Ranges.push_back(new Range(1,1));
			Variable* CF =  this->env->getregister("cf");
			CF->Ranges.push_back(new Range(1,1));
			Range* res_ = AL->Ranges[0]->operatorAND((Range (15,15)));
			AL->freeRanges();
			AL->Ranges.push_back(res_);
		}else{
			AF->freeRanges();
			AF->Ranges.push_back(new Range(0,0));
			Variable* CF =  this->env->getregister("cf");
			CF->Ranges.push_back(new Range(0,0));
			Range* res_ = AL->Ranges[0]->operatorAND((Range (15,15)));
			AL->freeRanges();
			AL->Ranges.push_back(res_);
		}
		if(res!=NULL)
			delete res;
		delete rangeAL;
	}
}

void OPcodeSimulatetion::SUB(){
	string inis = ini->getinstructin();
	inis = ini->deleteComments(inis);
	string s = inis.substr(inis.find(ini->opcode)+ini->opcode.length());
	vector<string> arg = ini->split_string(s,',');
	arg[0] = ini->deletespace(arg[0]);
	arg[1] = ini->deletespace(arg[1]);
	Range R1 = GetArg(arg[0]),R2=GetArg(arg[1]);
	setvariable(arg[0],R1.operatorSUB(R2));
}

void OPcodeSimulatetion::setvariable(string s,Range* R){
	if(ini->isregister(s))
	{
		Variable* Reg = this->env->getregister(s);
		if(Reg==NULL){
			Reg = new Variable(s);
			this->env->addtoregister(Reg);			
		}
		Reg->freeRanges();
		Reg->Ranges.push_back(R);
	}
	else if(ini->isHexnumber(s)){

	}else {
		Variable* var = this->env->getvariable(s);
		if(var==NULL){
			var = new Variable(s);
			this->env->addtovars(var);			
		}
		var->freeRanges();
		var->Ranges.push_back(R);
	}
}

Range OPcodeSimulatetion::GetArg(string s){
	if(ini->isregister(s))
	{
		Variable* Reg = this->env->getregister(s);
		if(Reg==NULL){
			Reg = new Variable(s);
			this->env->addtoregister(Reg);
		}
		if(Reg->Ranges.size()>0)
			return *Reg->Ranges.at(0);
		else {
			Range R();
		}
	}
	else if(ini->isHexnumber(s)){
		double d = ini->StringHexTointConverter(s);
		Range R(d,d);
		return R;
	}else {
		Variable* var = this->env->getvariable(s);
		if(var==NULL){
			var = new Variable(s);
			this->env->addtovars(var);
		}
		if(var->Ranges.size()>0)
			return *var->Ranges.at(0);
		else {
			Range R();
		}
	}
}

void OPcodeSimulatetion::Test_Env_changes(bool test_result, string arg1,string arg2,comperation_kind CKIND){
	double d = 0,min,max,min2,max2;
	bool arg1_isnotregister = false; // if 5,6,8, اولی نی تواند با حالت اول اجرا شود. اگر اولی رجیستر باشد این مقدار ناردست و اگر نباشد مقدار درست را به خود میگیرد.
	bool arg1_isHex_arg2_isHex = false;
	if(ini->isregister(arg1) && ini->isregister(arg2)){ //هر دو ورودی ثبات است
		d = this->env->getregister(arg2)->Ranges.at(0)->get__Max_value();
	}else if(ini->isregister(arg1) && ini->isHexnumber(arg2)){ // اولی ثبات و دومی عدد است
		d = ini->StringHexTointConverter(arg2);
	}else if(ini->isregister(arg1)){ // اولی ثبات و دومی حافظه است
		d = env->getvariable(arg2)->Ranges.at(0)->get__Max_value();
	}else if(ini->isHexnumber(arg1) && ini->isregister(arg2)){ // اولی عدد و دمی ثبات است.
		string temp = arg1; arg1 = arg2;arg2 = temp;// در این حالت جای دو تا ورودی را عوض کرده و مانند شرط 2 می شود.
		d = ini->StringHexTointConverter(arg2);
		test_result = !test_result;
	}
	else if(ini->isHexnumber(arg1) && ini->isHexnumber(arg2)){ // هر دو تا عدد هستند. در این صورت شرط همیشه صادق باید باشد. منطقی نیست شاید این قسمت اصلاً به وجود نیاید.
		// در این وضعیت در چیزی تغییر حاصل نمی شود.
		arg1_isHex_arg2_isHex = true;
		arg1_isnotregister = true;
	}
	else if(ini->isHexnumber(arg1)){// اولی از نوع عدد و دومی از نوع حافظه است. 
		arg1_isnotregister = true;
		string temp = arg1; arg1 = arg2;arg2 = temp;// در این حالت جای دو تا ورودی را عوض کرده و مانند شرط 2 می شود.
		d = ini->StringHexTointConverter(arg2);
		test_result = !test_result;
	}
	else if(ini->isregister(arg2)){ // اولی از نوع حافظه و دومی ازنوع رجیستر است.
		string temp = arg1; arg1 = arg2;arg2 = temp; // جای دو آرگماان با وضعیت آن عوض می شود مانند شرط 3 می شود.
		d = env->getvariable(arg2)->Ranges.at(0)->get__Max_value();
		test_result = !test_result;
	}
	else if(ini->isHexnumber(arg2)){// اولی از نوع حافظه و دومی از نوع عدد است.
		arg1_isnotregister = true;
		d = ini->StringHexTointConverter(arg2);
	} else{//هر دو تا ورودی از نوع حافظه هستند.
		arg1_isnotregister = true;
		d = env->getvariable(arg2)->Ranges.at(0)->get__Max_value();
	}

	/////////////////////////////////////
	switch (CKIND)
	{
	case comperation_kind::equal:
		if(test_result)
			min = max = d;
		else ;
		break;
	case notequal:
		test_result = !test_result;
		if(test_result)
			min = max = d;
		else ;
		break;
	case biger:
		if(test_result)
		{min = d+1;max = Range::getMax_doublevalue();}
		else {min = Range::getMin_doublevalue();max = d;}
		break;
	case bigerandequal:
		if(test_result)
		{min = d;max = Range::getMax_doublevalue(); min2 = Range::getMin_doublevalue();max2 = d-1;}
		else {min = Range::getMin_doublevalue();max = d-1; min2 = d;max2 = Range::getMax_doublevalue();}
		break;
	case smaller:
		test_result = !test_result;
		if(test_result)
		{min = d;max = Range::getMax_doublevalue();}
		else {min = Range::getMin_doublevalue();max = d-1;}
		break;
	case smallerandequal:
		test_result = !test_result;
		if(test_result)
		{min = d+1;max = Range::getMax_doublevalue();}
		else {min = Range::getMin_doublevalue();max = d;}
		break;
	case notfound:
		break;
	case ifodd:
		break;
	case ifeven:
		break;
	case LOOP:
		break;
	case LOOPE:
		break;
	case LOOPNE:
		break;
	case ifsign:
		break;
	case ifnotsign:
		break;
	case ifoverflow:
		break;
	case ifnotoverflow:
		break;
	case ifcxisziro:
		break;
	case ifecxisziro:
		break;
	default:
		break;
	}
	if(!arg1_isnotregister){
		Range R(min,max);
		Range R2(min2,max2);
		ENV* envc = env->Copy();
		Variable* reg = envc->getregister(arg1);
		if(reg!=NULL)
		{
			Range* RR = reg->Ranges[0]->operatorSUBESCRIPTION(R);
			reg->freeRanges();
			reg->Ranges.push_back(RR);
		}
		state->setENV(envc);
		reg = state->getlastenv()->getregister(arg1);
		if(reg!=NULL)
		{
			Range* RR = reg->Ranges[0]->operatorSUBESCRIPTION(R2);
			reg->freeRanges();
			reg->Ranges.push_back(RR);
		}
	}else if(!arg1_isHex_arg2_isHex){
		Range R(min,max);
		Range R2(min2,max2);
		ENV* envc = env->Copy();
		Variable* var = envc->getvariable(arg1);
		if(var!=NULL)
		{
			Range* RR = var->Ranges[0]->operatorSUBESCRIPTION(R);
			var->freeRanges();
			var->Ranges.push_back(RR);
		}
		state->setENV(envc);
		var = state->getlastenv()->getregister(arg1);
		if(var!=NULL)
		{
			Range* RR = var->Ranges[0]->operatorSUBESCRIPTION(R2);
			var->freeRanges();
			var->Ranges.push_back(RR);
		}
	}
}

bool OPcodeSimulatetion::is64_BitMode(){
	return false;
}

void OPcodeSimulatetion::_test(){
	string inis = ini->getinstructin();
	inis = ini->deleteComments(inis);
	string s = inis.substr(inis.find(ini->opcode)+ini->opcode.length());
	vector<string> arg = ini->split_string(s,',');
	arg[0] = ini->deletespace(arg[0]);
	arg[1] = ini->deletespace(arg[1]);
	Range R1 = GetArg(arg[0]),R2=GetArg(arg[1]);
	Range* res = R1.operatorSUB(R2);
	if((*res)==Range(0,0))
	{
		Variable *zf = env->getregister("z");
		zf->freeRanges();
		zf->Ranges.push_back(new Range(1,1));
	}
	delete res;
	/*string inis = ini->getinstructin();
	inis = ini->deleteComments(inis);
	string s = inis.substr(inis.find(ini->opcode)+ini->opcode.length());
	vector<string> arg = ini->split_string(s,',');
	arg[0] = ini->deletespace(arg[0]);
	arg[1] = ini->deletespace(arg[1]);
	Range R1 = GetArg(arg[0]),R2=GetArg(arg[1]);
	bool test_result;
	switch (getcomperation_kind_())
	{
	case comperation_kind::equal:
		test_result = R1 == R2;
		Test_Env_changes(test_result,arg[0],arg[1],comperation_kind::equal);
		return test_result;
		break;
	case comperation_kind::notequal:
		test_result = R1 != R2;
		Test_Env_changes(test_result,arg[0],arg[1],comperation_kind::notequal);
		return test_result;
		break;
	case comperation_kind::bigerandequal:
		test_result = (R1>=R2);
		Test_Env_changes(test_result,arg[0],arg[1],comperation_kind::bigerandequal);
		return test_result;
		break;
	case comperation_kind::biger:
		test_result = (R1>R2);
		Test_Env_changes(test_result,arg[0],arg[1],comperation_kind::biger);
		return test_result;
		break;
	case comperation_kind::smaller:
		test_result = (R1<R2);
		Test_Env_changes(test_result,arg[0],arg[1],comperation_kind::smaller);
		return test_result;
		break;
	case comperation_kind::smallerandequal:
		test_result = (R1<=R2);
		Test_Env_changes(test_result,arg[0],arg[1],comperation_kind::smallerandequal);
		return test_result;
		break;
	default:
		break;
	}*/
}

comperation_kind OPcodeSimulatetion::getcomperation_kind_(){
	string s = ini->getinstructin();
	int location = s.find(" ;@j ");
	if(location == string::npos){
		return comperation_kind::notfound;
	}
	location+=5;
	s=s.substr(location);
	HDICTIONARYNODEKEY<comperation_kind>* ret = (JumpsKind.getkey(s));
	if(ret==NULL)
		return comperation_kind::notfound;
	return ret->_value_;
}

void OPcodeSimulatetion::loadJUMPSKINDOPcodes(){
	this->JumpsKind.add("jb",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::smaller));
	this->JumpsKind.add("jnae",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::smaller));
	this->JumpsKind.add("jc",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::smaller));
	this->JumpsKind.add("jae",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::bigerandequal));
	this->JumpsKind.add("jnb",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::bigerandequal));
	this->JumpsKind.add("jnc",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::bigerandequal));
	this->JumpsKind.add("jbe",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::smallerandequal));
	this->JumpsKind.add("jna",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::smallerandequal));
	this->JumpsKind.add("ja",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::biger));
	this->JumpsKind.add("jnbe",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::biger));
	this->JumpsKind.add("jne",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::notequal));
	this->JumpsKind.add("jnz",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::notequal));
	this->JumpsKind.add("je",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::equal));
	this->JumpsKind.add("jz",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::equal));
	this->JumpsKind.add("jp",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::ifeven));
	this->JumpsKind.add("jpe",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::ifeven));
	this->JumpsKind.add("jnp",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::ifodd));
	this->JumpsKind.add("jpo",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::ifodd));
	this->JumpsKind.add("js",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::ifsign));
	this->JumpsKind.add("jns",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::ifnotsign));
	this->JumpsKind.add("jo",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::ifoverflow));
	this->JumpsKind.add("jno",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::ifnotoverflow));
	this->JumpsKind.add("jcxz",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::ifcxisziro));
	this->JumpsKind.add("jecxz",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::ifecxisziro));
	this->JumpsKind.add("jl",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::smaller));
	this->JumpsKind.add("jnge",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::smaller));
	this->JumpsKind.add("jge",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::bigerandequal));
	this->JumpsKind.add("jnl",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::bigerandequal));
	this->JumpsKind.add("jle",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::smallerandequal));
	this->JumpsKind.add("jng",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::smallerandequal));
	this->JumpsKind.add("jg",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::biger));
	this->JumpsKind.add("jnle",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::biger));
	this->JumpsKind.add("loop",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::LOOP));
	this->JumpsKind.add("loope",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::LOOPE));
	this->JumpsKind.add("loopz",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::LOOPE));
	this->JumpsKind.add("loopnz",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::LOOPNE));
	this->JumpsKind.add("loopne",new HDICTIONARYNODEKEY<comperation_kind>(comperation_kind::LOOPNE));
}

void OPcodeSimulatetion::ret(){
	
}

void OPcodeSimulatetion::mov(){
	string inis = ini->getinstructin();
	inis = ini->deleteComments(inis);
	string s = inis.substr(inis.find(ini->opcode)+ini->opcode.length());
	vector<string> arg = ini->split_string(s,',');
	string Dname = ini->deletespace(arg[0]),Uname = ini->deletespace(arg[1]);
	if(ini->isregister(Dname)){
		Variable* reg = env->getregister(Dname);
		if(reg==NULL){
			reg = new Variable(Dname);
			env->addtoregister(reg);
		}else
			reg->freeRanges();
		if(ini->isregister(Uname)){
			Variable* reg2 = env->getregister(Uname);
			if(reg2 == NULL){
				reg2 = new Variable(Uname);
				env->addtoregister(reg2);
			}else
			reg2->CopyRange(reg);
		}else if(ini->isptr(Uname)){
			Variable* var = env->getvariable(Uname);
			if(var == NULL){
				var = new Variable(Uname);
				env->addtovars(var);
			}else
			var->CopyRange(reg);
		}else{
			int value = ini->StringHexTointConverter(Uname);
			Range* R = new Range(value,value);
			reg->Ranges.push_back(R);
		}
	}else {
		Variable* var = env->getvariable(Dname);
		if(var==NULL){
			var = new Variable(Dname);
			env->addtoregister(var);
		}else
			var->freeRanges();
		if(ini->isregister(Uname)){
			Variable* reg2 = env->getregister(Uname);
			if(reg2 == NULL){
				reg2 = new Variable(Uname);
				env->addtoregister(reg2);
			}else
			reg2->CopyRange(var);
		}else if(ini->isptr(Uname)){
			Variable* var = env->getvariable(Uname);
			if(var == NULL){
				var = new Variable(Uname);
				env->addtovars(var);
			}else
			var->CopyRange(var);
		}else{
			int value = ini->StringHexTointConverter(Uname);
			Range* R = new Range(value,value);
			var->Ranges.push_back(R);
		}
	}
}

void OPcodeSimulatetion::push(){
	Range* R;
	if(ini->isregister(ini->U[0])){
		Variable* reg = env->getregister(ini->U[0]);
		if(reg!=NULL) R = reg->Ranges[0]; else R = new Range();
	}else if(ini->isHexnumber(ini->U[0])){
		double d = ini->StringHexTointConverter(ini->U[0]);
		R = new Range(d,d);
	}else {
		Variable* reg = env->getvariable(ini->U[0]);
		if(reg!=NULL) R = reg->Ranges[0]; else R = new Range();
	}
	env->PUSHtostack(R->tostring());
}

void OPcodeSimulatetion::POP(){
	string s = env->POPfromstack();
	Range* R = new Range(s);
	if(ini->isregister(ini->D[0])){
		Variable* reg = env->getregister(ini->D[0]);
		if(reg!=NULL){
			reg->freeRanges();
			reg->Ranges.push_back(R);
		}else {
			reg = new Variable(ini->D[0]);
			reg->Ranges.push_back(R);
		}
	}else if(ini->isHexnumber(ini->D[0])){
	}
	else{
		Variable* reg = env->getvariable(ini->D[0]);
		if(reg!=NULL){
			reg->freeRanges();
			reg->Ranges.push_back(R);
		}else {
			reg = new Variable(ini->D[0]);
			reg->Ranges.push_back(R);
		}
	}
}