#include <Windows.h>

static OPENFILENAME ofn ;

//Baraye neveshtane matne ye edit box dar file khorooji be kar miravad.
BOOL WriteToFile (HWND hwndEdit, PTSTR pstrFileName)
{
     DWORD  dwBytesWritten ;
     HANDLE hFile ;
     int    iLength ;
     PTSTR  pstrBuffer ;
     WORD   wByteOrderMark = 0xFEFF ;
          // Open the file, creating it if necessary
     
     if (INVALID_HANDLE_VALUE == 
               (hFile = CreateFile (pstrFileName, GENERIC_WRITE, 0, 
                                    NULL, CREATE_ALWAYS, 0, NULL)))
          return FALSE ;

          // Get the number of characters in the edit control and allocate
          // memory for them.
     
     iLength = GetWindowTextLength (hwndEdit) ;
     pstrBuffer = (PTSTR) malloc ((iLength + 1) * sizeof (TCHAR)) ;
     
     if (!pstrBuffer)
     {
          CloseHandle (hFile) ;
          return FALSE ;
     }

          // If the edit control will return Unicode text, write the
          // byte order mark to the file.

#ifdef UNICODE
     WriteFile (hFile, &wByteOrderMark, 2, &dwBytesWritten, NULL) ;
#endif

          // Get the edit buffer and write that out to the file.
     
     GetWindowText (hwndEdit, pstrBuffer, iLength + 1) ;
     WriteFile (hFile, pstrBuffer, iLength * sizeof (TCHAR), 
                &dwBytesWritten, NULL) ;
     
     if ((iLength * sizeof (TCHAR)) != (int) dwBytesWritten)
     {
          CloseHandle (hFile) ;
          free (pstrBuffer) ;
          return FALSE ;
     }
     
     CloseHandle (hFile) ;
     free (pstrBuffer) ;
     
     return TRUE ;
}


void PopFileInitialize (HWND hwnd)
{
     ofn.lStructSize       = sizeof (OPENFILENAME) ;
     ofn.hwndOwner         = hwnd ;
     ofn.hInstance         = NULL ;
     ofn.lpstrCustomFilter = NULL ;
     ofn.nMaxCustFilter    = 0 ;
     ofn.nFilterIndex      = 0 ;
     ofn.lpstrFile         = NULL ;          // Set in Open and Close functions
     ofn.nMaxFile          = MAX_PATH ;
     ofn.lpstrFileTitle    = NULL ;          // Set in Open and Close functions
     ofn.nMaxFileTitle     = MAX_PATH ;
     ofn.lpstrInitialDir   = NULL ;
     ofn.Flags             = 0 ;             // Set in Open and Close functions
     ofn.nFileOffset       = 0 ;
     ofn.nFileExtension    = 0 ;
     
     ofn.lCustData         = 0L ;
     ofn.lpfnHook          = NULL ;
     ofn.lpTemplateName    = NULL ;
}

BOOL PopFileOpenDlg (HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName,PTSTR dialogTitle,LPCSTR lpstrFilter,LPCSTR lpstrDefExt)
{
     ofn.hwndOwner         = hwnd ;
     ofn.lpstrFile         = pstrFileName ;
     ofn.lpstrFileTitle    = pstrTitleName ;
	 ofn.lpstrDefExt=lpstrDefExt;
	 ofn.lpstrFilter=lpstrFilter;
	 ofn.lpstrTitle=dialogTitle;
     ofn.Flags             = OFN_HIDEREADONLY | OFN_CREATEPROMPT ;
     
     return GetOpenFileName (&ofn) ;
}

BOOL PopFileSaveDlg (HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName,PTSTR dialogTitle,LPCSTR lpstrFilter,LPCSTR lpstrDefExt)
{
     ofn.hwndOwner         = hwnd ;
     ofn.lpstrFile         = pstrFileName ;
     ofn.lpstrFileTitle    = pstrTitleName ;
	 	 ofn.lpstrDefExt=lpstrDefExt;
	 ofn.lpstrFilter=lpstrFilter;
	 ofn.lpstrTitle=dialogTitle;
     ofn.Flags             = OFN_OVERWRITEPROMPT ;
     
     return GetSaveFileName (&ofn) ;
}