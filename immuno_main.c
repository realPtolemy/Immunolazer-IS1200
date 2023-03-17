/* MAIN FILE FOR IMMUNOLAZER - CONTAINS GAME LOGIC FUNCTIONS */ 
/* The code below is written by: Love Mitteregger & Peter Byström */

/* ========= COPIED FUNCTIONS CALLS ========== */ 
/* THE FUNCTIONS CALLS BELOW ARE COPIED WORK FROM KTH LAB 
 * FILES, CREATED BY AXEL ISAKSSON & F LUNDEVALL, THESE FUNCTION CALLS ARE 
 * ONLY USED FOR THE GAME CREDITS & HIGHSCORE DISPLAY SHOWN AT GAME OVER.
 * ORIGINATES FROM THE 'mipslamain.c' file.
 * display_string(0, "IMMUNOLAZER");
 * display_string(1, "BY LOVE & PETER");
 * display_string(2, "YOUR SCORE:");
 * display_string(3, str);
 * display_update();
 */



#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <pic32mx.h>
#include "immuno.h"

/* OLED display specific information */
#define OLED_MAX_PAGE		4
#define OLED_MAX_BUFFER		512
#define OLED_MAX_COLUMN		128
#define OLED_MAX_ROW		32


/* ============== GLOBAL VARIABLES ============== */ 
int currentLevel;
int gameOver;
int virus_lane;
int score =0;
uint8_t level;
uint8_t ufo_page = 1;
uint8_t ufo_column = 0;

/* ================ BUFFER ARRAYS ================ */
uint8_t worldbuffer[OLED_MAX_BUFFER];
uint8_t top_objectbuffer[OLED_MAX_BUFFER];
uint8_t bottom_objectbuffer[OLED_MAX_BUFFER];
uint8_t virus_buffer[OLED_MAX_BUFFER];


/* ========== SYSTEM INITIALIZATION AND GAME RUNTIME ========= */ 
int main(void) {

	chipkit_boot();
	sys_boot();

	return 0;
}

void sys_boot(void){	
	// Timer initialization
	clock_boot();

	// OLED display initialization
	oled_boot();
	
	// Input/Output initialization
	io_boot();
	
	// Display stuff on the screen when booting
	game_title();
	oled_flash();
	delay(3000);
	oled_clear(worldbuffer);
	delay(500);
	game_controls();
	delay(4000);
	game_menu();
}

void game_title(void) {
	oled_refresh(title);
}

void game_menu(void) {
	game_start();
}

void game_controls(void) {
	oled_refresh(controls1);
	//delay(xxx);
	//switch
	//oled_refresh(controls2);
}

void game_over(void){
    
    char str[20];

    int_to_str(score, str);
  
	oled_refresh(gameover);
	oled_flash();
	delay(3000);
	oled_refresh(clear);
	delay(10);

	/* COPIED FUNCTION CALLS */
	display_string(0, "IMMUNOLAZER");
	display_string(1, "BY LOVE & PETER");
	display_string(2, "YOUR SCORE:");
	display_string(3, str);
	display_update();
	/* END OF COPY */

}

void game_start(void) {

	currentLevel = 0;
	while(1) {

		if(gameOver) {
			game_over();
			return;
		}

		read_io();
		render();
		delay(50);
		scroll_level();
	}
	return;

}

void render(void) {

	static int framedelay = 0;
	static int random = 0;
	int i;
	int j = 0;

	// Render random viruses
	if (!(random % 50)){
		add_virus(2);
	}

	// Render MORE random viruses
	if (!((random +10) % 34)){
		add_virus(1);;
	}

	random++;

	// Use clock flag for frame refresh instead of general delay
	if (IFS(0) & 0x100) {
		framedelay++;
		IFSCLR(0) = 0x100;
	}

	/* Clear the buffer before rendering new values */
	oled_clear(worldbuffer);

	// Render mainframe and ufo
	for (i = 0; i < OLED_MAX_BUFFER; i++) {
		//worldbuffer[i] = gameframe[i] | ufo[i];
			worldbuffer[i] = ufo[i] | lazerbuffer[i];
	}

	// If object collission, then game over
	for (i = 0; i < 128; i++) {
		if (ufo[i] & top_scroll_1[i]) {
			gameOver = 1;
		} else if (ufo[i+384] & bottom_objectbuffer[i])
			gameOver = 1;
		}
	
	// Merge TOP World Objects
	for (i = 0; i < OLED_MAX_BUFFER; i++) {
		// worldbuffer[i] = worldbuffer[i] +top_scroll_1[i];
		 top_objectbuffer[i] = top_scroll_1[i] | top_gameframe[i];

		if(ufo[i] & virus_buffer[i]){
			gameOver=1;	
		}
		if(lazerbuffer[i+63] & virus_buffer[63+i]){
			score = score + 10;
			for(j=0;j<13;j++){
				virus_buffer[i+j+62] = 0;
			}

		}
	}

	// Render TOP World Objects
	for (i = 0; i < 128; i++) {
		// worldbuffer[i] = worldbuffer[i] +top_scroll_1[i];
		worldbuffer[i] = worldbuffer[i] + top_objectbuffer[i];
	}

	// Merge BOTTOM World Objects
		for (i = 0; i < OLED_MAX_BUFFER; i++) {
		// worldbuffer[i] = worldbuffer[i] +top_scroll_1[i];
		 bottom_objectbuffer[i] = bottom_scroll_1[i] + bottom_gameframe[i];
	}

	//Render BOTTOM World Objects
	j = 0;
	for (i = 512-128; i < 512; i++) {
		worldbuffer[i] = worldbuffer[i] | bottom_objectbuffer[j];
		j++;
	}
	
	for (i =0; i< 512; i++){
		worldbuffer[i] |= virus_buffer[i];
	}

	// Render frame refresh on completed delay
	if(framedelay == 3) {
		oled_refresh(worldbuffer);
		framedelay = 0;
	}
}

void scroll_level(void){
	// Code for scrolling the level
	//static int virusdelay;
	int i;
	score = score +1;
	// Shift TOP & BOTTOM objects one bit at the time
	for(i = 0; i < OLED_MAX_BUFFER - 1; i++) {
		top_scroll_1[i-1] = top_scroll_1[i];
		top_scroll_1[i] = 0;
		bottom_scroll_1[i-1] = bottom_scroll_1[i];
		bottom_scroll_1[i] = 0;
	}
	
	// Shift viruses
	for(i = 0; i < 512 - 8; i++) {
		virus_buffer[i-1] = virus_buffer[i];
	}
	// Set the element values of previous position to 0, for all 8 columns
	for(i = 0; i < 8; i++) {
	virus_buffer[512-i+i] = 0;
				}
}


void read_io(void) {

	static int button_pressed;
	static int switch_on; 

	int buttons = read_button();
	int switches = read_switch();
	int i, j;

	// if any button is pressed...
	if (buttons != 0) {

		// if BTN4 is pressed
		if (buttons & 0x08 && ufo_column > 0) {
			ufo_column--;
	
			// Shift the elements to the right by 8 posiitons
			for(i = 0; i < 512 - 8; i++) {
				ufo[i-8] = ufo[i];
			}
			// Set the element values of previous position to 0, for all 8 columns
			for(i = 0; i < 8; i++) {
				ufo[512-8+i] = 0;
			}
			delay(150);
			render();
		}
		
		// if BTN3 is pressed
		if (buttons & 0x04 && ufo_page > 0) {
			ufo_page--;
			// Move up one page, by shifting elements in the array 128 indexvalues to "left"
   		
			// Shift the elements to the left by 128 positions
    		for(i = 0; i < OLED_MAX_BUFFER - OLED_MAX_COLUMN; i++) {
        		ufo[i] = ufo[i+OLED_MAX_COLUMN];
    		}
    
			// Set the element values of previous position to 0, for all 128 columns
    		for(i = 0; i < OLED_MAX_COLUMN; i++) {
        		ufo[OLED_MAX_BUFFER-OLED_MAX_COLUMN+i] = 0;
    		}
    		delay(150);
			switch_on = 1;
    		render();
		}

		// if BTN2 is pressed
		if (buttons & 0x02 && ufo_page < OLED_MAX_PAGE-1) {
			ufo_page++;
			// Move down one page, by shifting elements in the array 128 indexvalues to "right"

			// Shift the elements to the right by 8 posiitons
			for(i = OLED_MAX_BUFFER-1; i >= OLED_MAX_COLUMN; i--) {
				ufo[i] = ufo[i-OLED_MAX_COLUMN];
			}

			// Set the element values of previous position to 0, for all 128 columns
			for(i = 0; i < OLED_MAX_COLUMN; i++) {
				ufo[i] = 0;
			}
			delay(150); // TEMPORARY DELAY, SHOULD SYNC WITH CLOCK LATER
			render();
		}

		// if BTN1 is pressed and ufo is within display (128px/9px=14 - 1 render)
		if (buttons & 0x01 && ufo_column < 14) {
			ufo_column++;
	
			// Shift the elements to the right by 8 posiitons
			for(i = 512-1; i >= 8; i--) {
				ufo[i] = ufo[i-8];
			}
			// Set the element values of previous position to 0, for all 8 columns
			for(i = 0; i < 8; i++) {
				ufo[i] = 0;
			}
			delay(150); // TEMPORARY DELAY, SHOULD SYNC WITH CLOCK LATER
			render();
		}
	}
	
	// If switches are on, activate lazer because its cool and kills viruses!
	if (switches != 0)
	{
		lazer();
	} else {
		oled_clear(lazerbuffer);
	}	
}


// Adds viruses randomly to the array
void add_virus(int number) {
	virus_lane = number;
	int i;
	for(i=0; i<7;i++){
		virus_buffer[121 +virus_lane*128 +i] |= virus[i];
	}
}

// Generates a cool lazerbeam infront of the UFO
void lazer(void) {
	int i,j;
	oled_clear(lazerbuffer);
	for (i =0; i <OLED_MAX_BUFFER; i++){
		if(ufo[i] !=0){
			for(j = 0; j<32; j++){
				lazerbuffer[i+8 +j] = 0x10;	
			}
		}
	}
}


/* DEMO FEATURES */
/* INT TO STRING CONVERTER */

void int_to_str(int num, char* str) {
    int i = 0, is_negative = 0;

    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    do {
        str[i++] = num % 10 + '0';
        num /= 10;
    } while (num > 0);

    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // Reverse the string
    int len = i;
    for (i = 0; i < len / 2; i++) {
        char tmp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = tmp;
    }
}

