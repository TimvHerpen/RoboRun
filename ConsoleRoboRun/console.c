// ConsoleRoboRun.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

//		Key definitions by ascii number
#define ESC 27
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_1 49
#define KEY_2 50
#define KEY_3 51
#define KEY_Q 81

void clrscr() {
	system("cls");
}

void serialWrite() {

}

void serialRead() {

}

void getInput() {

}

void readBatteryStats() {
	clrscr();
}

void coordinateInput() {
	char coord[3];
	clrscr();
	printf("Voer hier een coordinaat in waar de robot zich naar toe moet bevinden:\n");
	fgets(coord, 3, stdin);
	printf("Je hebt de volgende coordinaat ingegeven:\n");
	printf(coord);
	
	
}

void manualOperation() {
	clrscr();
	char key;
	char msg[20];
	do {
		key = _getch();
		if (key == 0 || key == -32) {
			key = _getch(); //key code has two keys - read the second one
			switch (key) {
			case KEY_UP: memcpy(msg, "UP", sizeof(msg)); break;
			case KEY_DOWN: memcpy(msg, "DOWN", sizeof(msg)); break;
			case KEY_LEFT: memcpy(msg, "LEFT", sizeof(msg)); break;
			case KEY_RIGHT: memcpy(msg, "RIGHT", sizeof(msg)); break;
			default:  memcpy(msg, "unknown key", sizeof(msg)); break;
			}
			printf("Key: %s", msg);
			putchar('\n');
			continue;
		}
		else {
			printf("Key: %c", key);
			putchar('\n');
		}
	} while (key != KEY_Q);
}

void menu() {
	char key;
	clrscr();
	printf("Maak een keuze uit de volgende opties:\n1) Batterij status\n2) Coordinaat invoeren\n3) Handmatige Besturing\nJe kan terugkeren naar dit menu met ESC.\n");
	do {
		key = _getch();
	
			switch (key) {
			case KEY_1: readBatteryStats(); break;
			case KEY_2: coordinateInput(); break;
			case KEY_3: manualOperation(); break;
			
		}
	} while (key != ESC);
}

int main() {
	//		Init variables
	HANDLE hSerial;
	DCB dcbSerialParams = { 0 };
	COMMTIMEOUTS timeout = { 0 };
	
	//		Timeout values
	timeout.ReadIntervalTimeout = 50;
	timeout.ReadTotalTimeoutConstant = 50;
	timeout.ReadTotalTimeoutMultiplier = 10;
	timeout.WriteTotalTimeoutConstant = 50;
	timeout.WriteTotalTimeoutMultiplier = 10;

	//		Handle
	hSerial = CreateFile("\\\\.\\COM8",	// Port name
		GENERIC_READ | GENERIC_WRITE,	// Read/Write
		0,								// No Sharing
		NULL,							// No Security
		OPEN_EXISTING,					// Open existing port only
		FILE_ATTRIBUTE_NORMAL,          // Non Overlapped I/O
		NULL);							// Null for Comm Devices
	//		Setup Com Port
	if (hSerial == INVALID_HANDLE_VALUE) {
		printf("Error opening serial port");

		if (!SetCommTimeouts(hSerial, &timeout)){
			//An error occurred
		}

		if (GetLastError() == ERROR_FILE_NOT_FOUND) {
			printf("serial port does not exist.");
		}

	}
	else {
		printf("Opening serial port successful\n");
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

	CloseHandle(hSerial);							//Closing the Serial Port
	return 0;
}