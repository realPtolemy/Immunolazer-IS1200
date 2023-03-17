/* CONFIG FILE FOR CHIPKIT BASIC I/O SHIELD OLED DISPLAY */
/* The code below is written by: Love Mitteregger & Peter Byström with exception
 * of the copied functions mentioned below... */

/* ========= COPIED FUNCTIONS ========== */ 
/* THE FUNCTIONS MENTIONED BELOW ARE COPIED WORK FROM KTH LAB 
 * FILES, CREATED BY AXEL ISAKSSON & F LUNDEVALL, THESE FUNCTIONS ARE 
 * ONLY USED FOR THE GAME CREDITS & HIGHSCORE DISPLAY SHOWN AT GAME OVER.
 * ORIGINATES FROM THE 'mipslafunc.c' file.
 * void display_string(int line, char *s);
 * void display_image(int x, const uint8_t *data);
 * void display_update(void);
 */

/* Note that the other code below is inspired by the display configuration file
 * 'mipslabfunc.c' provided by Axel Isaksson, F Lundevall and 
 * KTH. The file is primarily based on the Digilent 'chipKIT™' Basic I/O Shield™
 * Reference Manual and SOLOMON SYSTECH OLED Common Driver information 'SSD1306'.
 */

#include <stdint.h>
#include <pic32mx.h>
#include "immuno.h"



/* Standard Peripheral Input - NOTE: SPI2 */
#define SPI_TRANSMIT_BUFFER	(SPI2STAT & 0x08)
#define SPI_RECEIVE_BUFFER	(SPI2STAT & 0x01)

/* Activate/Deactivate powersupply to OLED display */
#define	OLED_VBAT_ON		(PORTFCLR = 0x20)
#define OLED_VBAT_OFF		(PORTFSET = 0x20)
#define	OLED_VDD_ON			(PORTFCLR = 0x40)
#define OLED_VDD_OFF		(PORTFSET = 0x40)

/* Set OLED display to read commands or to read data */
#define OLED_READ_COMMANDS	(PORTFCLR = 0x10)
#define OLED_READ_DATA		(PORTFSET = 0x10)

/* OLED display Reset */
#define OLED_RESET_ON		(PORTGCLR = 0x200)
#define OLED_RESET_OFF		(PORTGSET = 0x200)

/* OLED display Properties */
#define OLED_MAX_PAGE		4
#define OLED_MAX_BUFFER		512
#define OLED_MAX_COLUMN		128
#define OLED_MAX_ROW		32

/* The 'spi2_comms' function lets the Basic I/O shield communicate with PIC32 board.
   NOTE: uint8_t tells us that we are dealing with unsigned integers of 8 bits (1 byte). 
   IN SHORT: Write/Read a byte on SPI port 2. */
uint8_t spi2_comms(uint8_t bytedata) { 		
	/* Wait for transmitter to be ready */
	while(SPI_TRANSMIT_BUFFER == 0);
	/* Write the next transmit byte */
	SPI2BUF = bytedata;
	/* Wait for receiver to be ready */
	while(SPI_RECEIVE_BUFFER == 0);
	/* Put the byte in the buffer */
	return SPI2BUF;
}

/* Bootstraping the OLED display */
void oled_boot(void) {	
	/* Set OLED to read commands by clearing the read data bit */
	OLED_READ_COMMANDS;

	/* Turn on OLED VDD and wait for power to come up */
	OLED_VDD_ON;
	delay(1000);
	
	/* Turn off OLED by sending 0xAE command to SPI, see SPI function above*/
	spi2_comms(0xAE);
	
	/* Reset OLED peripherals */
	OLED_RESET_ON;
	delay(10);
	OLED_RESET_OFF;
	delay(10);
	
	/* Turn on 'Charge Pump' by sending 0x8D and 0x14 command to SPI */
	spi2_comms(0x8D);
	spi2_comms(0x14);
	
	/* Turn on 'Pre-Charge Period' by sending 0xD9 and 0x14 command to SPI */
	spi2_comms(0xD9);
	spi2_comms(0xF1);
	
	/* Turn on OLED VBAT (VCC) and wait */
	OLED_VBAT_ON;
	delay(1000);

	/* Invert the OLED-display, puts origin in upper left corner */
	spi2_comms(0xA1);	// Send 0xA1 to remap columns
	spi2_comms(0xC8);	// Send 0xC8 to remap the rows
	
	/* Turn on 'sequential COM configruation' by sending
	 * 0xDA and 0x20 command to SPI. This will make the
	 * OLED-display memory non-interleaved. I.e. does not
	 * break the memory up into chunks. */
	spi2_comms(0xDA);	// Send 0xDA to Activate 'COM configuration'
	spi2_comms(0x20);	// Send 0x20 to set config to 'sequential COM'
						// left/right remap enabled

	/* Turn on OLED by sending 0xAF command to SPI */
	spi2_comms(0xAF);
}


/* The below function writes data to the display panel using the SPI controller */
void oled_releasebuffer(int cb, uint8_t *bytedata) {
	int ib;
	uint8_t bTmp;
	
	for (ib = 0; ib < cb; ib++){
		/* Wait for transmitter to be ready */
		while(SPI_TRANSMIT_BUFFER == 0);
		/* Write the next transmit byte */
		SPI2BUF = *bytedata++;
	/* Wait for receiver to be ready */
	while(SPI_RECEIVE_BUFFER == 0);
	bTmp = SPI2BUF;
	}
}


/* THIS FUNCTION IS COPIED FROM 'mipslabfunc.c' */
void display_string(int line, char *s) {
	int i;
	if(line < 0 || line >= 4)
		return;
	if(!s)
		return;
	
	for(i = 0; i < 16; i++)
		if(*s) {
			textbuffer[line][i] = *s;
			s++;
		} else
			textbuffer[line][i] = ' ';
}

/* THIS FUNCTION IS COPIED FROM 'mipslabfunc.c'
 * AND MODIFIED to work properly with OLED peripherals calls */
void display_image(int x, const uint8_t *data) {
	int i, j;
	
	for(i = 0; i < 4; i++) {
		OLED_READ_COMMANDS;

		spi2_comms(0x22);
		spi2_comms(2);
		
		spi2_comms(x & 0xF);
		spi2_comms(0x10 | ((x >> 4) & 0xF));
		
		OLED_READ_DATA;
		
		for(j = 0; j < 32; j++)
			spi2_comms(~data[i*32 + j]);
	}
}

/* THIS FUNCTION IS COPIED FROM 'mipslabfunc.c'
 * AND MODIFIED to work properly with OLED peripherals calls */
void display_update(void) {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) {
		OLED_READ_COMMANDS;
		spi2_comms(0x22);
		spi2_comms(i);
		
		spi2_comms(0x0);
		spi2_comms(0x10);
		
		OLED_READ_DATA;
		
		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;
			
			for(k = 0; k < 8; k++)
				spi2_comms(font[c*8 + k]);
		}
	}
}
/* END OF COPY */


/* Update the OLED display with contents from memory buffer */
void oled_refresh(uint8_t* buffer) {
	int page;

	for (page = 0; page < OLED_MAX_PAGE; page++) {

		/* Set display to read commands */
		OLED_READ_COMMANDS;
		
		/* Set to update page */
		spi2_comms(0x22);
		/* Set which page should be updated. NOTE: page++ itteration */
		spi2_comms(page);

		/* Update from left column */
		spi2_comms(0x00); // Set lowest nibble of column
		spi2_comms(0x10); // Set highest nibble of column

		/* Set display to read data */
		OLED_READ_DATA;

		/* Send the buffer sent as an argument for refresh to
		   the buffer to display forwarding funciton
		   copy everything from each column of the current page itteration */
		oled_releasebuffer(OLED_MAX_COLUMN, buffer);
		buffer += OLED_MAX_COLUMN;
	}
}

/* Update the OLED display with an empty 128x32 array */
void oled_clear(uint8_t* buffer) {
	int i;
	for (i = 0; i < OLED_MAX_BUFFER; i++)
		buffer[i] = 0;
}

void oled_flash(void) {
	OLED_READ_COMMANDS;
	delay(300);
	spi2_comms(0xA7);
	delay(300);
	spi2_comms(0xA6);
	delay(300);
	spi2_comms(0xA7);
	delay(300);
	spi2_comms(0xA6);
}


