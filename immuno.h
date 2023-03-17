/* HEADER FILE FOR IMMUNO */
/* The code below is written by: Love Mitteregger & Peter Byström */
#define OLED_MAX_BUFFER		512

/* ========= COPIED FUNCTIONS ========== */ 
/* THE FIRST 5 FUNCTIONS MENTIONED BELOW ARE COPIED WORK FROM KTH LAB 
 * FILES, CREATED BY AXEL ISAKSSON & F LUNDEVALL, THESE FUNCTIONS ARE 
 * ONLY USED FOR THE GAME CREDITS & HIGHSCORE DISPLAY SHOWN AT GAME OVER.
 */
extern char textbuffer[4][16];
extern const uint8_t const font[128*8];
void display_string(int line, char *s);
void display_image(int x, const uint8_t *data);
void display_update(void);
/* END OF COPY */


/* =================== ASSEMBLY FUNCTIONS ==================== */
void delay(int);
void enable_interrupt(void);

/* ============ CHIPKIT HARDWARE CONFIG FUNCTIONS ============ */
int chipkit_boot(void);

/* ============= CHIPKIT CLOCK CONFIG FUNCTIONS ============== */
void clock_boot(void);

/* ========== CHIPKIT OLED DISPLAY CONFIG FUNCTIONS =========== */
uint8_t spi2_comms(uint8_t bytedata);
void oled_boot(void);
void oled_releasebuffer(int cb, uint8_t* bytedata);
void oled_refresh(uint8_t* buffer);
void oled_updtpos(uint8_t* buffer);
void oled_clear(uint8_t* buffer);
void oled_flash(void);

/* ============= CHIPKIT IO CONFIG FUNCTIONS ================= */
void io_boot(void);
int read_switch(void);
int read_button(void);

/* ===================== BUFFERS ===================== */

// Main Assets
extern uint8_t clear[OLED_MAX_BUFFER];
extern uint8_t worldbuffer[OLED_MAX_BUFFER];
extern uint8_t top_objectbuffer[OLED_MAX_BUFFER];
extern uint8_t bottom_objectbuffer[OLED_MAX_BUFFER];
extern uint8_t  virus_buffer[OLED_MAX_BUFFER];
extern uint8_t title[OLED_MAX_BUFFER];
extern uint8_t controls1[OLED_MAX_BUFFER];
extern uint8_t controls2[OLED_MAX_BUFFER];
extern uint8_t menu[OLED_MAX_BUFFER];
extern uint8_t gameover[OLED_MAX_BUFFER];

// World Assets
extern uint8_t top_gameframe[OLED_MAX_BUFFER];
extern uint8_t bottom_gameframe[OLED_MAX_BUFFER];
extern uint8_t virus[];
extern uint8_t top_fat_1[];
extern uint8_t top_valve_1[];
extern uint8_t bottom_fat_1[];
extern uint8_t bottom_valve_1[];

// Player Assets
extern uint8_t ufo[OLED_MAX_BUFFER];
extern uint8_t lazerbuffer[OLED_MAX_BUFFER];

// Scroll Assets
extern uint8_t top_scroll_1[];
extern uint8_t bottom_scroll_1[];

/* ===================== MAIN FUNCTIONS ===================== */
void sys_boot(void);	// Boot up the display and the game
void game_title(void);	// Display the game title for a few seconds
void game_menu(void);	// Display the game menu and activate its functions
void game_controls(void); // Briefly show action buttons
void game_start(void); 	// Start game
void render(void);		// Renders the level, players and enemies
void scroll_level(void);
void read_io(void);
void add_virus(int);
void lazer(void);
void int_to_str(int num, char* str);

/* ========================= LEVELS ========================= */
const uint8_t load_level(int i); // load any of the lvl bitmaps
uint8_t load_ufo();				// load the player




