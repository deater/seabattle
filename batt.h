/*************************************************************************
 * batt.h     Part of the SEABATTLE game by Vince Weaver                 *
 ************************************************************************/

/* global vars */
extern int sound_on;
extern int sound_device;

#define C_BLUE   1      /* Define values for the colors */
#define C_GREEN  2      /* I co-incidently used the IBM PC values */
#define C_CYAN   3      /* For the colors, because I know them already */
#define C_RED    4
#define C_PURPLE 5
#define C_YELLOW 6
#define C_WHITE  7
#define C_BOLD   1
#define C_NORMAL 0

#define C_BGBLUE 9
#define C_BGCYAN 11
#define C_BGWHITE 15
#define C_WHITECYAN 16

#define SOUND_ON 1
#define SOUND_OFF 0

#define SOUND_ERROR	0
#define SOUND_HIT	1
#define SOUND_MISS	2
#define SOUND_SUNKIT	3

#define SOUND_DEVICE_SPEAKER	0
#define SOUND_DEVICE_DEV_AUDIO	1
#define SOUND_DEVICE_APLAY	2

typedef struct  {
	char *first_name;
	char *last_name;
	char *addy1;
	char *addy2;
	char *phonenum;
	int times_played;
	int best_score;
} DATA;

typedef struct element{
	struct element *prev;
	struct element *next;
	DATA *data;
} ELEMENT;

typedef struct {
	ELEMENT *head;
	ELEMENT *tail;
	int num_elements;
} MAIN_THINGY;

extern DATA *current_player;

/* All the function declarations */

/* binp.c */
void play_sound(char *soundfile);
void seabattle_init_curses(void);
void seabattle_exit_curses(void);
void set_color(int color,int bold);
void printxy(int x,int y,char *tempst);
void draw_title(void);
void draw_opening(void);
int get_users_grid(int grid[8][8]);
void get_computers_grid(int grid[8][8]);
void place_grid(int grid[8][8],int x,int y,int show_ships);
int test_overlap(int grid[8][8],int x,int y,int xdir,int ydir,int size);
void put_ship(int grid[8][8],int x,int y,int xdir,int ydir,int size);
int place_ship(int grid[8][8],int size);
void quit(void);
void clear_side(void);
void do_sound(int which_one);
void clear_status_lines(void);

/* bgam.c */
DATA *enter_info(MAIN_THINGY *main_thing);
int main_menu(DATA *person,MAIN_THINGY *main_thing);

/* bdb.c */
void out_of_memory_panic(void);
MAIN_THINGY *make_main_thingy(void);
void write_data_to_disk(MAIN_THINGY *main_thing);
void do_high_score(char *name,int turn);

/* THE END *******************  batt.h *************  by Vince Weaver */
