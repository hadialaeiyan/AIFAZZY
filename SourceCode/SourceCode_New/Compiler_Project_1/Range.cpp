#include "Range.h"


Range::Range(void)
{
	_valueisdef = false;
}

Range::Range(string s){
	int indexvirghol = s.find(',');
	if(indexvirghol != string::npos){
		string smin = s.substr(1,indexvirghol-1) ,smax = s.substr(indexvirghol+1,s.length()-2-indexvirghol);
		if(smax.compare("")!=0&& smin.compare("")!=0){
			try{
				Max_value = atof(smax.c_str());
				Min_value = atof(smin.c_str());
				_valueisdef = true;
			}catch(exception e){
				_valueisdef = false;
			}
		}else _valueisdef = false;
	}
	else{
		_valueisdef = false;
	}
}

Range::~Range(void)
{
}

bool Range::UPERBOUNDISINF(){
	return this->Max_value == std::numeric_limits<double>::max();
}

Range::	Range(double _Min_value,double _Max_value)
{
	this->_valueisdef = false;
	set(_Min_value,_Max_value);
}

bool Range:: hasonevalue() // return ((Max_value - Min_value) == 0)
{
	return ((Max_value - Min_value) == 0);
}

Range* Range::operatorUP(Range x) // paper : ref ->abstract interpretation : a unified model for static analysis of programs by construction or approximation of foxpoints. cousot
{
	if(this != NULL && &x!=NULL){
		if(!x._valueisdef)
			return this->copy();
		if(!this->_valueisdef)
			return x.copy();

		double min, max;
		Range* R = new Range();
		if(this->Min_value == std::numeric_limits<double>::min())
			min = x.Min_value;
		else min = this->___min(this->Min_value,x.Min_value);
		if(this->Max_value == std::numeric_limits<double>::max())
			max = x.Max_value;
		else max = this->___min(this->Max_value,x.Max_value);

		R->set(min,max);
		return R;
	}
	return NULL;
}

Range* Range::operatorDOWN(Range x) // paper : ref ->abstract interpretation : a unified model for static analysis of programs by construction or approximation of foxpoints. cousot
{
	if(this != NULL && &x!=NULL){
		if(!x._valueisdef)
			return this->copy();
		if(!this->_valueisdef)
			return x.copy();

		double min, max;
		Range* R = new Range();
		if(x.Min_value<this->Min_value)
			min = std::numeric_limits<double>::min();
		else min = this->Min_value;
		if(x.Max_value>this->Max_value)
			max = std::numeric_limits<double>::max();
		else max = this->Max_value;

		R->set(min,max);
		return R;
	}
	return NULL;
}

double Range::getMax_doublevalue(){return std::numeric_limits<double>::max();}
double Range::getMin_doublevalue(){return std::numeric_limits<double>::min();}


void Range::operatorADDwithselt(int value){
	Range R(value,value);
	operatorADDwithselt(R);
}

void Range::operatorADDwithselt(Range value){
	Range* res =  this->operatorADD(value);
	res->copyto(this);
	delete res;
}

void Range::operatorSUBwithselt(int value){
	Range R(value,value);
	operatorSUBwithselt(R);
}

void Range::operatorSUBwithselt(Range value){
	Range* res =  this->operatorSUB(value);
	res->copyto(this);
	delete res;
}

Range* Range::operatorADD(Range x){
	if(this != NULL && &x!=NULL && x._valueisdef && (this->_valueisdef)){
		double min, max;
		Range* R = new Range();
		bool notopdo = false;
		///////////////////////////////////////////////////find min value          
		if(this->Min_value == std::numeric_limits<double>::max() && x.Min_value==std::numeric_limits<double>::max())
			min = std::numeric_limits<double>::max();
		else if((this->Min_value == std::numeric_limits<double>::max() && x.Min_value==std::numeric_limits<double>::min())
			||(x.Min_value == std::numeric_limits<double>::max() && this->Min_value==std::numeric_limits<double>::min()))
			notopdo = true;
		else if(this->Min_value == std::numeric_limits<double>::max() || x.Min_value==std::numeric_limits<double>::max())
			min = std::numeric_limits<double>::max();
		else if((this->Min_value == std::numeric_limits<double>::min() && x.Min_value==std::numeric_limits<double>::min()))
			min = std::numeric_limits<double>::min();
		else if(this->Min_value == std::numeric_limits<double>::min() || x.Min_value==std::numeric_limits<double>::min())
			min = std::numeric_limits<double>::min();
		else min = this->Min_value + x.Min_value;

		///////////////////////////////////////////////////find max value
		if(this->Max_value == std::numeric_limits<double>::max() && x.Max_value==std::numeric_limits<double>::max())
			max = std::numeric_limits<double>::max();
		else if((this->Max_value == std::numeric_limits<double>::max() && x.Max_value==std::numeric_limits<double>::min())
			||(x.Max_value == std::numeric_limits<double>::max() && this->Max_value==std::numeric_limits<double>::min()))
			notopdo = true;
		else if(this->Max_value == std::numeric_limits<double>::max() || x.Max_value==std::numeric_limits<double>::max())
			max = std::numeric_limits<double>::max();
		else if((this->Max_value == std::numeric_limits<double>::min() && x.Max_value==std::numeric_limits<double>::min()))
			max = std::numeric_limits<double>::min();
		else if(this->Max_value == std::numeric_limits<double>::min() || x.Max_value==std::numeric_limits<double>::min())
			max = std::numeric_limits<double>::min();
		else max = this->Max_value + x.Max_value;

		R->set(min,max);
		return R;
	}
	return NULL;
}

Range* Range::operatorSUB(Range x){
	if(this != NULL && &x!=NULL && x._valueisdef && _valueisdef){
		Range* R = x.copy();
		Range* R2 = new Range(-1,-1);
		Range* Mul = R->operatorMUL(*R2);
		delete R;
		delete R2;
		if(Mul!=NULL)
			return this->operatorADD(*Mul);
	}
	return NULL;
}

Range* Range::operatorMUL(Range x){
	if(this != NULL && &x!=NULL && x._valueisdef && this->_valueisdef){
		Range* R = new Range();
		///////////////////////////////////////////////////find min value
		double ac = this->Min_value*x.Min_value,ad=this->Min_value*x.Max_value,bc = this->Max_value*x.Min_value,bd = this->Max_value*x.Max_value;
		R->set(___min(ac,ad,bc,bd),___max(ac,ad,bc,bd));
		return R;
	}
	return NULL;
}

double Range::___min(double x,double y,double z,double w){
	return ___min(___min(x,y),___min(z,w));
}

double Range::___max(double x,double y,double z,double w){
	return ___max(___max(x,y),___max(z,w));
}

Range* Range::operatorDIV(Range x)
{
	if(canDIV(x)){
		Range R(1/x.Max_value,1/x.Min_value);
		return this->operatorMUL(R);
	}
	return NULL;
}

bool Range::canDIV(Range R){
	if(R.Min_value>0 || R.Max_value<0)
		return true;
	return false;
}

Range* Range::operatorUNION(Range x){
	if(this != NULL && &x!=NULL){
		if(!(x._valueisdef))
			return this->copy();
		if(!(this->_valueisdef))
			return x.copy();

		double min, max;
		Range* R = new Range();

		min = this->___min(this->Min_value,x.Min_value);
		max = this->___max(this->Max_value,x.Max_value);
		if(min>max)
			return NULL;

		R->set(min,max);
		return R;
	}
	return NULL;
}

Range* Range::operatorSUBESCRIPTION(Range x){
	if(this != NULL && &x!=NULL){
		if(!x._valueisdef)
			return this->copy();
		if(!this->_valueisdef)
			return x.copy();

		double min, max;
		Range* R = new Range();

		min = this->___max(this->Min_value,x.Min_value);
		max = this->___min(this->Max_value,x.Max_value);
		if(min>max)
			return NULL;

		R->set(min,max);
		return R;
	}
	return NULL;
}

Range* Range::copy(){
	Range* r = new Range(Min_value,Max_value);
	if(!_valueisdef) r->_valueisdef = false;
	return r;
}

void Range::copyto(Range* r){
	r->Min_value= this->Min_value;
	r->Max_value = this->Max_value;
	if(!_valueisdef) r->_valueisdef = false;
}

void Range::set(double _Min_value,double _Max_value){
	this->Max_value = _Max_value;
	this->Min_value = _Min_value;
	this->_valueisdef = true;
}

void Range::set(double Max_Min_value){// max = min = max_min;{
	this->Max_value = this->Min_value = Max_Min_value;
}

double Range::get__Max_value(){
	return this->Max_value;
}

double Range::get__Min_value(){
	return this->Min_value;
}

double  Range::___min(double x,double y){
	return (x>=y)? y:x;
}

double  Range::___max(double x,double y){
	return (x<=y)?y:x;
}

string Range::tostring(){
	std::stringstream s;
	if(this->_valueisdef){
		if(Min_value == std::numeric_limits<double>::min()) s<<"( -INF,";
		else s<<"["<<Min_value<<",";
		if(Max_value == std::numeric_limits<double>::max()) s<<" INF)";
		else s<<Max_value<<"]";
	}
	else return "[,]";
	return s.str();
}

double Range::Magnitude(){
	int x = this->Max_value,y = Min_value;
	if(x <0) x=x*-1;
	if(y <0) y=y*-1;
	return ___max(x,y);
}

bool Range::operator== (Range R){return (Min_value == R.Min_value && Max_value == R.Max_value);}
bool Range::operator!= (Range R){return !(Min_value == R.Min_value && Max_value == R.Max_value);}
bool Range::operator<= (Range R){return !(this->operator>(R));}
bool Range::operator< (Range R){ return Max_value<R.Min_value;}
bool Range::operator>= (Range R){return !(this->operator<(R));}
bool Range::operator> (Range R){return R.Max_value<this->Min_value;}


bool Range::operator== (int R1){ Range R(R1,R1);  return (Min_value == R.Min_value && Max_value == R.Max_value);}
bool Range::operator!= (int R1){Range R(R1,R1);return !(Min_value == R.Min_value && Max_value == R.Max_value);}
bool Range::operator<= (int R1){Range R(R1,R1);return !(this->operator>(R));}
bool Range::operator< (int R1){Range R(R1,R1); return Max_value<R.Min_value;}
bool Range::operator>= (int R1){Range R(R1,R1);return !(this->operator<(R));}
bool Range::operator> (int R1){Range R(R1,R1); return R.Max_value<this->Min_value;}

Range* Range::operator& (Range R){
	Range* Ret = NULL;
	if(this != NULL && this->valueisdef() && R.valueisdef()){
		Ret = new Range();
		double ac = (long long)this->Min_value & (long long)R.Min_value,ad= (long long)this->Min_value & (long long)R.Max_value,bc = (long long)this->Max_value& (long long)R.Min_value,bd = (long long)this->Max_value & (long long)R.Max_value;
		Ret->set(___min(ac,ad,bc,bd),___max(ac,ad,bc,bd));
	}
	return Ret;
}

Range* Range::operatorAND (Range R){
	Range* Ret = NULL;
	if(this != NULL && this->valueisdef() && R.valueisdef()){
		Ret = new Range();
		double ac = (long long)this->Min_value & (long long)R.Min_value,ad= (long long)this->Min_value & (long long)R.Max_value,bc = (long long)this->Max_value& (long long)R.Min_value,bd = (long long)this->Max_value & (long long)R.Max_value;
		Ret->set(___min(ac,ad,bc,bd),___max(ac,ad,bc,bd));
	}
	return Ret;
}

Range* Range::operatorADD(double x){
	return this->operatorADD(Range(x,x));
}