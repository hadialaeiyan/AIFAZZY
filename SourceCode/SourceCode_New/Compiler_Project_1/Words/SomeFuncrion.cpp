#include <string>
#include <vector>
using namespace std;

bool isID(string s) {
        if (s.length() == 9) {
			char c;
            for (unsigned int i = 0; i < s.length(); i++) {
                c = s[i];
                if (!((c >= 'a' && c <= 'z') ||(c >= 'A' && c <= 'Z') || c == '_' || (c >= '0' && c <= '9'))) {
                    return false;
                }
            }
			return true;
		}
        return false;
}


vector<string> split_string_with_E(string s){
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

vector<string> split_string(string s,char c){
	vector<string> data;
	unsigned int lastindex = 0;
	unsigned int i =0 ;
	for(; i<s.length();i++)
		if(s[i]==c)
		{
			data.push_back(s.substr(lastindex,i-lastindex));
			lastindex = i+1;
		}

	data.push_back(s.substr(lastindex,i-lastindex));
	return data;
}

string split_first_string(string s,char c){
	for(unsigned int i=0; i<s.length();i++)
		if(s[i]==c)
		{
			return s.substr(0,i);
		}
		return "nofound";
}

long StringHexTointConverter(string hex) {
	if(hex[hex.length()-1]=='h'||hex[hex.length()-1]=='H')
		hex = hex.substr(0,hex.length()-1);
    char * p;
    long n = strtol( hex.c_str(), & p, 16 );
    if ( * p != 0 ) { //my bad edit was here
        cout << "not a number" << endl;
    }
	return n;
}

short StringHexToshortConverter(string hex) {
	if(hex[hex.length()-1]=='h'||hex[hex.length()-1]=='H')
		hex = hex.substr(0,hex.length()-1);
    char * p;
	short n = strtol( hex.c_str(), & p, 16 );
    if ( * p != 0 ) { //my bad edit was here
        cout << "not a number" << endl;
    }
	return n;
}

std::string int_to_hex( int i )
{
  stringstream s;
  s << std::setfill ('0') << std::setw(sizeof(int)*2) 
         << std::hex << i;
  string str = s.str();
  transform(str.begin(), str.end(),str.begin(), ::toupper);
  return str;
}

bool isHexnumber(string s){
	char c;
    for (unsigned int i = 0; i < s.length(); i++) {
        c = s[i];
        if (!((c >= 'a' && c <= 'f') ||(c >= 'A' && c <= 'F') || c == 'h'|| c == 'H' || (c >= '0' && c <= '9'))) {
            return false;
        }
    }
	return true;
}

bool isregister(string s){
	if(
		///x86 registers
		s.compare("eax")==0||s.compare("ebx")==0||s.compare("ecx")==0||s.compare("edx")==0
		||s.compare("ax")==0||s.compare("bx")==0||s.compare("cx")==0||s.compare("dx")==0
		||s.compare("ah")==0||s.compare("bh")==0||s.compare("ch")==0||s.compare("dh")==0
		||s.compare("al")==0||s.compare("bl")==0||s.compare("cl")==0||s.compare("dl")==0
		||s.compare("cs")==0||s.compare("ds")==0||s.compare("es")==0||s.compare("fs")==0||s.compare("gs")==0||s.compare("ss")==0
		||s.compare("edi")==0||s.compare("ebp")==0||s.compare("eip")==0||s.compare("esp")==0||s.compare("esi")==0
		||s.compare("sil")==0||s.compare("dil")==0||s.compare("bpl")==0||s.compare("spl")==0
		||s.compare("si")==0||s.compare("di")==0||s.compare("bp")==0||s.compare("sp")==0
		//flags registers
		||s.compare("cf")==0||s.compare("pf")==0||s.compare("af")==0||s.compare("zf")==0||s.compare("sf")==0
		||s.compare("tf")==0||s.compare("if")==0||s.compare("df")==0||s.compare("of")==0||s.compare("iopl")==0
		||s.compare("nt")==0||s.compare("rf")==0||s.compare("vm")==0||s.compare("ac")==0||s.compare("vif")==0||s.compare("id")==0
		//x64 register
		||s.compare("rax")==0||s.compare("rdx")==0||s.compare("rcx")==0||s.compare("rdx")==0
		||s.compare("rsi")==0||s.compare("rdi")==0||s.compare("rbp")==0||s.compare("rsp")==0
		||s.compare("r8")==0||s.compare("r9")==0||s.compare("r10")==0||s.compare("r11")==0||s.compare("r12")==0||s.compare("r13")==0||s.compare("r14")==0||s.compare("r15")==0
		||s.compare("r8d")==0||s.compare("r9d")==0||s.compare("r10d")==0||s.compare("r11d")==0||s.compare("r12d")==0||s.compare("r13d")==0||s.compare("r14d")==0||s.compare("r15d")==0
		||s.compare("r8w")==0||s.compare("r9w")==0||s.compare("r10w")==0||s.compare("r11w")==0||s.compare("r12w")==0||s.compare("r13w")==0||s.compare("r14w")==0||s.compare("r15w")==0
		||s.compare("r8b")==0||s.compare("r9b")==0||s.compare("r10b")==0||s.compare("r11b")==0||s.compare("r12b")==0||s.compare("r13b")==0||s.compare("r14b")==0||s.compare("r15b")==0
		)
			return true;
		return false;
}

bool isptr(string s){
	int location = s.find("ptr");
	if(location != string::npos)
	{
		return true;
	}
	return false;
}

string deletespace(string s) {
    string r = "";
	for (unsigned int i = 0; i < s.length(); i++) {
        if (s[i] != ' ' && s[i] != '\t' && s[i] != '\n' && s[i] != '\r') {
            r += s[i];
        }
    }
    return r;
}