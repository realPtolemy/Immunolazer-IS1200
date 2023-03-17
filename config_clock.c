/* CONFIG FILE FOR CHIPKIT CLOCK */
/* The code below is written by: Love Mitteregger & Peter Byström */
#include <stdint.h>
#include <pic32mx.h>
#include "immuno.h"

/* Bootstraping the hardware clock */
void clock_boot(void) { 

	// T2CON: Set prescaler value to 256
	T2CON = 0x70;

	// TMR2: Reset Timer
	TMR2 = 0x0;

	// PR2: Period Counter - sets the value that TMR2 counts to before 0-reset & interrupt
	// 31250 = (80 Mhz / 256 prescaler)/10 s
	PR2 = 31250;
	
	// IPC(2) - Interrupt Priority Control, controls priority lvl of interrupt sources
	// IPCSET(2) = 0x1f000000; // Switch Priority
	// IPCSET(2) = 0x10;		// Timer Priority
	IPCSET(2) = 0x1f;

	// IFSCLR(0) - Clear Interrupt Flag Status
	IFSCLR(0) = 0x100;

	// IECSET(0) - Interrupt Enable Control Set specific interrupt
	IECSET(0) = 0x100;

	// BONUS!!
	//volatile int* switches_portd = (volatile int*) 0xbf8860D0;
	//int switch2 = *switches_portd >> 8;
	
	//if (switch2 = 0x2) {
	//		INTCONSET = 0x4;
	//}

	//enable_interrupt();

	// T2CONSET - Activate timer
	T2CONSET = 0x8000;

}
