
include 'win32wx.inc' ; you can simply switch between win32ax, win32wx, win64ax and win64wx here

.code

  start:
	push eax;
	mov eax,5;
 _loop:
	test eax,100 ;
	jge endloop ;
	add eax,1  ;
	jmp _loop;
  endloop:
  exit:
	pop eax;

.end start