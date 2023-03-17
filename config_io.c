/* CONFIG FILE FOR CHIPKIT BASIC I/O SHIELD I/O PORTS */
/* The code below is written by: Love Mitteregger & Peter Byström */
#include <stdint.h>
#include <pic32mx.h>
#include "immuno.h"

/* Bootstraping the IO ports */
void io_boot( void )
{
	/* Registers for TRI-State functions ('TRISx')
	 * The 'TRISx'-registers configure the data direction flow through I/O-pins. 
	 * Determines whether a PORTx I/O-pin is input or output!
	 * If Input -> TRISx = '1'. If Output -> TRISx = '0'
	 */

	TRISEINV = 0x0;		// Atomic TRISx set, sets LEDs to output
	PORTE = 0xFF;		// Activate All LEDs because its cool
	TRISDSET = 0xFE0;	// Switches 1-4 (0xF00) & Buttons 2-4 (0x0E0)
	TRISFSET = 0x002;	// Button 1 
}

/* Switch/Button is Low (0) if not activated, High (1) if activated.
 * PORTD represents the switches & buttons on the ChipKIT I/O-shield.
 * Initialize portd as a global volatile variable for the functions to use.
 */
int read_switch(void)
{
	volatile int* portd = (volatile int*) 0xbf8860D0;

	/* The four least significant bits of the return value
	 * should contain data from switches SW4, SW3, SW2, SW.
	 *
	 * To rephrase: Must set the data recieved from the switches
	 * SW4, SW3, SW2 and SW1 as the lsb of the return value.
	 * Data from switches are present in the the 3rd Byte of PORTD, 
	 * so the switch data bits must be shifted by 2 bytes (8 bits) 
	 * to the right.
	 */
	int switches = *portd >> 8;

	// Mask the data retrieved from the switches, using AND-operand
	// So that in any bits that are high (1) in
	switches &= 0x0f;
}

int read_button(void)
{
	/* The three least significant bits of the return value must
	 * contain current data from push buttons BTN4, BTN3 and BTN2.
	 * BTN2 corresponds to the least significant bit. All other bits
	 * of the return value must be zero.
	 *
	 * Saving one bit space for button 1, which will come from PORTF.
	 * Button1 will then merge with buttons and then be returned.
	 */
	
	int buttons = PORTD >> 4;
	// Mask the 4 lsb (1110 = E) for button 2 to 4.
	buttons &= 0xE;
	int button1 = PORTF >> 1;
	button1 &= 0x1;

	// NOTE: can also write int buttons = (*portd >> 4) & 0xE;

	// Merge Button1 with rest of the buttons
	buttons |= button1;
	// return masked button data
	return buttons;
}

