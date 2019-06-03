// ConsoleRoboRun.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include "console.h"

//		Key definitions by ascii number
#define ESC 27
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_1 49
#define KEY_2 50
#define KEY_3 51
#define KEY_Q 113
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_E 101

HANDLE hSerial;
char comPortNumber = "2";
char serialPort = "\\\\.\\COM";

void clrscr() {
	system("cls");
}

void serialInit() {
	//		Init variables
	extern HANDLE hSerial;
	DCB dcbSerialParams = { 0 };
	COMMTIMEOUTS timeout = { 0 };

	//		Timeout values
	timeout.ReadIntervalTimeout = 50;
	timeout.ReadTotalTimeoutConstant = 50;
	timeout.ReadTotalTimeoutMultiplier = 10;
	timeout.WriteTotalTimeoutConstant = 50;
	timeout.WriteTotalTimeoutMultiplier = 10;

	//		Handle
	hSerial = CreateFile("\\\\.\\COM2",	// Port name
		GENERIC_READ | GENERIC_WRITE,	// Read/Write
		0,								// No Sharing
		NULL,							// No Security
		OPEN_EXISTING,					// Open existing port only
		FILE_ATTRIBUTE_NORMAL,          // Non Overlapped I/O
		NULL);							// Null for Comm Devices
	//		Setup Com Port
	if (hSerial == INVALID_HANDLE_VALUE) {
		printf("Error opening serial port");

		if (!SetCommTimeouts(hSerial, &timeout)) {
			//An error occurred
		}

		if (GetLastError() == ERROR_FILE_NOT_FOUND) {
			printf("Serial port does not exist.");
		}

	}
	else {
		printf("COM Poort geopend, druk op de any toets om door te gaan.\n");
		menu();
	}


	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (!GetCommState(hSerial, &dcbSerialParams)) {
		//error getting state
	}
	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	if (!SetCommState(hSerial, &dcbSerialParams)) {
		//error setting serial port state
	}
	//return hSerial;
}

void serialWrite(char txchar) {
	BOOL bWriteRC;
	static DWORD iBytesWritten;
	bWriteRC = WriteFile(hSerial, &txchar, 1, &iBytesWritten, NULL);
}

char serialRead(unsigned int bytes) {
	char rxchar[4];
	BOOL bReadRC;
	static DWORD iBytesRead;
	bReadRC = ReadFile(hSerial, &rxchar, 4 , &iBytesRead, NULL);
	return rxchar;
}

void getComPort() {
	strcat(serialPort, comPortNumber);
}

void readBatteryStats() {
	clrscr();
	char batteryLevel;

	serialWrite('b');
	batteryLevel = serialRead(4);
	batteryLevel = batteryLevel - 4000;
	batteryLevel = batteryLevel / 10;
	printf("De batterij is nog %c %c vol", batteryLevel, '%');

}

void coordinateInput() {
	char coord[10][3];
	clrscr();
	printf("Voer hier een coordinaat in waar de robot zich naar toe moet bevinden:\n");
	fgets(coord, 3, stdin);
	printf("Je hebt de volgende coordinaat ingegeven:\n");
	printf("%s\n",coord);
	
	
}

void manualOperation() {
	clrscr();
	serialWrite('h');
	printf("Gebruik WASD of de numpad pijltjestoetsen. Druk op Q om terug te gaan naar het hoofdmenu.\n");
	char key;
	char msg[20];
	do {
		key = _getch();
		switch (key) {
		case KEY_W: serialWrite('w'); break;
		case KEY_S: serialWrite('f'); break;
		case KEY_D: serialWrite('d'); break;
		case KEY_A: serialWrite('a'); break;
			//case KEY_Q: {serialWrite('q'); active != active; break; }
		}
		if (key == 0xE0 || key == 0) {
			key = 256 + _getch(); //key code has two keys - read the second one
			switch (key) {
			case KEY_UP: serialWrite('w'); break;
			case KEY_DOWN: serialWrite('f'); break;
			case KEY_LEFT: serialWrite('d'); break;
			case KEY_RIGHT: serialWrite('a'); break;
			}
			continue;
		}
		else {
			
		}
	} while (key != KEY_Q);
}

void menu() {
	char key;
	int done = 1;
	key = _getch();
	while (key != ESC) {
		done = 1;
		clrscr();
		printf("Maak een keuze uit de volgende opties:\n1) Batterij status\n2) Coordinaat invoeren\n3) Handmatige Besturing\nJe kan terugkeren naar dit menu met ESC.\n");
		do {
			key = _getch();

			switch (key) {
			case KEY_1: readBatteryStats(); done = 0; break;
			case KEY_2: coordinateInput(); done = 0; break;
			case KEY_3: manualOperation(); done = 0; break;
			case ESC: done = 0; break;
			case KEY_S: serialWrite('s);
			default: printf("Geen geldige keus, probeer opnieuw.\n"); break;
			}
		} while (done);
	}
}

int main() {
	serialInit();

	CloseHandle(hSerial);							//Closing the Serial Port
	return 0;
}
