#include "stringCalculator.h"


stringCalculator::stringCalculator(void)
{
}


stringCalculator::~stringCalculator(void)
{
}

int stringCalculator::converttoint(double r){
	return static_cast<int>(r);
}


std::string stringCalculator::int_to_hex( int i )
{
  stringstream s;
  s << std::setfill ('0') << std::setw(sizeof(int)*2) 
         << std::hex << i;
  string str = s.str();
  transform(str.begin(), str.end(),str.begin(), ::toupper);
  return str;
}

double stringCalculator::calculate(string s){
	double result = 0;
	vector<string> tokens = split_string_with_E(s);
	result = E(tokens);
	return result;
}

double stringCalculator::F(vector<string> tokens){
	if(tokens.size()==1)
		return atof(tokens.at(0).c_str());
	return 0;
}

double stringCalculator::T(vector<string> tokens){
	int index = 0;
	int op = -1; // 1 is '*' and 2 is '/'
	for(unsigned int i=0;i<tokens.size();i++){
		if(tokens.at(i).compare("*")==0)
		{
			index = i;
			op = 1;
			break;
		}else if(tokens.at(i).compare("/")==0)
		{
			index = i;
			op = 2;
			break;
		}
	}
	if(op == -1)
	{
		return F(tokens);
	}else{
		vector<string> part1 = getlastVector(tokens,index);
		vector<string> part2 = getotherVector(tokens,index);
		if(op==1)
			return F(part1) * F(part2);
		else if(op == 2)
			return F(part1) / F(part2);
	}
	return 0;
}

double stringCalculator::E(vector<string> tokens){
	int index = 0;
	int op = -1; // 1 is '+' and 2 is '-'
	for(unsigned int i=0;i<tokens.size();i++){
		if(tokens.at(i).compare("+")==0)
		{
			index = i;
			op = 1;
			break;
		}else if(tokens.at(i).compare("-")==0)
		{
			index = i;
			op = 2;
			break;
		}
	}
	if(op == -1)
	{
		return T(tokens);
	}else{
		vector<string> part1 = getlastVector(tokens,index);
		vector<string> part2 = getotherVector(tokens,index);
		if(op==1)
			return T(part1) + T(part2);
		else if(op == 2)
			return T(part1) - T(part2);
	}
	return 0;
}

vector<string> stringCalculator::getlastVector(vector<string> tokens, int index){
	vector<string> s;
	for(unsigned int i=0;i<index&&i<tokens.size();i++)
		s.push_back(tokens.at(i));
	return s;
}

vector<string> stringCalculator::getotherVector(vector<string> tokens, int index){
	vector<string> s;
	for(unsigned int i=index+1;i<tokens.size();i++)
		s.push_back(tokens.at(i));
	return s;
}

vector<string> stringCalculator::split_string_with_E(string s){
	vector<string> data;
	unsigned int lastindex = 0;
	unsigned int i =0 ;
	for(; i<s.length();i++)
		if(s[i]=='+')
		{
			data.push_back(s.substr(lastindex,i-lastindex));
			lastindex = i+1;
			data.push_back("+");
			lastindex = i+1;
		}
		else if(s[i]=='-')
		{
			data.push_back(s.substr(lastindex,i-lastindex));
			lastindex = i+1;
			data.push_back("-");
			lastindex = i+1;
		}
		else if(s[i]=='*')
		{
			data.push_back(s.substr(lastindex,i-lastindex));
			lastindex = i+1;
			data.push_back("*");
			lastindex = i+1;
		}
		else if(s[i]=='/')
		{
			data.push_back(s.substr(lastindex,i-lastindex));
			lastindex = i+1;
			data.push_back("/");
			lastindex = i+1;
		}
	data.push_back(s.substr(lastindex,i-lastindex));
	return data;
}