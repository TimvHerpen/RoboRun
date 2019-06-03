#pragma once
void menu();
void manualOperation();
void main();
void readBatteryStats(); 
void coordinateInput();
void clrscr();
void serialWrite();
char serialRead();
handle serialInit();



typedef struct _DCB {
	DWORD DCBlength;
	DWORD BaudRate;
	DWORD fBinary : 1;
	DWORD fParity : 1;
	DWORD fOutxCtsFlow : 1;
	DWORD fOutxDsrFlow : 1;
	DWORD fDtrControl : 2;
	DWORD fDsrSensitivity : 1;
	DWORD fTXContinueOnXoff : 1;
	DWORD fOutX : 1;
	DWORD fInX : 1;
	DWORD fErrorChar : 1;
	DWORD fNull : 1;
	DWORD fRtsControl : 2;
	DWORD fAbortOnError : 1;
	DWORD fDummy2 : 17;
	WORD  wReserved;
	WORD  XonLim;
	WORD  XoffLim;
	BYTE  ByteSize;
	BYTE  Parity;
	BYTE  StopBits;
	char  XonChar;
	char  XoffChar;
	char  ErrorChar;
	char  EofChar;
	char  EvtChar;
	WORD  wReserved1;
} DCB, * LPDCB;