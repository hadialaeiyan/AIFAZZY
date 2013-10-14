#pragma once
#include <limits>
#include <string>
#include <sstream>
using namespace std;

class Range
{
public:
	Range(void);
	Range(double _Min_value,double _Max_value);
	Range(string s);
	~Range(void);
	bool hasonevalue(); // return ((Max_value - Min_value) == 0)
	Range* operatorUP(Range x); // paper : ref ->abstract interpretation : a unified model for static analysis of programs by construction or approximation of foxpoints. cousot
	Range* operatorDOWN(Range x); // paper : ref ->abstract interpretation : a unified model for static analysis of programs by construction or approximation of foxpoints. cousot
	Range* operatorADD(Range x);
	Range* operatorADD(double x);
	Range* operatorSUB(Range x);
	Range* operatorMUL(Range x);
	Range* operatorDIV(Range x);
	Range* operatorUNION(Range x);
	Range* operatorSUBESCRIPTION(Range x);
	void set(double _Min_value,double _Max_value);
	void set(double Max_Min_value); // max = min = max_min;
	double get__Max_value();
	double get__Min_value();
	bool valueisdef(){return (_valueisdef);}
	Range* copy();
	string tostring();
	double mid(){return (Min_value+Max_value)/2;}
	double Magnitude();
	double Width(){return (Max_value - Min_value);}
	bool operator== (Range R);
	bool operator!= (Range R);
	bool operator<= (Range R);
	bool operator< (Range R);
	bool operator>= (Range R);
	bool operator> (Range R);
	bool operator== (int R);
	bool operator!= (int R);
	bool operator<= (int R);
	bool operator< (int R);
	bool operator>= (int R);
	bool operator> (int R);
	Range* operator& (Range R);
	Range* operatorAND (Range R);
	static double getMax_doublevalue();
	static double getMin_doublevalue();
	bool UPERBOUNDISINF();
	void operatorADDwithselt(int value);
	void operatorADDwithselt(Range value);
	void operatorSUBwithselt(int value);
	void operatorSUBwithselt(Range value);
	void copyto(Range* r);
	
private:

	double ___min(double x,double y);
	double ___min(double x,double y,double z,double w);
	double ___max(double x,double y);
	double ___max(double x,double y,double z,double w);
	bool canDIV(Range R);

	bool _valueisdef;
	double Max_value,Min_value;
};

