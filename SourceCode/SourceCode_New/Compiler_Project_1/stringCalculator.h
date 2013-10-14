#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <stdint.h>
#include <iomanip>
#include <algorithm>
#include <ostream>
#include <sstream>
#include <iostream>
#include <ios>
using namespace std;


class stringCalculator
{
public:
	stringCalculator(void);
	~stringCalculator(void);
	double calculate(string s);
	int converttoint(double r);
	std::string int_to_hex( int i );
	vector<string>  split_string_with_E(string s);
	double E(vector<string> tokens);
	vector<string> getlastVector(vector<string> tokens,int index);
	vector<string> getotherVector(vector<string> tokens, int index);
	double T(vector<string> tokens);
	double F(vector<string> tokens);
};

