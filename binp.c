/*************************************************************************
 * binp.c --- The First Module of Vince Weaver's SEA BATTLE program      *
 *            For ENEE114.   Finished 15 April 1997                      *
 ************************************************************************/

#include <stdio.h>
#include <ncurses.h>     /* Slang support is good for rxvt in linux */

#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#include "batt.h"                          /* Include prototypes */

/* INIT CURSES -- inits curses */
/* library */

void seabattle_init_curses(void) {

	initscr();
	start_color();

	/* Allocate the colors we'll use */
	init_pair(7,COLOR_WHITE,COLOR_BLACK);
	init_pair(1,COLOR_BLUE,COLOR_BLACK);
	init_pair(2,COLOR_GREEN,COLOR_BLACK);
	init_pair(3,COLOR_CYAN,COLOR_BLACK);
	init_pair(4,COLOR_RED,COLOR_BLACK);
	init_pair(6,COLOR_YELLOW,COLOR_BLACK);

	/* Start key by key mode */
	cbreak();

	/* Don't echo */
	noecho();

	/* Um I forget, view the manpage */
	nonl();
#ifndef USE_SLANG
	intrflush(stdscr,FALSE);	/* Slang doesn't like this */
#endif
	/* Enable arrow keys */
	keypad(stdscr,TRUE);
}


/* Function to set colors */
void set_color(int color,int bold) {
	if (bold) attrset(COLOR_PAIR(color)|A_BOLD);
	else attrset(COLOR_PAIR(color)|A_NORMAL);
}


/* Simple function that saves */
/* repetitive move() printw() */
/* sequences */
void printxy(int x,int y,char *tempst) {
	move(y,x);
	printw(tempst);
}


/* Shutdown curses nicely so the */
/* terminal works properly */
void seabattle_exit_curses(void) {
	clear();
	refresh();
	nocbreak();
	echo();
	endwin();
}


/* Draw opening screen */
void draw_opening(void) {

	int i;
	char ch;
	FILE *ftemp;

	set_color(C_BLUE,C_BOLD);
	printxy(0,0,".|'''|                    '||'''|,            ||      ||    '||`");
	printxy(0,1,"||                         ||   ||            ||      ||     ||");
	printxy(0,2,"`|'''|, .|''|,  '''|.      ||;;;;    '''|.  ''||''  ''||''   ||  .|''|,");
	printxy(0,3," .   || ||..|| .|''||      ||   ||  .|''||    ||      ||     ||  ||..||");
	printxy(0,4," |...|' `|...  `|..||.    .||...|'  `|..||.   `|..'   `|..' .||. `|...");
	set_color(C_WHITE,C_NORMAL);
	printxy(0,6,"                                )_");
	printxy(0,7,"                           _____|_|_____+__");
	printxy(0,8,"                           |   x    x  x   |");
	printxy(0,9,"           \\----|------------------------------------------/");
	printxy(0,10,"	    \\                                             /");
	move(11,0);
	set_color(C_CYAN,C_NORMAL);
	for(i=0;i<39;i++) printw("/\\");
	set_color(C_RED,C_BOLD);
	printxy(10,13,"A Game By Vince Weaver - Who cannot draw ASCII ships");
	set_color(C_YELLOW,C_BOLD);
	printxy(11,14,"vince@deater.net   http://www.deater.net/weave");
	set_color(C_GREEN,C_BOLD);
	printxy(18,15,"Another Amazing Project for ENEE114");
	set_color(C_WHITE,C_BOLD);
	if (!has_colors()) {
		printxy(0,17,"COLOR NOT AVAILABLE ON THIS TERMINAL. Using Black and White.");
	}
	refresh();
	set_color(C_WHITE,C_NORMAL);
	move(19,0);

	/***********************/
	/* detect sound setup */
	/*********************/

	/* first see if /usr/bin/aplay exists */
	if (!access("/usr/bin/aplay",X_OK)) {
		sound_device=SOUND_DEVICE_APLAY;
	}
	/* Sees if /dev/audio is available */
	else if (!access("/dev/audio",W_OK)) {
		sound_device=SOUND_DEVICE_DEV_AUDIO;
	}
	else {
		sound_device=SOUND_DEVICE_SPEAKER;
	}

	if (sound_device==SOUND_DEVICE_SPEAKER) {
		printw("No digital sound available.  "
			"Press any key to continue.");
		ch=mvgetch(19,55);
	}
	else {
		printw("Do you want nifty digital sound effects (y/n) ?");
		ch=mvgetch(19,49);
		if ( (ch!='y') && (ch!='Y') ) {
			sound_device=SOUND_DEVICE_SPEAKER;
		}
	}
}


/* This made code slightly less */
/* Cluttered */
static void put_dumb_instructions(void) {
	set_color(C_GREEN,C_BOLD);
	printxy(5,7,"Choose a ship:");
	printxy(5,8,"1. Submarine");
	printxy(5,9,"2. Battleship");
	printxy(5,10,"3. Aircraft Carrier");
	printxy(5,11,"Q. Continues with game");
	printxy(5,12,"X. Quits game entirely");
	printxy(5,13,"--->");
}


/* Clears an 8x8 grid */
static void clear_grid(int grid[8][8]) {

	int i,j;

	for(i=0;i<8;i++) {
		for(j=0;j<8;j++) {
			grid[i][j]=0;
		}
	}
}

/* Play appropriate sound */
/* through appropriate device */
/* 0=error 1=hit 2=miss 3=sunkit */
/* Incomplete as of yet */

void play_sound(char *soundfile) {

	char command[256];

	if (sound_device==SOUND_DEVICE_APLAY) {
		snprintf(command,256,"/usr/bin/aplay -q %s/%s",
			DATADIR,soundfile);
		system(command);
	}
	else  if (sound_device==SOUND_DEVICE_DEV_AUDIO) {
		snprintf(command,256,"cat %s/%s > /dev/audio",
			DATADIR,soundfile);
		system(command);
	}
	else {
		beep();
	}
}

void do_sound(int which_one) {

	if (!sound_on) return;

	switch(which_one) {
		case SOUND_ERROR:
			beep();
			break;
		case SOUND_HIT:
			if (rand()%2) play_sound("hit1.au");
			else play_sound("hit2.au");
			break;
		case SOUND_MISS:
			if (rand()%2) play_sound("miss1.au");
			else play_sound("miss2.au");
			break;
		case SOUND_SUNKIT:
			play_sound("sunkit.au");
			break;
	}
}

#define INVALID_LEAVES_BOARD	0
#define INVALID_OVERLAPS	1

/* Prints some simple messages */
static void invalid_message(int message) {

	clear_status_lines();
	set_color(C_BLUE,C_BOLD);
	do_sound(SOUND_ERROR);
	if (message==0) {
		printxy(10,20,"Invalid! Leaves Board! Try Again.");
	}
	if (message==1) {
		printxy(10,20,"Invalid! Overlaps another ship! Try Again.");
	}
}

/* Remove all of a value from */
/* a grid */
static void purge_grid(int grid[8][8], int type) {

	int i,j;
	for(i=0;i<8;i++) {
		for(j=0;j<8;j++) {
			if (grid[i][j]==(type+3)) grid[i][j]=0;
		}
	}
}


/* Main function to allow user */
/* to place ships */
int get_users_grid(int grid[8][8]) {

	int i,ch,shiptype=0,x,y,ch2,ch3,ch4;
	char shipname[3][19]={"Submarine","Battleship","Aircraft Carrier"};
	int ship_placed[3];
	char text[250];
	int exit_now=0;

	/* Clear placement memory */
	for(i=0;i<3;i++) ship_placed[i]=0;

	set_color(C_RED,C_BOLD);
	printxy(32,1,"Place Your Ships");
	set_color(C_WHITE,C_NORMAL);
	printxy(1,2,"Pick the number corresponding to the type"
		" of ship you want to place. ");
	printxy(1,3,"Then you may either type the coordinate "
		"[e.g. A1] or use the arrow keys to pick");
	printxy(1,4,"the starting block of the ship.  "
		"Then Press Spacebar.  Then pick the direction");
	printxy(1,5,"to put the ship [using NESW or the arrows].  "
		"Then type 'Q' to continue.");
	put_dumb_instructions();

	/* Draw Compas points */
	set_color(C_CYAN,C_BOLD);
	printxy(60,7,"   N");
	printxy(60,8,"   |");
	printxy(60,9,"W<-+->E");
	printxy(60,10,"   | ");
	printxy(60,11,"   S");

	/* Clear the grid */
	clear_grid(grid);
	place_grid(grid,30,7,1);

	/* The big loop */
	do {
		ch=mvgetch(13,9);
		if ( (ch>'0') && (ch<'z') ) {
			if ( (ch>'0') && (ch<'4') )shiptype=ch-'1';
			if ( (ch=='1') || (ch=='2') || (ch=='3') ) {
				if (ship_placed[shiptype]) {
					clear_status_lines();
					sprintf(text,
						"You have already placed the %s!",
						shipname[shiptype]);
					printxy(5,20,text);
					do_sound(SOUND_ERROR);
					printxy(5,21,"Replace it? (y/n)");
					ch4=mvgetch(21,24);
					if (toupper(ch4)=='Y') {
						ship_placed[shiptype]=0;
						purge_grid(grid,shiptype);
						place_grid(grid,30,7,1);
					}
					clear_status_lines();
				}
				if (!ship_placed[shiptype]){
					x=0; y=0;
					clear_side();
					printxy(5,7,"Now Placing: ");
					printxy(6,8,shipname[shiptype]);
					printxy(5,9,"Press Space when done");
					do {
						ch2=mvgetch(8+y,31+x);
						if((ch2==KEY_UP)&&(y>0)) y--;
						if((ch2==KEY_DOWN)&&(y<7)) y++;
						if((ch2==KEY_RIGHT)&&(x<14)) x+=2;
						if((ch2==KEY_LEFT)&&(x>0)) x-=2;
						ch3=toupper(ch2);
						if((ch3>='A')&&(ch3<'I')) y=ch3-'A';
						if((ch3>='1')&&(ch3<'9')) x=2*(ch3-'1');
						place_grid(grid,30,7,1);
						set_color(C_RED,C_BOLD);
						move(8+y,31+x);
						printw("&&");
						refresh();
					} while (ch2!=' ');
					clear_side();
					printxy(5,7,"Now Pick N,S,E, or W");
					printxy(5,8,"Or press an arrow key");
					ch2=mvgetch(7,5);
					x=x/2;
					switch (ch2) {
	          case 'N': case 'n': case KEY_UP :
		       if (y<shiptype+2) {
			  invalid_message(INVALID_LEAVES_BOARD); break;
		       }
		       else if (test_overlap(grid,x,y,0,-1,shiptype+3) ){
		           invalid_message(INVALID_OVERLAPS); break;
		       }
		       else {
			   put_ship(grid,x,y,0,-1,shiptype+3);
			   ship_placed[shiptype]=1;
		       }
		       break;
	          case 'S': case 's': case KEY_DOWN:
	               if (y>8-(shiptype+3)) {
			  invalid_message(INVALID_LEAVES_BOARD); break;
		       }
	               else if (test_overlap(grid,x,y,0,1,shiptype+3) ){
		          invalid_message(INVALID_OVERLAPS); break;
		       }
	               else {
			  put_ship(grid,x,y,0,1,shiptype+3);
			  ship_placed[shiptype]=1;
	               }
	               break;
	          case 'E': case 'e': case KEY_RIGHT:
		       if (x>8-(shiptype+3)) {
			  invalid_message(INVALID_LEAVES_BOARD);
		       }
		       else if (test_overlap(grid,x,y,1,0,shiptype+3) ){
		          invalid_message(INVALID_OVERLAPS);
		       }
		       else {
			  put_ship(grid,x,y,1,0,shiptype+3);
			  ship_placed[shiptype]=1;
		       }
	               break;
		  case 'W': case 'w': case KEY_LEFT:
	               if (x<(shiptype+2)) {
			  invalid_message(INVALID_LEAVES_BOARD); break;
		       }
	               else if (test_overlap(grid,x,y,-1,0,shiptype+3)){
	                  invalid_message(INVALID_OVERLAPS); break;
	               }
	               else {
			  put_ship(grid,x,y,-1,0,shiptype+3);
			  ship_placed[shiptype]=1;
	               }
							break;
						default: /*printw("Invalid Entry")*/;
					}
					clear_side();
					place_grid(grid,30,7,1);
					put_dumb_instructions();
					refresh();
				}
			}

			if (toupper(ch)=='Q') {
				if ((ship_placed[0]==1)&&
					(ship_placed[1]==1)&&
					(ship_placed[2]==1) ) {
					exit_now=1;
				}
				else {
					set_color(C_RED,C_BOLD);
					printxy(5,20,"You need to place one of "
						"each kind of ship before you "
						"continue!");
					do_sound(SOUND_ERROR);
				}
			}
			if (toupper(ch)=='X') quit();
		}
	} while(!exit_now);
	return 0;
}



/* Clear the bottom lines */
void clear_status_lines(void) {
	move(20,1); clrtoeol();
	move(21,1); clrtoeol();
}

/* Clear the side area */
void clear_side(void) {
	printxy(5,7,"                      ");
	printxy(5,8,"                      ");
	printxy(5,9,"                      ");
	printxy(5,10,"                     ");
	printxy(5,11,"                      ");
	printxy(5,12,"                      ");
	printxy(5,13,"                      ");
	refresh();
}

/* Create a Quit Pop-UP */
void quit(void) {
	char ch;
	WINDOW *quit_window;		/* It's a sub-window */

	quit_window=newwin(5,30,10,20);
	wattrset(quit_window,COLOR_PAIR(C_YELLOW)|A_BOLD);
	mvwprintw(quit_window,0,0,"*****************************");
	mvwprintw(quit_window,1,0,"* This will Quit COMPLETELY *");
	mvwprintw(quit_window,2,0,"* Are you sure you want to  *");
	mvwprintw(quit_window,3,0,"* Do this? (y/n)            *");
	mvwprintw(quit_window,4,0,"*****************************");

	wrefresh(quit_window);
	ch=mvgetch(13,37);
	wclear(quit_window);
	wrefresh(quit_window);
	delwin(quit_window);

	if ((ch=='Y') || (ch=='y')) {
		seabattle_exit_curses();
		exit(0);
	}
#ifndef BROKEN_CURSES
	redrawwin(stdscr);
#endif
}

/* Place computers ships */
void get_computers_grid(int grid[8][8]) {

	clear_grid(grid);
	while(place_ship(grid,3));
	while(place_ship(grid,4));
	while(place_ship(grid,5));
}

/* Make sure ships don't overlap */
int test_overlap(int grid[8][8],int x,int y,int xdir,int ydir,int size) {
	int i,j;
	j=0;
	for(i=0;i<size;i++) {
		if (grid[x+(i*xdir)][y+(i*ydir)]!=0) j++;
	}
	return j;
}

/* Put ship in a grid */
void put_ship(int grid[8][8],int x,int y,int xdir,int ydir,int size) {

	int i;

	for(i=0;i<size;i++) {
		grid[x+(i*xdir)][y+(i*ydir)]=size;
	}
}

/* Place the computer's ships */
/* in random locations, but no */
/* overlaps or out of bounds */
int place_ship(int grid[8][8],int size) {

	int x,y,direction;

	if ((grid[x=rand()%8][y=rand()%8])==0) {
		direction=rand()%4;
		switch(direction) {
			case 0: /*north*/
				if ((y>=size)&&
					(test_overlap(grid,x,y,0,-1,size)==0))  {
					put_ship(grid,x,y,0,-1,size);
				}
				else return -1;
				break;
			case 1: /*east*/
				if ((x<(8-size))&&
					(test_overlap(grid,x,y,1,0,size)==0)) {
					put_ship(grid,x,y,1,0,size);
				}
				else return -1;
				break;
			case 2: /*south*/
				if ((y<(8-size))&&
					(test_overlap(grid,x,y,0,1,size)==0)) {
					put_ship(grid,x,y,0,1,size);
				}
				else return -1;
				break;
			case 3: /*west*/
				if ((x>=size)&&
					(test_overlap(grid,x,y,-1,0,size)==0)) {
					put_ship(grid,x,y,-1,0,size);
				}
				else return -1;
				break;
		}
		return 0;
	}

	return -1;
}

/* Draw very simple screen */
/* will be fancier with actual game */
static void display_screen(int users_grid[8][8], int computers_grid[8][8]) {
	place_grid(users_grid,10,5,1);
	place_grid(computers_grid,50,5,1);
	refresh();
}




/* Display a grid on the screen */
void place_grid(int grid[8][8],int x,int y,int show_ships) {

	int i,j;

	move(y,x);
	set_color(C_WHITE,C_BOLD);
	printw("  1 2 3 4 5 6 7 8");
	for(i=0;i<8;i++) {
		move(y+1+i,x);
		set_color(C_WHITE,C_BOLD);
		printw("%c",65+i);

		for (j=0;j<8;j++) {
			switch(grid[j][i]) {
				case 0: printw("  ");
					break;
				case 3: set_color(C_WHITE,C_NORMAL);
					if (show_ships) printw("SS");
					else printw("  ");
					break;
				case 4: set_color(C_WHITE,C_NORMAL);
					if (show_ships) printw("BB");
					else printw("  ");
					break;
				case 5: set_color(C_WHITE,C_NORMAL);
					if (show_ships) printw("AA");
					else printw("  ");
					break;
				case 1: set_color(C_RED,C_BOLD);
					printw("**");
					break;
				case 2: set_color(C_CYAN,C_NORMAL);
					printw("',");
					break;
			}
		}
	}
}

/* ************************END binp.c ******************************** */
