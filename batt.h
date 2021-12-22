/*************************************************************************
 * batt.h     Part of the SEABATTLE game by Vince Weaver                 *
 ************************************************************************/

#include <curses.h>     /* Slang support is good for rxvt in linux */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>       /* Needed to randomize from time */
#include <string.h>

#define C_BLUE   1      /* Define values for the colors */
#define C_GREEN  2      /* I co-incidently used the IBM PC values */
#define C_CYAN   3      /* For the colors, because I know them already */
#define C_RED    4
#define C_PURPLE 5
#define C_YELLOW 6
#define C_WHITE  7
#define C_BOLD   1
#define C_NORMAL 0

#define SOUND_ON 1
#define SOUND_OFF 0
#define SOUND_SPEAKER 0
#define SOUND_8BIT 1

#define INBOUNDSX(x,y) ( (x>=0) && (x<=7) && (users_grid[x][y]!=1) && (users_grid[x][y]!=2) )
#define INBOUNDSY(y,x) ( (y>=0) && (y<=7) && (users_grid[x][y]!=1) && (users_grid[x][y]!=2) )


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
void draw_opening(void);
void clear_grid(int grid[8][8]);
int get_users_grid(int grid[8][8]);
void get_computers_grid(int grid[8][8]);
void display_screen(int users_grid[8][8],int computers_grid[8][8]);
void place_grid(int grid[8][8],int x,int y,int show_ships);
int test_overlap(int grid[8][8],int x,int y,int xdir,int ydir,int size);
void put_ship(int grid[8][8],int x,int y,int xdir,int ydir,int size);
int place_ship(int grid[8][8],int size);
void quit(void);
void clear_side(void);
void put_dumb_instructions(void);
void do_sound(int which_one);
void clear_status_lines(void);
void purge_grid(int grid[8][8],int type);
void simple_message(int message);
/* bgam.c */
DATA *enter_info(MAIN_THINGY *main_thing);
void do_arrow(int whos_turn,int num_turns);
void print_sound_status();
void draw_the_screen(DATA *person,int num_turns);
void print_message(int color,int person,char *message);
void play_the_game(DATA *person);
void do_options();
void do_help();
int main_menu(DATA *person,MAIN_THINGY *main_thing);
/* bdb.c */
void out_of_memory_panic();
MAIN_THINGY *make_main_thingy(void);
char *read_string_from_disk(FILE *fff,char *string);
int my_string_comp(const char *cs, const char *ct);
DATA *insert_new_data(MAIN_THINGY *main_thing,DATA *datum);
void read_data_from_disk(MAIN_THINGY *main_thing);
void write_data_to_disk(MAIN_THINGY *main_thing);
void do_high_score(char *name,int turn);
/* THE END *******************  batt.h *************  by Vince Weaver */
