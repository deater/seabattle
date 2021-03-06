/**************************************************************************
 * batt.c --- Module 0 for SEABATTLE by Vince Weaver                      *
 *  Made for ENEE114 on ---> 15 April 1997                                *
 *  Updated for Module 2 --> 27 April 1997                                *
 *  Updated for Module 3 --> 13 May 1997                                  *
 *************************************************************************/

#include <ncurses.h>	/* Slang support is good for rxvt in linux */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>	/* Needed to randomize from time */

#include "batt.h"

/* Global Variables */
int sound_on=SOUND_ON;				/*  These are exported */
int sound_device=SOUND_DEVICE_SPEAKER;		/* Awkward, but keeps */
int salvo_play=0;				/* paramaters uncluttered*/
DATA *current_player;

int main(int argc, char **argv) {

	MAIN_THINGY *main_thing;

	if (( main_thing=make_main_thingy())==NULL) {
		out_of_memory_panic();
	}

	srand(time(NULL));			/* Randomize timer */
	seabattle_init_curses();		/* Initialize display */
	clear();				/* Clear the screen */
	refresh();
	draw_opening();				/* Draw title screen */
	clear();
	refresh();
	current_player=enter_info(main_thing);	/* Prompt to enter info */

	while (main_menu(current_player,main_thing)!=0);   /* Main Loop */

	return 0;
}

/********************* END batt.c *************************************/
