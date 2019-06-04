/***********************************\
		  Roborun Console
		   4 Guys 1 Robot
\***********************************/

//		Libraries
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#define KEY_Y 121

//		Init Variables
HANDLE hSerial;
char comPortNumber[1] = "1";
char serialPort[9] = "\\\\.\\COM";


//		Functions
void clrscr() {
	system("cls");					// Since C is not aware of the command prompt having a screen, we use this Windows API feature to execute a cmd command that clears the screen.
}

void anyKey() {
	printf("Druk op de any toets om door te gaan.\n");
	_getch();
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
	hSerial = CreateFile(serialPort,	// Port name
		GENERIC_READ | GENERIC_WRITE,	// Read/Write
		0,								// No Sharing
		NULL,							// No Security
		OPEN_EXISTING,					// Open existing port only
		FILE_ATTRIBUTE_NORMAL,          // Non Overlapped I/O
		NULL);							// Null for Comm Devices
	//		Setup Com Port
	if (hSerial == INVALID_HANDLE_VALUE) {
		printf("Er is een fout opgetreden bij het openen van de COM Poort.\n");

		if (!SetCommTimeouts(hSerial, &timeout)) {
			//		Connection Lost
			printf("Verbinding met de robot is verloren.\n");
		}

		if (GetLastError() == ERROR_FILE_NOT_FOUND) {
			//		COM Port not found
			printf("De ingevoerde COM Poort bestaat niet.\n");
		}

	}
	else {
		printf("COM Poort geopend, druk op de any toets om door te gaan.\n");
		menu();
	}

	//		Set COM Port settings in DCB Struct.
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (!GetCommState(hSerial, &dcbSerialParams)) {
		//		Error getting state
	}
	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	if (!SetCommState(hSerial, &dcbSerialParams)) {
		//		Error setting serial port state
	}
}

void serialWrite(char txchar) {
	BOOL bWriteRC;
	static DWORD iBytesWritten;
	bWriteRC = WriteFile(hSerial, &txchar, 1, &iBytesWritten, NULL);	// Writes to COM Port
}

char serialRead(unsigned int bytes) {
	char rxchar[4];
	BOOL bReadRC;
	static DWORD iBytesRead;
	bReadRC = ReadFile(hSerial, &rxchar, 4 , &iBytesRead, NULL);		// Reads from COM Port
	return rxchar;
}

void getComPort() {
	printf("Voer het nummer van de COM Poort in.\n");
	comPortNumber[0] = _getch();
	strcat_s(serialPort, 9, comPortNumber);
	printf("%s\n", serialPort);
}

void readBatteryStats() {
	clrscr();
	char msg[4];
	char batteryLevel;
	serialWrite('b');
	for (int i = 0; i < 4; i++) {
		msg[i] = serialRead(1);
	}
	batteryLevel = atoi(msg) - 3600;
	batteryLevel = batteryLevel / 19;
	printf("De batterij is nog %c%c vol\n", batteryLevel, '%');
	anyKey();

}

void coordinateInput() {
	char x[10], y[10];
	unsigned int coordCount = 0;
	int sendCoord = 0;
	char key;
	clrscr();
	printf("Voer hier de coordina(a)t(en) in waar de robot zich naar toe moet bevinden.");
	do {
		for (int i = 0; i < 10;) {
			printf("\nX=");
			x[i] = _getch();			// Let user input X coord.
			putchar(x[i]);
			printf("\nY=");
			y[i] = _getch();			// Let user input Y coord.
			coordCount++;
			putchar(y[i]);
			printf("\nJe hebt de volgende coordinaat ingegeven:\n");
			printf("Coord %i: (%c,%c)\n", coordCount, x[i], y[i]);
			printf("Druk op E om nog een coordinaat in te voeren, druk op Y om de coordinaten te versturen, of druk op Q om de ingevoerde coordinaten te verwijderen.\n");
			key = _getch();				// Check for confirmation.
			switch (key) {}
			if (key == KEY_E) {
				i++;
			}
			else if (key == KEY_Y) {
				sendCoord++;
				break;
			}
			else if (key == KEY_Q) {
				break;
				coordinateInput();
			}
		}
	} while (!sendCoord);
	
	if (sendCoord) {
		serialWrite('c');				// Activates coordinate input on the robot.
		for (int i = 0; i < coordCount; i++) {
			serialWrite(x[i]);
			serialWrite(y[i]);
		}
		serialWrite('y');				// Confirms sent coordinates.
	}
	anyKey();
}

void manualOperation() {
	clrscr();
	serialWrite('h');									// Initiates manual mode on the robot.
	printf("Gebruik WASD of de numpad pijltjestoetsen om de robot te besturen.\nDruk op E voor een noodstop.\nDruk op Q om terug te gaan naar het hoofdmenu.\n");
	char key;
	char msg[20];
	do {
		key = _getch();
		switch (key) {
		case KEY_W: serialWrite('w'); break;			// Makes the robot go forward when W is pressed.
		case KEY_S: serialWrite('f'); break;			// Makes the robot go backwards when S is pressed.
		case KEY_D: serialWrite('d'); break;			// Makes the robot turn clockwise when D is pressed.
		case KEY_A: serialWrite('a'); break;			// Makes the robot turn counter-clockwise when A is pressed.
		case KEY_E: serialWrite('s');
		}
		if (key == 0xE0 || key == 0) {
			key = 256 + _getch();						// Key has two codes - read the second one.
			switch (key) {
			case KEY_UP: serialWrite('w'); break;		// Makes the robot go forward when Arrow_up is pressed.
			case KEY_DOWN: serialWrite('f'); break;		// Makes the robot go backwards when Arrow_down is pressed.
			case KEY_LEFT: serialWrite('d'); break;		// Makes the robot turn counter-clockwise when Arrow_left is pressed.
			case KEY_RIGHT: serialWrite('a'); break;	// Makes the robot turn clockwise when Arrow_right is pressed.
			}
			continue;
		}
		else {
			
		}
	} while (key != KEY_Q);
	serialWrite('q');
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
			case KEY_1: readBatteryStats(); done = 0; break;		// Displays Battery Stats when 1 is pressed.
			case KEY_2: coordinateInput(); done = 0; break;			// Let the user input a coordinate when 2 is pressed.
			case KEY_3: manualOperation(); done = 0; break;			// Let the user move the robot manually when 3 is pressed.
			case ESC: done = 0; break;								// Exits console when ESC is pressed.
			case KEY_S: serialWrite('s');							// Let the Robot stop immediately when S is pressed.
			default: printf("Geen geldige keus, probeer opnieuw.\n"); break;
			}
		} while (done);
	}
}

int main() {
	getComPort();				// Asks user for the COM Port number
	serialInit();				// Initialize Serial Communication

	CloseHandle(hSerial);		// Closing the Serial Port
	return 0;
}
