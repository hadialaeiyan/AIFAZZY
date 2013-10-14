// Coded in Visual Studio 2010 by: AmirGooran Oorimi

#include <Windows.h>
#include <Shlwapi.h>
#include <tchar.h>
#include <stdio.h>
#include <shlobj.h>  
#define BEA_ENGINE_STATIC
#define BEA_USE_STDCALL
#include ".\HEADERS\BeaEngine.h"
#include "resource.h"
#include "Disassemble.h"
#include "FGD.h"
#include "FileDialog.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

LRESULT CALLBACK MainWindowDlgProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK AboutWindowDlgProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK LocationWindowDlgProc(HWND,UINT, WPARAM, LPARAM);
LRESULT CALLBACK DisassemblerConfigWindowDlgProc(HWND,UINT, WPARAM, LPARAM);
void CreateCFG(PTCHAR,PTCHAR);
void GetExeDirectory(PTCHAR);

int ConvertOffsetToRVA(int);
int ConvertRVAToOffset(int);
int DisassembleOneInstruction(char *, int (*)(void));
int imagebase;
HWND hWnd_main;//handle panjereye asli.
extern bool draw_cfg;//moteghaieri ke neshan midahad aya cfg bayad rasm shavad ya kheir.
extern bool cfg_pdf;
extern bool cfg_openpdf;
void* strBigBuffer;
DISASM MyDisasm;
string filename_exe;
int max_value = 1000;
CFGD cfg;

//File information variables:
IMAGE_DOS_HEADER fileinfo_imgdosheader;
IMAGE_NT_HEADERS fileinfo_imgntheader;
IMAGE_SECTION_HEADER* fileinfo_imgsectionheader;
bool imgdos_valid,imgntheader_valid,imgsection_valid; //in moteghaierha neshan midahand ke aya dadehaye darun structurehaye bala valid hastand,ya kheir.




//baraye daryafte ettelaat file estefade mishavad.
void GetFileInformation(HWND hDlg,char* file_path)
{
	HANDLE hMapping;
	LPVOID pMapping,pNTSignature,pSectionTable;
	HANDLE hFile=CreateFile(file_path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{//File can't be opened
		SetDlgItemText(hDlg,IDC_FILENAME,"File Can't be opened");
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"Unknown");
		SetDlgItemText(hDlg,IDC_EPRVA,"Unknown");
		SetDlgItemText(hDlg,IDC_EPOFFSET,"Unknown");
		SetDlgItemText(hDlg,IDC_SUBSYSTEM,"Unknown");
		SetDlgItemText(hDlg,IDC_BASEOFCODE,"Unknown");
		SetDlgItemText(hDlg,IDC_BASEOFDATA,"Unknown");
		imgdos_valid=false;
		imgntheader_valid=false;
		imgsection_valid=false;
		return;
	}
	if(!(hMapping=CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,0)))
	{
		SetDlgItemText(hDlg,IDC_FILENAME,"File Can't be mapped");
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"Unknown");
		SetDlgItemText(hDlg,IDC_EPRVA,"Unknown");
		SetDlgItemText(hDlg,IDC_EPOFFSET,"Unknown");
		SetDlgItemText(hDlg,IDC_SUBSYSTEM,"Unknown");
		SetDlgItemText(hDlg,IDC_BASEOFCODE,"Unknown");
		SetDlgItemText(hDlg,IDC_BASEOFDATA,"Unknown");
		imgdos_valid=false;
		imgntheader_valid=false;
		imgsection_valid=false;
		CloseHandle(hFile);
		return;
	}
	if(!(pMapping=MapViewOfFile(hMapping,FILE_MAP_READ,0,0,0)))
	{
		SetDlgItemText(hDlg,IDC_FILENAME,"File Can't be mapped");
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"Unknown");
		SetDlgItemText(hDlg,IDC_EPRVA,"Unknown");
		SetDlgItemText(hDlg,IDC_EPOFFSET,"Unknown");
		SetDlgItemText(hDlg,IDC_SUBSYSTEM,"Unknown");
		SetDlgItemText(hDlg,IDC_BASEOFCODE,"Unknown");
		SetDlgItemText(hDlg,IDC_BASEOFDATA,"Unknown");
		imgdos_valid=false;
		imgntheader_valid=false;
		imgsection_valid=false;
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return;
	}
	if(((IMAGE_DOS_HEADER*)pMapping)->e_magic!=IMAGE_DOS_SIGNATURE)
	{
		SetDlgItemText(hDlg,IDC_FILENAME,"Not an executable");
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"Unknown");
		SetDlgItemText(hDlg,IDC_EPRVA,"Unknown");
		SetDlgItemText(hDlg,IDC_EPOFFSET,"Unknown");
		SetDlgItemText(hDlg,IDC_SUBSYSTEM,"Unknown");
		SetDlgItemText(hDlg,IDC_BASEOFCODE,"Unknown");
		SetDlgItemText(hDlg,IDC_BASEOFDATA,"Unknown");
		imgdos_valid=false;
		imgntheader_valid=false;
		imgsection_valid=false;
		UnmapViewOfFile(pMapping);
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return;
	}
	fileinfo_imgdosheader=*((IMAGE_DOS_HEADER*)pMapping);
	imgdos_valid=true;
	pNTSignature = (char*)pMapping + ((int)((IMAGE_DOS_HEADER*)pMapping)->e_lfanew);
	IMAGE_NT_HEADERS inh_NT=*(IMAGE_NT_HEADERS*)pNTSignature;
	if(inh_NT.Signature != IMAGE_NT_SIGNATURE)
	{
		SetDlgItemText(hDlg,IDC_FILENAME,"Not an NT executable");
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"Unknown");
		SetDlgItemText(hDlg,IDC_EPRVA,"Unknown");
		SetDlgItemText(hDlg,IDC_EPOFFSET,"Unknown");
		SetDlgItemText(hDlg,IDC_SUBSYSTEM,"Unknown");
		SetDlgItemText(hDlg,IDC_BASEOFCODE,"Unknown");
		SetDlgItemText(hDlg,IDC_BASEOFDATA,"Unknown");
		imgntheader_valid=false;
		imgsection_valid=false;
		UnmapViewOfFile(pMapping);
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return;
	}
	fileinfo_imgntheader=inh_NT;
	imgntheader_valid=true;
	fileinfo_imgsectionheader=new IMAGE_SECTION_HEADER[fileinfo_imgntheader.FileHeader.NumberOfSections];
	pSectionTable=(char*)pNTSignature + sizeof(IMAGE_NT_HEADERS);
	IMAGE_SECTION_HEADER ish_section=*(IMAGE_SECTION_HEADER*)pSectionTable;
	fileinfo_imgsectionheader[0]=ish_section;
	for(int i=1;i<fileinfo_imgntheader.FileHeader.NumberOfSections;i++)
	{
		pSectionTable= (char*) pSectionTable + sizeof(IMAGE_SECTION_HEADER);
		IMAGE_SECTION_HEADER ish_section=*(IMAGE_SECTION_HEADER*)pSectionTable;
		fileinfo_imgsectionheader[i]=ish_section;
	}
	imgsection_valid=true;
	//etelaat file ba movafaghit khande shod,handlehaye baz shode ra mibandim:
	UnmapViewOfFile(pMapping);
	CloseHandle(hMapping);
	CloseHandle(hFile);
	//dar inja ettelaate marbut be file ra dar dialoge barname namayesh midahim:
	char strBuffer[256];
	strcpy(strBuffer,file_path);
	strrev(strBuffer);
	int mahal=0;
	while(strBuffer[mahal]!='\\' && strBuffer[mahal]!=NULL)
		mahal++;
	strBuffer[mahal]=NULL;
	strrev(strBuffer);
	SetDlgItemText(hDlg,IDC_FILENAME,strBuffer);
	switch(fileinfo_imgntheader.FileHeader.Machine)
	{
	case 0:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"IMAGE_FILE_MACHINE_UNKNOWN");
		break;
	case 0x1d3:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"IMAGE_FILE_MACHINE_AM33");
		break;
	case 0x8664:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"IMAGE_FILE_MACHINE_AMD64");
		break;
	case 0x1c0:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"IMAGE_FILE_MACHINE_ARM");
		break;
	case 0x1c4:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"IMAGE_FILE_MACHINE_ARMNT");
		break;
	case 0xaa64:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"IMAGE_FILE_MACHINE_ARM64");
		break;
	case 0xebc:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"IMAGE_FILE_MACHINE_EBC");
		break;
	case 0x14c:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"IMAGE_FILE_MACHINE_I386");
		break;
	case 0x200:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"IMAGE_FILE_MACHINE_IA64");
		break;
	case 0x9041:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"IMAGE_FILE_MACHINE_M32R");
		break;
	case 0x266:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"IMAGE_FILE_MACHINE_MIPS16");
		break;
	case 0x366:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"IMAGE_FILE_MACHINE_MIPSFPU");
		break;
	case 0x466:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"IMAGE_FILE_MACHINE_MIPSFPU16");
		break;
	case 0x1f0:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"IMAGE_FILE_MACHINE_POWERPC");
		break;
	case 0x1f1:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"IMAGE_FILE_MACHINE_POWERPCFP");
		break;
	case 0x166:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"IMAGE_FILE_MACHINE_R4000");
		break;
	case 0x1a2:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"IMAGE_FILE_MACHINE_SH3");
		break;
	case 0x1a3:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"IMAGE_FILE_MACHINE_SH3DSP");
		break;
	case 0x1a6:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"IMAGE_FILE_MACHINE_SH4");
		break;
	case 0x1a8:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"IMAGE_FILE_MACHINE_SH5");
		break;
	case 0x1c2:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"IMAGE_FILE_MACHINE_THUMB");
		break;
	case 0x169:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"IMAGE_FILE_MACHINE_WCEMIPSV2");
		break;
	default:
		SetDlgItemText(hDlg,IDC_ARCHITECTURE,"Unknown");
		break;
	}
	sprintf(strBuffer,"0x%.8X",fileinfo_imgntheader.OptionalHeader.AddressOfEntryPoint);
	SetDlgItemText(hDlg,IDC_EPRVA,strBuffer);
	sprintf(strBuffer,"0x%.8X",ConvertRVAToOffset(fileinfo_imgntheader.OptionalHeader.AddressOfEntryPoint));
	SetDlgItemText(hDlg,IDC_EPOFFSET,strBuffer);
	switch(fileinfo_imgntheader.OptionalHeader.Subsystem)
	{
	case 0:
		SetDlgItemText(hDlg,IDC_SUBSYSTEM,"IMAGE_SUBSYSTEM_UNKNOWN");
		break;
	case 1:
		SetDlgItemText(hDlg,IDC_SUBSYSTEM,"IMAGE_SUBSYSTEM_NATIVE");
		break;
	case 2:
		SetDlgItemText(hDlg,IDC_SUBSYSTEM,"IMAGE_SUBSYSTEM_WINDOWS_GUI");
		break;
	case 3:
		SetDlgItemText(hDlg,IDC_SUBSYSTEM,"IMAGE_SUBSYSTEM_WINDOWS_CUI");
		break;
	case 7:
		SetDlgItemText(hDlg,IDC_SUBSYSTEM,"IMAGE_SUBSYSTEM_POSIX_CUI");
		break;
	case 9:
		SetDlgItemText(hDlg,IDC_SUBSYSTEM,"IMAGE_SUBSYSTEM_WINDOWS_CE_GUI");
		break;
	case 10:
		SetDlgItemText(hDlg,IDC_SUBSYSTEM,"IMAGE_SUBSYSTEM_EFI_APPLICATION");
		break;
	case 11:
		SetDlgItemText(hDlg,IDC_SUBSYSTEM,"IMAGE_SUBSYSTEM_EFI_BOOT_ SERVICE_DRIVER");
		break;
	case 12:
		SetDlgItemText(hDlg,IDC_SUBSYSTEM,"IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER");
		break;
	case 13:
		SetDlgItemText(hDlg,IDC_SUBSYSTEM,"IMAGE_SUBSYSTEM_EFI_ROM");
		break;
	case 14:
		SetDlgItemText(hDlg,IDC_SUBSYSTEM,"IMAGE_SUBSYSTEM_XBOX");
		break;
	}
	sprintf(strBuffer,"0x%.8X",fileinfo_imgntheader.OptionalHeader.BaseOfCode);
	SetDlgItemText(hDlg,IDC_BASEOFCODE,strBuffer);
	sprintf(strBuffer,"0x%.8X",fileinfo_imgntheader.OptionalHeader.BaseOfData);
	SetDlgItemText(hDlg,IDC_BASEOFDATA,strBuffer);
	return;
}
vector<int> get_next_add_inits(vector<string> s){
	int address = 0;
	vector<int> ret ;
	for(int i=0;i<s.size();i++)
	{
		address = 0;
		string strBuffer_2 = s.at(i).c_str();
		strBuffer_2.insert(strBuffer_2.length(),"h");
		if(strBuffer_2[strBuffer_2.length()-1]=='h' || strBuffer_2[strBuffer_2.length()-1]=='H')//agar enteha harfe h dashte bashim,yani voroodi dar mabnaye 16 ast.
			sscanf(strBuffer_2.c_str(),"%x",&address);
		else
			sscanf(strBuffer_2.c_str(),"%d",&address);
		address=ConvertRVAToOffset(address-imagebase);
		bool find_ = false;
		for(int i=0;i<ret.size();i++)
		{
			if(ret.at(i) == address || ret.at(i) + 1000 > address){
				find_ = true;
				break;
			}
		}
		if(!find_)
			ret.push_back(address);
	}
	return ret;
}
void MakeCFG(const char *inistractions){
	cfg.DOCFG(inistractions,false,filename_exe);//cfg ra misazad,va an ra ejra mikonad.
}

void Anti_AntiDisassemblerCode(char *StartCodeSection,char *EndCodeSection, int (*Virtual_Address)(void),char* outed,bool farakhanishode_dll=true)
{
	int Original_Virtual_Address=(int)Virtual_Address;
	char* start=StartCodeSection;
	strcpy(outed,"");
	char opcodes[100];
	char outing[100];
	char tmp[20];
	int len=-1;//-1 yani hanuz shoroo nashode ast.
	char strBuffer[256],strBuffer_2[256];//bufferi baraye zakhireye dadehaye movaghati.
	HANDLE  hLibrary;
	DWORD dwLastErr;
	typedef BOOL (WINAPI * DLL_PROCEDURE) (PTCHAR,PTCHAR,int (*)(void),PTCHAR,void (*)(char *,char *, int (*)(void),char*,bool),void (*) (PTCHAR,PTCHAR),int,PTCHAR) ;
	DLL_PROCEDURE dll_procedure;//adrese tabe plugin dar in moteghaier zakhire mishavad.

	while(len!=0 && start <= EndCodeSection)
	{
		len=DisassembleOneInstruction(start,Virtual_Address);
		strcpy_s(opcodes,"");
		for(int i=0;i<min(len,EndCodeSection-start);i++)//baraye daryafte opcode
		{
			sprintf_s(tmp,"%.2hhX",(unsigned char)*((char*)start+i));
			strcat_s(opcodes,tmp);
		}
			
		if(start+len<=EndCodeSection)//agar hanuz be akharin dasture narecide bashim.
		{
			sprintf(outing,"%.8X: %s opcode: %s\r\n",(int) Virtual_Address, MyDisasm.CompleteInstr,opcodes);
			Virtual_Address = (int(*)())((int)Virtual_Address + len);
			strcat(outed,outing);
			start+=len;
		}
		else//namayeshe bytehaye baghimande:(be alaeiyan begam injaro dorost kone)
		{
			sprintf(outing,"%.8X: ??? opcode: %s\r\n",(int) Virtual_Address, opcodes);
			strcat(outed,outing);
			break;
		}
	}
	if(farakhanishode_dll==false)
	{
		GetPrivateProfileString("Plugins","Count","0",strBuffer,256,"disassemblerconfig.ini");
		int tedad=atoi(strBuffer);
		for(int i=0;i<tedad;i++)///be azaye har plugin yek bar ejra mishavad.
		{
			strcpy(strBuffer,"Num");
			itoa(i,strBuffer_2,10);
			strcat(strBuffer,strBuffer_2);
			GetPrivateProfileString("Plugins",strBuffer,"",strBuffer_2,256,"disassemblerconfig.ini");
			strcpy(strBuffer,"Dlls\\");
			strcat(strBuffer,strBuffer_2);
			hLibrary=LoadLibrary(strBuffer);
			if(hLibrary)
			{
				dll_procedure=(DLL_PROCEDURE)GetProcAddress((HMODULE)hLibrary,"_AntiAntiDisassemble@32");
				if(!dll_procedure)
				{
					strcpy(strBuffer_2,"áÇíä ÒíÑ ÏÇÑÇí ãÔ˜á ãí ÈÇÔÏ¡ äãí ÊæÇäã Âä ÑÇ ÈÇÑÐÇÑí ˜äã\n");
					strcat(strBuffer_2,strBuffer);
					MessageBox(GetActiveWindow(),strBuffer_2,"ÇÎØÇÑ",MB_ICONINFORMATION);
					continue;
				}
				strBigBuffer=malloc(strlen(outed)+0x1000);
				bool ret_value=dll_procedure(StartCodeSection,EndCodeSection,(int(*)())Original_Virtual_Address,outed,Anti_AntiDisassemblerCode,CreateCFG,EndCodeSection-StartCodeSection,(PTCHAR)strBigBuffer);//farakhanie file dll jahate antianti disassembling
				if(ret_value==true)
					strcpy(outed,(PTCHAR)strBigBuffer);
				FreeLibrary((HMODULE)hLibrary);
			}
			else
			{
				strcpy(strBuffer_2,"áÇíä ÒíÑ ÏÇÑÇí ãÔ˜á ãí ÈÇÔÏ¡ äãí ÊæÇäã Âä ÑÇ ÈÇÑÐÇÑí ˜äã\n");
				strcat(strBuffer_2,strBuffer);
				MessageBox(GetActiveWindow(),strBuffer_2,"ÇÎØÇÑ",MB_ICONINFORMATION);
				continue;
			}
		}
	}
}

//in tabe baraye sakhte cfg ast. in tabe tavasote pluginha morede estefade gharar migirad:
void CreateCFG(PTCHAR disassembled_code,PTCHAR return_value)
{
	cfg.DOCFG(disassembled_code,false,filename_exe);//cfg ra misazad,va an ra ejra mikonad.
	cfg.getallbb(return_value);
	cfg.clear();
}

int DisassembleOneInstruction(char *StartCodeSection, int (*Virtual_Address)(void)) 
{
	int count_disassemble=0;
	DisAssemble Disassemble_Code;//tedad khotooe disassemble shode dar in adress gharar migirad.
	int len;
	BOOL Error = 0; 
	char tmp_buffer[256];//baraye zakhire dadehaye movaghati dar in tabe.
	(void) memset (&MyDisasm, 0, sizeof(DISASM));
	MyDisasm.EIP = (int) StartCodeSection;
	MyDisasm.VirtualAddr = (long long) Virtual_Address; 
	GetDlgItemText(hWnd_main,IDC_ARCHITECTURE,tmp_buffer,256);
	if(strcmp(tmp_buffer,"IMAGE_FILE_MACHINE_I386")==0)
		MyDisasm.Archi = 0;
	else
		MyDisasm.Archi=64;
	MyDisasm.SecurityBlock = (int) 15;       
	len = Disasm(&MyDisasm);     
	if (len == OUT_OF_BLOCK)    
		return 0;        
	else if (len == UNKNOWN_OPCODE)
		return 0;        
	else
	{
		//baraye gereftane opcode dastur
		MyDisasm.EIP = MyDisasm.EIP + len;    
		MyDisasm.VirtualAddr = MyDisasm.VirtualAddr + len;      
	}
	return len; 
} 

void Disassembler(HWND hDlg,int address,PTCHAR input_file,PTCHAR output)//Disassembler Function
{
	lstrcpy(output,TEXT(""));
	DISASM MyDisasm;
	DWORD lpNumberReaded;
	int i = 0;
	int Error = 0;

	strBigBuffer=malloc(max_value);
	memset(strBigBuffer,0x90,max_value);
	(void) memset (&MyDisasm, 0, sizeof(DISASM));
	HANDLE hFile=CreateFile(input_file,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(SetFilePointer(hFile,address,0,FILE_BEGIN)==INVALID_SET_FILE_POINTER)
	{
		MessageBox(hDlg,"ÎØÇíí ÏÑ åäÇã ÎæÇäÏä ÝÇíá ÑÎ ÏÇÏå ÇÓÊ. ÂíÇ ÂÏÑÓ åÇ ÑÇ ÏÑÓÊ æÇÑÏ ˜ÑÏå ÇíÏ¿","ÎØÇ",MB_ICONWARNING);
		return;
	}
	ReadFile(hFile,strBigBuffer,max_value,&lpNumberReaded,NULL);
	CloseHandle(hFile);
	Anti_AntiDisassemblerCode((char*)strBigBuffer,(char*)strBigBuffer+max_value,(int(*)())(imagebase+ConvertOffsetToRVA(address)),output,false);
	
}

int ConvertRVAToOffset(int address)
{
	if(imgsection_valid==false || imgntheader_valid==false)//section table ya nt header motabar nist.
		return 0;
	int iNumSections=fileinfo_imgntheader.FileHeader.NumberOfSections;
	int i=0;
	for(i=0;i<iNumSections;i++)
	{
		if(fileinfo_imgsectionheader[i].VirtualAddress + fileinfo_imgsectionheader[i].SizeOfRawData>address)
			break;
	}
	if(i==iNumSections)
		return 0;
	else
		return address - fileinfo_imgsectionheader[i].VirtualAddress + fileinfo_imgsectionheader[i].PointerToRawData;
}

int ConvertOffsetToRVA(int address)
{
	if(imgsection_valid==false || imgntheader_valid==false)//section table ya nt header motabar nist.
		return 0;
	int iNumSections=fileinfo_imgntheader.FileHeader.NumberOfSections;
	int i=0;
	for(i=0;i<iNumSections;i++)
	{
		if(fileinfo_imgsectionheader[i].PointerToRawData+fileinfo_imgsectionheader[i].SizeOfRawData>address)
			break;
	}
	if(i==iNumSections)
		return 0;
	else
		return address - fileinfo_imgsectionheader[i].PointerToRawData + fileinfo_imgsectionheader[i].VirtualAddress;  
}

//ÔÑæÚ ˜ÏåÇí ÈÑäÇãå
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, int)
{
	return DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_MAINWINDOW),NULL,(DLGPROC)MainWindowDlgProc,NULL);
}

//ÑæÇá ÑÏÇÒÔ íÛÇã åÇí ÑÓíÏå Èå äÑã ÇÝÒÇÑ
LRESULT CALLBACK MainWindowDlgProc(HWND hDlg,UINT message, WPARAM wParam, LPARAM lParam)
{
	static TCHAR szFileName[MAX_PATH],szFileTitle[MAX_PATH],szDialogTitle[MAX_PATH];//open ÈÑÇí ÐÎíÑå äÇã æ ÚäæÇä ˜ÇÏÑåÇí ãÍÇæÑå Çí
	TCHAR strBuffer[100000],strBuffer_2[100000];//ÈÇÝÑ ÑæÇá¡ ÈÑÇí ÐÎíÑå ÇØáÇÚÇÊ ãæÞÊí
	HMENU hMenu;
	HBRUSH hB=NULL;

	switch(message)
	{
	case WM_INITDIALOG:
		InitCommonControls();
		SetClassLongA(hDlg,GCL_HICON,(LONG)LoadIcon(GetModuleHandle(NULL),(LPCTSTR)IDI_ICON1));//ÈÑÇí ÊäÙíã Âí˜ä
		PopFileInitialize(hDlg);	
		SetTimer(hDlg,1,10,NULL); //timer baraye inke bebinim che kelidi feshorde shode ast.
		hWnd_main=hDlg;
		draw_cfg=GetPrivateProfileInt("Features","DRAW_CFG",0,"disassemblerconfig.ini");
		if(draw_cfg)
			CheckMenuItem(GetMenu(hDlg),ID_40004,MF_CHECKED);
		else
		{
			EnableMenuItem(GetMenu(hDlg),ID_40006,MF_DISABLED);
			EnableMenuItem(GetMenu(hDlg),ID_40007,MF_DISABLED);
		}
		cfg_pdf=GetPrivateProfileInt("Features","CFG_PDF",0,"disassemblerconfig.ini");
		if(cfg_pdf)
			CheckMenuItem(GetMenu(hDlg),ID_40006,MF_CHECKED);
		else
		{
			EnableMenuItem(GetMenu(hDlg),ID_40007,MF_DISABLED);
		}
		cfg_openpdf=GetPrivateProfileInt("Features","CFG_PDFOPEN",0,"disassemblerconfig.ini");
		if(cfg_openpdf)
			CheckMenuItem(GetMenu(hDlg),ID_40007,MF_CHECKED);
		//baraye khanadne maghadire save shode text boxha:
		GetPrivateProfileString("Saved","FILE","",strBuffer,256,"disassemblerconfig.ini");
		SetDlgItemText(hDlg,IDC_INPUT,strBuffer);
		GetPrivateProfileString("Saved","ADDRESS","",strBuffer,256,"disassemblerconfig.ini");
		SetDlgItemText(hDlg,IDC_ADDRESS,strBuffer);
		GetPrivateProfileString("Saved","ADDRESSRVA","",strBuffer,256,"disassemblerconfig.ini");
		SetDlgItemText(hDlg,IDC_ADDRESSRVA,strBuffer);

		return TRUE;
	case WM_SETCURSOR://ÈÑÇí ÊäÙíã äÔÇäÑ ãæÓ
		if(LOWORD(lParam)==HTCLIENT && (GetDlgCtrlID((HWND)wParam)==IDC_BROWSE || GetDlgCtrlID((HWND)wParam)==IDC_DOIT || GetDlgCtrlID((HWND)wParam)==IDC_SAVEOUTPUT))
		{
			SetCursor(LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_CURSOR1)));
			SetWindowLongPtr(hDlg,DWLP_MSGRESULT,TRUE);
			return TRUE;
		}
		return TRUE;
	case WM_TIMER: 
		if(wParam==1){//har 1 sanie be inja miaeem
			if(HIWORD(GetKeyState(VK_F1))) //F1 Key
			{
				for(int i=0;i<256;i++)//ebteda kelidhaye feshorde shode ghabli ra pak mikonim.
					strBuffer[i]=0;
				SetKeyboardState((BYTE*)strBuffer);
				//sepas rahnama ra namyesh midhim:
				SendMessage(hDlg,WM_COMMAND,MAKEWPARAM(ID_40011,0),(LPARAM)hDlg);
			}
			else if(HIWORD(GetKeyState(VK_CONTROL)) && HIWORD(GetKeyState(79))) // CTRL + O Key
				SendMessage(hDlg,WM_COMMAND,MAKEWPARAM(IDC_BROWSE,0),(LPARAM)hDlg);
		}
		return TRUE;
	case WM_COMMAND:
		switch(wParam)//ÈÑÇí ãäæåÇ
		{
			case ID_40003://ÎÑæÌ ÇÒ äÑã ÇÝÒÇÑ
				SendMessage(hDlg,WM_CLOSE,NULL,NULL);
				return TRUE;
			case ID_40004://ÑÓã äãæÏÇÑ CFG
				hMenu=GetMenu(hDlg);
				if((GetMenuState(hMenu,ID_40004,MF_BYCOMMAND) & MF_CHECKED) != 0)
				{
					draw_cfg=false;
					cfg_pdf=false;
					cfg_openpdf=false;
					WritePrivateProfileString("Features","DRAW_CFG","0","disassemblerconfig.ini");
					WritePrivateProfileString("Features","CFG_PDF","0","disassemblerconfig.ini");
					WritePrivateProfileString("Features","CFG_PDFOPEN","0","disassemblerconfig.ini");
					CheckMenuItem(hMenu,ID_40004,MF_UNCHECKED);
					CheckMenuItem(hMenu,ID_40006,MF_UNCHECKED);
					CheckMenuItem(hMenu,ID_40007,MF_UNCHECKED);
					EnableMenuItem(hMenu,ID_40006,MF_DISABLED);
					EnableMenuItem(hMenu,ID_40007,MF_DISABLED);
				}
				else
				{
					draw_cfg=true;
					WritePrivateProfileString("Features","DRAW_CFG","1","disassemblerconfig.ini");
					CheckMenuItem(hMenu,ID_40004,MF_CHECKED);
					EnableMenuItem(hMenu,ID_40006,MF_ENABLED);
				}
				return TRUE;
			case ID_40006://ÐÎíÑå ÝÇíá PDF
				hMenu=GetMenu(hDlg);
				if((GetMenuState(hMenu,ID_40006,MF_BYCOMMAND) & MF_CHECKED) != 0)
				{
					cfg_pdf=false;
					cfg_openpdf=false;
					WritePrivateProfileString("Features","CFG_PDF","0","disassemblerconfig.ini");
					WritePrivateProfileString("Features","CFG_PDFOPEN","0","disassemblerconfig.ini");
					CheckMenuItem(hMenu,ID_40006,MF_UNCHECKED);
					CheckMenuItem(hMenu,ID_40007,MF_UNCHECKED);
					EnableMenuItem(hMenu,ID_40007,MF_DISABLED);
				}
				else
				{
					cfg_pdf=true;
					WritePrivateProfileString("Features","CFG_PDF","1","disassemblerconfig.ini");
					CheckMenuItem(hMenu,ID_40006,MF_CHECKED);
					EnableMenuItem(hMenu,ID_40007,MF_ENABLED);
				}
				return TRUE;
			case ID_40007://ÈÇÒ ˜ÑÏä ÝÇíá PDF
				hMenu=GetMenu(hDlg);
				if((GetMenuState(hMenu,ID_40007,MF_BYCOMMAND) & MF_CHECKED) != 0)
				{
					cfg_openpdf=false;
					WritePrivateProfileString("Features","CFG_PDFOPEN","0","disassemblerconfig.ini");
					CheckMenuItem(hMenu,ID_40007,MF_UNCHECKED);
				}
				else
				{
					cfg_openpdf=true;
					WritePrivateProfileString("Features","CFG_PDFOPEN","1","disassemblerconfig.ini");
					CheckMenuItem(hMenu,ID_40007,MF_CHECKED);
				}
				return TRUE;
			case ID_40008://ÊäÙíãÇÊ ãÑÈæØ Èå ãÍá ÐÎíÑå ÓÇÒí ÎÑæÌí
				DialogBoxParam(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_LOCATION),hDlg,(DLGPROC)LocationWindowDlgProc,NULL);
				return TRUE;
			case ID_40009:
				DialogBoxParam(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_DISASSEMBLERCONFIG),hDlg,(DLGPROC)DisassemblerConfigWindowDlgProc,NULL);
				return TRUE;
			case ID_40001://ÈÇÒ ˜ÑÏä ÝÇíá ÇÌÑÇíí
				SendMessage(hDlg,WM_COMMAND,MAKEWPARAM(IDC_BROWSE,0),(LPARAM)hDlg);
				return TRUE;
			case ID_40011://ÑÇåäãÇí äÑã ÇÝÒÇÑ
				if(PathFileExists("Help.pdf")==TRUE)
					ShellExecute(NULL, "OPEN", "Help.pdf" , NULL,NULL, SW_SHOWNORMAL);
				else
					MessageBox(hDlg,"ÑÇåäãÇí äÑã ÇÝÒÇÑ íÇÝÊ äÔÏ","ÎØÇ",MB_ICONWARNING);
				return TRUE;
			case ID_40010://ÏÑÈÇÑå äÑã ÇÝÒÇÑ
				DialogBoxParam(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_ABOUT),hDlg,(DLGPROC)AboutWindowDlgProc,NULL);
				return TRUE;
		}
		switch(LOWORD(wParam))//ÈÑÇí ÓÇíÑ Ï˜ãå åÇ
		{
		case IDC_BROWSE:
			lstrcpy(szDialogTitle,TEXT("áØÝÇ ÝÇíá æÑæÏí ÎæÏ ÑÇ ÇäÊÎÇÈ äãÇííÏ"));
			if(PopFileOpenDlg(hDlg,szFileName,szFileTitle,szDialogTitle,TEXT ("Executable Files (*.EXE)\0*.exe\0DLL Files (*.DLL)\0*.dll\0System Driver Files (*.SYS)\0*.sys\0\0"),"exe"))
			{
				SetDlgItemText(hDlg,IDC_INPUT,szFileName);
				szFileName[0]=NULL;
				//vaghti file avaz shod,textbox haye adres ham pak mishavad,ta moshkeli baraye karbar pish nayayad:
				SetDlgItemText(hDlg,IDC_ADDRESS,"");
				SetDlgItemText(hDlg,IDC_ADDRESSRVA,"");
			}
			return TRUE;
		case IDC_SAVEOUTPUT:
			lstrcpy(szDialogTitle,TEXT("áØÝÇ ÝÇíáí ÌåÊ ÐÎíÑå ÎÑæÌí ÇäÊÎÇÈ äãÇííÏ"));
			if(PopFileSaveDlg(hDlg,szFileName,szFileTitle,szDialogTitle,TEXT ("Text Files (*.txt)\0*.txt\0\0"),"txt"))
			{
				if(!WriteToFile(GetDlgItem(hDlg,IDC_DISASSEMBLED),szFileName))
					MessageBox(hDlg,TEXT("ÎØÇíí ÏÑ åäÇã äæÔÊä ÏÑ ÝÇíá ÎÑæÌí ÑÎ ÏÇÏå ÇÓÊ"),TEXT("ÎØÇ ÏÑ äÑã ÇÝÒÇÑ"),MB_ICONWARNING);
			}
			return TRUE;
		case IDC_DOIT:
			GetDlgItemText(hDlg,IDC_INPUT,strBuffer,MAX_PATH);
			if(PathFileExists(strBuffer)==TRUE)
			{
				filename_exe = strBuffer;
				GetFileInformation(hDlg,strBuffer);//baraye daryafte ettelaate file az header.
				GetPrivateProfileString("Features","ImageBase","4194304",strBuffer_2,256,"disassemblerconfig.ini");
				imagebase=atoi(strBuffer_2);
				GetDlgItemText(hDlg,IDC_ARCHITECTURE,strBuffer_2,256);
				if(strcmp(strBuffer_2,"IMAGE_FILE_MACHINE_AMD64")!=0 && strcmp(strBuffer_2,"IMAGE_FILE_MACHINE_I386")!=0)
				{
					MessageBox(hDlg,"˜ÇÑÈÑ ÑÇãí ÝÇíá ÔãÇ í˜ ÝÇíá ÇÌÑÇíí äíÓÊ. íÇ Çíä˜å ãÚãÇÑí Çíä ÝÇíá ÊæÓØ äÑã ÇÝÒÇÑ ÔÊíÈÇäí äãí ÔæÏ\nÌåÊ ÇØáÇÚÇÊ ÈíÔÊÑ¡ ÏÇÏå åÇí ÓãÊ  äÌÑå ÑÇ ÈÑÑÓí äãÇííÏ","ÎØÇ",MB_ICONWARNING);
					return TRUE;
				}

				GetDlgItemText(hDlg,IDC_ADDRESS,strBuffer_2,MAX_PATH);//daryafte offsete voroodi karbar.
				unsigned int address=0;
				if(lstrcmp(strBuffer_2,TEXT(""))==0)
				{
					GetDlgItemText(hDlg,IDC_ADDRESSRVA,strBuffer_2,MAX_PATH);
					if(lstrcmp(strBuffer_2,TEXT(""))!=0)
					{
						if(strBuffer_2[strlen(strBuffer_2)-1]=='h' || strBuffer_2[strlen(strBuffer_2)-1]=='H')//agar enteha harfe h dashte bashim,yani voroodi dar mabnaye 16 ast.
							sscanf(strBuffer_2,"%x",&address);
						else
							sscanf(strBuffer_2,"%d",&address);
						address=ConvertRVAToOffset(address-imagebase);
					}
					else
						address=ConvertRVAToOffset(fileinfo_imgntheader.OptionalHeader.AddressOfEntryPoint);
					SetDlgItemInt(hDlg,IDC_ADDRESS,address,false);
				}
				else
				{
					if(strBuffer_2[strlen(strBuffer_2)-1]=='h' || strBuffer_2[strlen(strBuffer_2)-1]=='H')//agar enteha harfe h dashte bashim,yani voroodi dar mabnaye 16 ast.
						sscanf(strBuffer_2,"%x",&address);
					else
						sscanf(strBuffer_2,"%d",&address);

				}
				if(address==0)
				{
					MessageBox(hDlg,TEXT("ÂÏÑÓ æÇÑÏ ÔÏå ãÚÊÈÑ äíÓÊ"),TEXT("ÎØÇ ÏÑ äÑã ÇÝÒÇÑ"),MB_ICONWARNING);
					return TRUE;
				}
				Disassembler(hDlg,address,strBuffer,strBuffer_2);
				string alldis;
				alldis = strBuffer_2;
				if(draw_cfg==true)
				{
					MakeCFG(strBuffer_2);
					/*vector<string> s = cfg.get_next_address();
					while (s.size()>0)
					{
						cfg.clear();
						vector<int> adds = get_next_add_inits(s);
						s.clear();
						for(int i = 0 ; i<adds.size();i++){
							if(adds.at(i)>0){
								Disassembler(hDlg,adds.at(0),strBuffer,strBuffer_2);
								alldis.insert(alldis.length(), strBuffer_2);
							}
						}
						if(adds.size()>0){
							MakeCFG(alldis.c_str());
							s = cfg.get_next_address();
						}
					}*/
					cfg.make_pdf();
					cfg.clear();
				}
				SetDlgItemText(hDlg,IDC_DISASSEMBLED,strBuffer_2);
			}
			else
			{
				MessageBox(hDlg,TEXT("ÝÇíá æÑæÏí íÇÝÊ äÔÏ"),TEXT("ÎØÇ ÏÑ äÑã ÇÝÒÇÑ"),MB_ICONWARNING);
			}
			return TRUE;
		}
		return TRUE;
	case WM_CLOSE:
		GetDlgItemText(hDlg,IDC_INPUT,strBuffer,256);
		WritePrivateProfileString("Saved","FILE",strBuffer,"disassemblerconfig.ini");
		GetDlgItemText(hDlg,IDC_ADDRESS,strBuffer,256);
		WritePrivateProfileString("Saved","ADDRESS",strBuffer,"disassemblerconfig.ini");
		GetDlgItemText(hDlg,IDC_ADDRESSRVA,strBuffer,256);
		WritePrivateProfileString("Saved","ADDRESSRVA",strBuffer,"disassemblerconfig.ini");
		EndDialog(hDlg,0);
		return TRUE;
	}
	return FALSE;
}

LRESULT CALLBACK AboutWindowDlgProc(HWND hDlg,UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_CLOSE:
			EndDialog(hDlg,0);
			return TRUE;
	}
	return FALSE;
}
LRESULT CALLBACK LocationWindowDlgProc(HWND hDlg,UINT message, WPARAM wParam, LPARAM lParam)
{
	static TCHAR szFileName[MAX_PATH],szFileTitle[MAX_PATH],szDialogTitle[MAX_PATH];//save ÈÑÇí ÐÎíÑå äÇã æ ÚäæÇä ˜ÇÏÑåÇí ãÍÇæÑå Çí
	BROWSEINFO bi = { 0 };
    LPITEMIDLIST pidl;


	switch(message)
	{
		case WM_CLOSE:
			EndDialog(hDlg,0);
			return TRUE;
		case WM_INITDIALOG:
			GetPrivateProfileString("Path","CFG_PATH","",szFileName,MAX_PATH,"disassemblerconfig.ini");
			if(strcmp(szFileName,"")!=0)
				SetDlgItemText(hDlg,IDC_SAVECFG,szFileName);
			GetPrivateProfileString("Path","CFGPDF_PATH","",szFileName,MAX_PATH,"disassemblerconfig.ini");
			if(strcmp(szFileName,"")!=0)
				SetDlgItemText(hDlg,IDC_PDFFOLDER,szFileName);
			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case IDC_BROWSE:
				if(PopFileSaveDlg(hDlg,szFileName,szFileTitle,szDialogTitle,TEXT ("Text Files (*.txt)\0*.txt\0\0"),"txt"))
				{
					SetDlgItemText(hDlg,IDC_SAVECFG,szFileName);
					szFileName[0]=NULL;
				}
				return TRUE;
			case IDC_BROWSE2:
				bi.lpszTitle = ("áØÝÇ í˜ æÔå ÇäÊÎÇÈ ˜äíÏ");
				pidl = SHBrowseForFolder ( &bi );
				SHGetPathFromIDList ( pidl, szFileName );
				if(PathFileExists(szFileName))
					SetDlgItemText(hDlg,IDC_PDFFOLDER,szFileName);
				return TRUE;
			case IDOK:
				GetDlgItemText(hDlg,IDC_SAVECFG,szFileName,MAX_PATH);
				if(strcmp(szFileName,"")==0)//agar karbar matni dar textbox vared nakarde bashad, angah pak ma ham dar file ettelaat khorooji ra pak mikonim.
					WritePrivateProfileString("Path","CFG_PATH","","disassemblerconfig.ini");
				else
					WritePrivateProfileString("Path","CFG_PATH",szFileName,"disassemblerconfig.ini");
				GetDlgItemText(hDlg,IDC_PDFFOLDER,szFileName,MAX_PATH);
				if(PathFileExists(szFileName))
					WritePrivateProfileString("Path","CFGPDF_PATH",szFileName,"disassemblerconfig.ini");
				else if(strcmp(szFileName,"")==0)
					WritePrivateProfileString("Path","CFGPDF_PATH","","disassemblerconfig.ini");
				SendMessage(hDlg,WM_CLOSE,NULL,NULL);
				return TRUE;
			}

			return TRUE;
	}
	return FALSE;
}

//baraye daryafte mahale nasbe barname.
void GetExeDirectory(PTCHAR return_value)
{
	string myDirectory="";//jai ke file exe dar anja gharar darad.
	string saveDirectory="";//mahali ke file pdf anja zakhire mishavad.
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
	strcpy(return_value,myDirectory.c_str());
}

LRESULT CALLBACK DisassemblerConfigWindowDlgProc(HWND hDlg,UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR strBuffer[256],strBuffer_2[256];//baraye zakhireye reshtehaye movaghati darune in window procedure:
	int temp;
	BOOL temp_bool;

	switch(message)
	{
	case WM_INITDIALOG:
		GetExeDirectory(strBuffer);
		strcat(strBuffer,"Dlls\\");
		if(PathFileExists(strBuffer))
		{
			strcat(strBuffer,"*.dll");
			SendMessage(GetDlgItem(hDlg,IDC_AVAILABLEPLUGINS),LB_DIR,DDL_READWRITE,(LPARAM)strBuffer);
		}
		GetPrivateProfileString("Features","ImageBase","4194304",strBuffer,256,"disassemblerconfig.ini");
		SetDlgItemText(hDlg,IDC_IMAGEBASE,strBuffer);
		//baraye daryafte pluginhayee ke bayad estefade shavand,va namayeshe an dar listbox:
		GetPrivateProfileString("Plugins","Count","0",strBuffer,256,"disassemblerconfig.ini");
		temp=atoi(strBuffer);
		for(int i=0;i<temp;i++)///be azaye har plugin yek bar ejra mishavad.
		{
			strcpy(strBuffer,"Num");
			itoa(i,strBuffer_2,10);
			strcat(strBuffer,strBuffer_2);
			GetPrivateProfileString("Plugins",strBuffer,"",strBuffer_2,256,"disassemblerconfig.ini");
			SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_ADDSTRING,0,(LPARAM)strBuffer_2);
		}
		return TRUE;
	case WM_CLOSE:
		EndDialog(hDlg,0);
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			temp=SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_GETCOUNT,0,0);//tedad pluginhaye mored estefade.
			itoa(temp,strBuffer,10);
			WritePrivateProfileString("Plugins","Count",strBuffer,"disassemblerconfig.ini");
			for(int i=0;i<temp;i++)
			{
				strcpy(strBuffer,"Num");
				itoa(i,strBuffer_2,10);
				strcat(strBuffer,strBuffer_2);
				SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_GETTEXT,i,(LPARAM)strBuffer_2);
				WritePrivateProfileString("Plugins",strBuffer,strBuffer_2,"disassemblerconfig.ini");
			}
			temp=GetDlgItemInt(hDlg,IDC_IMAGEBASE,&temp_bool,false);
			if(temp_bool==true)
			{
				itoa(temp,strBuffer,10);
				WritePrivateProfileString("Features","ImageBase",strBuffer,"disassemblerconfig.ini");
				SendMessage(hDlg,WM_CLOSE,NULL,NULL);
			}
			else
			{
				GetDlgItemText(hDlg,IDC_IMAGEBASE,strBuffer,256);
				if(strcmp(strBuffer,"")==0)
				{
					WritePrivateProfileString("Features","ImageBase","4194304","disassemblerconfig.ini");
					SendMessage(hDlg,WM_CLOSE,NULL,NULL);
				}
				else
					MessageBox(hDlg,"äÇãÚÊÈÑ ÇÓÊ ImageBase ãÞÏÇÑ","ÇÎØÇÑ",MB_ICONWARNING);
			}
			return TRUE;
		case IDC_ADD:
			temp=SendMessage(GetDlgItem(hDlg,IDC_AVAILABLEPLUGINS),LB_GETCURSEL,0,0);
			if(temp!=LB_ERR)
			{
				SendMessage(GetDlgItem(hDlg,IDC_AVAILABLEPLUGINS),LB_GETTEXT,temp,(LPARAM)strBuffer);
				if(SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_FINDSTRINGEXACT,-1,(LPARAM)strBuffer)==LB_ERR)//agar ghablan dar list pluginhaye estefade shode mujud nabashad.
					SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_ADDSTRING,0,(LPARAM)strBuffer);
			}
			else
				MessageBox(hDlg,"áÇíäí ÇäÊÎÇÈ äÔÏå ÇÓÊ","ÇÎØÇÑ",MB_ICONWARNING);
			return TRUE;
		case IDC_REMOVE:
			temp=SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_GETCURSEL,0,0);
			if(temp!=LB_ERR)
				SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_DELETESTRING,temp,0);
			else
				MessageBox(hDlg,"áÇíäí ÇäÊÎÇÈ äÔÏå ÇÓÊ","ÇÎØÇÑ",MB_ICONWARNING);
			return TRUE;
		case IDC_UP:
			temp=SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_GETCURSEL,0,0);
			if(temp!=LB_ERR)
			{
				if(temp==0)//agar avalin onsor entekhab shode bashad.
					return TRUE;
				else
				{
					SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_GETTEXT,temp,(LPARAM)strBuffer);
					SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_GETTEXT,temp-1,(LPARAM)strBuffer_2);
					SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_DELETESTRING,temp-1,0);
					SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_DELETESTRING,temp-1,0);
					SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_INSERTSTRING,temp-1,(LPARAM)strBuffer);
					SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_INSERTSTRING,temp,(LPARAM)strBuffer_2);
					SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_SETCURSEL,temp-1,0);
				}
			}
			else
				MessageBox(hDlg,"áÇíäí ÇäÊÎÇÈ äÔÏå ÇÓÊ","ÇÎØÇÑ",MB_ICONWARNING);
			return TRUE;
		case IDC_DOWN:
			temp=SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_GETCURSEL,0,0);
			if(temp!=LB_ERR)
			{
				if(temp==(SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_GETCOUNT,0,0)-1))//agar akharin onsor entekhab shode bashad.
					return TRUE;
				else
				{
					SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_GETTEXT,temp,(LPARAM)strBuffer);
					SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_GETTEXT,temp+1,(LPARAM)strBuffer_2);
					SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_DELETESTRING,temp,0);
					SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_DELETESTRING,temp,0);
					SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_INSERTSTRING,temp,(LPARAM)strBuffer_2);
					SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_INSERTSTRING,temp+1,(LPARAM)strBuffer);
					SendMessage(GetDlgItem(hDlg,IDC_USEDPLUGINS),LB_SETCURSEL,temp+1,0);
				}
			}
			else
				MessageBox(hDlg,"áÇíäí ÇäÊÎÇÈ äÔÏå ÇÓÊ","ÇÎØÇÑ",MB_ICONWARNING);
			return TRUE;
		}
	}
	return FALSE;
}