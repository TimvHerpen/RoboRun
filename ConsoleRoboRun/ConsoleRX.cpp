#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "rs232.h"


int main()
{
	int i, n,
		cport_nr = 0,        // COM port
		bdrate = 9600;       // 9600 baud

	unsigned char buf[4096];

	char mode[] = { '8','N','1',0 };


	if (RS232_OpenComport(cport_nr, bdrate, mode))
	{
		printf("Can not open comport\n");

		return(0);
	}

	while (1)
	{
		n = RS232_PollComport(cport_nr, buf, 4095);

		if (n > 0)
		{
			buf[n] = 0;   // always end string with a "null"

			for (i = 0; i < n; i++)
			{
				if (buf[i] < 32)  // replace unreadable control-codes by dots
				{
					buf[i] = '.';
				}
			}

			printf("received %i bytes: %s\n", n, (char*)buf);
		}


		Sleep(100);
	}

	return(0);
}

