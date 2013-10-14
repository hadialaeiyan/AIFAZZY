include 'win32wx.inc' ; you can simply switch between win32ax, win32wx, win64ax and win64wx here

.code

  start:
	push eax;
	push ebx;
	mov eax,5;
	mov ebx,10;
 _loop:
	test eax,100 ;
	jge endloop ;
	add eax,1;
	sub ebx,1;
	test ebx,0
	jle endloop;
	jmp _loop;
  endloop:
  exit:
	pop ebx;
	pop eax;


.end start