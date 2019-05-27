// ConsoleRoboRun.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include "rs232.h"
#include "ConsoleRX.cpp"

#define ESC 27
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_1 49
#define KEY_2 50
#define KEY_3 51

enum MenuOption { None = 0, BatteryStats = 1, CoordinateInput = 2, ManualOperation = 3 };





int manualOperation() {
	char key;
	char msg[20];
	do {
		key = _getch();
		if (key == 0 || key == -32) {
			key = _getch(); //key code has two keys - read the second one
			switch (key) {
			case F1: memcpy(msg, "F1", sizeof(msg)); break;
			case F2: memcpy(msg, "F2", sizeof(msg)); break;
			case F3: memcpy(msg, "F3", sizeof(msg)); break;
			case F4: memcpy(msg, "F4", sizeof(msg)); break;
			case F5: memcpy(msg, "F5", sizeof(msg)); break;
			case F6: memcpy(msg, "F6", sizeof(msg)); break;
			case F7: memcpy(msg, "F7", sizeof(msg)); break;
			case F8: memcpy(msg, "F8", sizeof(msg)); break;
			case F9: memcpy(msg, "F9", sizeof(msg)); break;
			case F10: memcpy(msg, "F10", sizeof(msg)); break;
			case PAGE_UP: memcpy(msg, "PAGE UP", sizeof(msg)); break;
			case PAGE_DOWN: memcpy(msg, "PAGE DOWN", sizeof(msg)); break;
			case HOME: memcpy(msg, "HOME", sizeof(msg)); break;
			case END: memcpy(msg, "END", sizeof(msg)); break;
			case UP: memcpy(msg, "UP", sizeof(msg)); break;
			case DOWN: memcpy(msg, "DOWN", sizeof(msg)); break;
			case LEFT: memcpy(msg, "LEFT", sizeof(msg)); break;
			case RIGHT: memcpy(msg, "RIGHT", sizeof(msg)); break;
			default:  memcpy(msg, "unknown key", sizeof(msg)); break;
			}
			printf("Key: %s", msg);
			putchar('\n');
			continue;
		}
		if (key == ESC) {
			printf("Key: ESCAPE");
			putchar('\n');
		}
		else {
			printf("Key: %c", key);
			putchar('\n');
		}
	} while (key != ESC);
}


/*int main()
{
    std::cout << "Hello World!\n"; 
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
*/
int main() {
	HANDLE hSerial;

	hSerial = CreateFile("\\\\.\\COM1",	// Port name
		GENERIC_READ | GENERIC_WRITE,	// Read/Write
		0,								// No Sharing
		NULL,							// No Security
		OPEN_EXISTING,					// Open existing port only
		FILE_ATTRIBUTE_NORMAL,          // Non Overlapped I/O
		NULL);							// Null for Comm Devices

	if (hSerial == INVALID_HANDLE_VALUE) {
		printf("Error opening serial port");
		if (GetLastError() == ERROR_FILE_NOT_FOUND) {
			printf("serial port does not exist.");
		}

	}
	else {
		printf("opening serial port successful");
	}
	CloseHandle(hSerial);							//Closing the Serial Port

	DCB dcbSerialParams = { 0 };
	dcbSerial.DCBlength = sizeof(dcbSerialParams);
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




	return 0;
}