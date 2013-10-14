#pragma once

#include <iostream>
#include <string>
#include <vector>
using namespace std;

template<class T>
class HDICTIONARYNODEKEY
{
public:
	HDICTIONARYNODEKEY();
	HDICTIONARYNODEKEY(T __value_);
	~HDICTIONARYNODEKEY();

	T _value_;
private:

};

template<class T>
HDICTIONARYNODEKEY<T>::HDICTIONARYNODEKEY(T __value_)
{
	this->_value_ = __value_;
}

template<class T>
HDICTIONARYNODEKEY<T>::HDICTIONARYNODEKEY()
{
}

template<class T>
HDICTIONARYNODEKEY<T>::~HDICTIONARYNODEKEY()
{
}

template <class T>
class HDICTIONARYNODE
{
public:
	HDICTIONARYNODE();
	HDICTIONARYNODE(char _Value_);
	~HDICTIONARYNODE();
	bool isleaf();

	char _Value_;
	HDICTIONARYNODEKEY<T>* key;
	vector<HDICTIONARYNODE<T>*> childs;
	HDICTIONARYNODE<T>* parrent;
private:

};

template <class T>
HDICTIONARYNODE<T>::HDICTIONARYNODE()
{
	parrent = NULL;
	key= NULL;
}

template <class T>
HDICTIONARYNODE<T>::HDICTIONARYNODE(char __Value_)
{
	parrent = NULL;
	key= NULL;
	this->_Value_ = __Value_;
}


template<class T>
bool HDICTIONARYNODE<T>::isleaf(){
	return childs.size()==0;
}

template <class T>
HDICTIONARYNODE<T>::~HDICTIONARYNODE()
{
}

template <class T>
class HDICTIONARY
{
public:
	HDICTIONARY();
	~HDICTIONARY();
	void add(string s,HDICTIONARYNODEKEY<T>* key);
	bool find(string s);
	void deletewords(string s);
	HDICTIONARYNODEKEY<T>* getkey(string s);

private:
	vector<HDICTIONARYNODE<T>*> hashtable;
};

template <class T>
HDICTIONARY<T>::HDICTIONARY()
{
}

template <class T>
void HDICTIONARY<T>::add(string s,HDICTIONARYNODEKEY<T>* key){
	if(s.length()>0){
		bool find = false;
		char c = s[0];
		unsigned int i=0;
		for(;i<hashtable.size();i++)
		{
			if(hashtable[i]->_Value_ == c)
			{
				find = true;
				break;
			}
		}
		if(!find){
			HDICTIONARYNODE<T>* w = new HDICTIONARYNODE<T>(c);
			HDICTIONARYNODE<T>* lastw;
			this->hashtable.push_back(w);
			for(unsigned int k = 1;k<s.length();k++){
				c = s[k];
				lastw = w;
				w = new HDICTIONARYNODE<T>(c);
				lastw->childs.push_back(w);
				w->parrent = lastw;
			}
			w->key = key;
		}
		else{	
			HDICTIONARYNODE<T>* w = hashtable[i];
			unsigned int k = 1;
			find = true;
			for(;k<s.length()&&find;k++){
				find = false;
				c = s[k];
				for(unsigned int j=0;j<w->childs.size();j++)
					if(c == w->childs.at(j)->_Value_)
					{
						find = true;
						w = w->childs.at(j);
						break;
					}
			}
			if(k <=s.length()){
				if(!find){
					HDICTIONARYNODE<T>* w2 = new HDICTIONARYNODE<T>(c);
					HDICTIONARYNODE<T>* lastw;
					w->childs.push_back(w2);
					w2->parrent = w;
					for(unsigned int k1 = k;k1<s.length();k1++){
						c = s[k1];
						lastw = w2;
						w2 = new HDICTIONARYNODE<T>(c);
						lastw->childs.push_back(w2);
						w2->parrent = lastw;
					}
					w2->key = key;
				}
				else{
					w->key = key;
				}
			}
		}
	}
}

template <class T>
bool HDICTIONARY<T>::find(string s){
	if(s.length()>0){
		bool find = false;
		char c = s[0];
		unsigned int i=0;
		for(;i<hashtable.size();i++)
		{
			if(hashtable[i]->_Value_ == c)
			{
				find = true;
				break;
			}
		}
		if(find){
			HDICTIONARYNODE<T>* w = hashtable[i];
			unsigned int k = 1;
			find = true;
			for(;k<s.length()&&find;k++){
				find = false;
				c = s[k];
				for(unsigned int j=0;j<w->childs.size();j++)
					if(c == w->childs.at(j)->_Value_)
					{
						find = true;
						w = w->childs.at(j);
						break;
					}
			}
			if(k == s.length() && w->key!=NULL) return true;
		}
	}
	return false;
}

template <class T>
void HDICTIONARY<T>::deletewords(string s){
	if(find(s))
	{
		bool find = false;
		char c = s[0];
		unsigned int i=0;
		for(;i<hashtable.size();i++)
		{
			if(hashtable[i]->_Value_ == c)
			{
				find = true;
				break;
			}
		}
		if(find){
			HDICTIONARYNODE<T>* w = hashtable[i];
			unsigned int k = 1;
			find = true;
			for(;k<s.length()&&find;k++){
				find = false;
				c = s[k];
				for(unsigned int j=0;j<w->childs.size();j++)
					if(c == w->childs.at(j)->_Value_)
					{
						find = true;
						w = w->childs.at(j);
						break;
					}
			}
			if(k == s.length()&&w->key!=NULL){
				delete w->key;
				w->key = NULL;
				while(w!=NULL && w->key==NULL && w->childs.size()==0){
					HDICTIONARYNODE<T>* p = w->parrent;
					if(p!=NULL)
						for(unsigned int f=0;f<p->childs.size();f++)
							if(p->childs.at(f)==w)
								p->childs.erase(p->childs.begin()+f);
					delete w;
					w=p;
				}
			}
		}
	}
}

template <class T>
HDICTIONARYNODEKEY<T>* HDICTIONARY<T>::getkey(string s){
	if(s.length()>0){
		bool find = false;
		char c = s[0];
		unsigned int i=0;
		for(;i<hashtable.size();i++)
		{
			if(hashtable[i]->_Value_ == c)
			{
				find = true;
				break;
			}
		}
		if(find){
			HDICTIONARYNODE<T>* w = hashtable[i];
			unsigned int k = 1;
			find = true;
			for(;k<s.length()&&find;k++){
				find = false;
				c = s[k];
				for(unsigned int j=0;j<w->childs.size();j++)
					if(c == w->childs.at(j)->_Value_)
					{
						find = true;
						w = w->childs.at(j);
						break;
					}
			}
			if(k == s.length()) return w->key;
		}
	}
	return NULL;
}

template <class T>
HDICTIONARY<T>::~HDICTIONARY()
{
	for(unsigned int i=0;i<hashtable.size();i++)
		delete hashtable.at(i);
	hashtable.clear();
}