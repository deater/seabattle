/**************************************************************************
 * bgam.c --- Module 2 for SEABATTLE by Vince Weaver                      *
 *  Made for ENEE114 on ---> 28 April 1997                                *
 * Updated for Module 3 ---> 13 May 1997                                  *
 *************************************************************************/

#include <stdio.h>
#include <ncurses.h>     /* Slang support is good for rxvt in linux */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "batt.h"

/* Draws the arrow pointing to */
/* Whoever's turn it is */
/* 0=Computer, 1=Player */
static void do_arrow(int whos_turn, int turn_num) {

	char text[20];

	set_color(C_GREEN,C_BOLD);
	printxy(36,13,"TURN");
	if (whos_turn) {
		set_color(C_RED,C_BOLD);
		printxy(36,14,"<");
		set_color(C_GREEN,C_NORMAL);
		printxy(37,14,"---");
	}
	else {
		set_color(C_GREEN,C_NORMAL);
		printxy(36,14,"---");
		set_color(C_RED,C_BOLD);
		printxy(39,14,">");
	}
	set_color(C_GREEN,C_BOLD);
	sprintf(text,"%i",turn_num);
	printxy(37,16,text);
}

/* Toggles Sound on and Off */
/* And Displays the Change  */
static void print_sound_status(void) {

	set_color(C_BLUE,C_NORMAL);
	sound_on=!sound_on;

	if (sound_on) {
		printxy(32,5,"Sound ON ");
	}
	else {
		printxy(32,5,"Sound OFF");
	}

	if (sound_device) {
		printxy(32,6,"Mode 8bit   ");
	}
	else {
		printxy(32,6,"Mode Speaker");
	}

	set_color(C_BLUE,C_BOLD);
	printxy(32,5,"S");
	printxy(32,6,"M");
}

/* Draw the main screen    */
/* Also called on a redraw */
static void draw_the_screen(DATA *person, int turn_num) {

	set_color(C_BLUE,C_BOLD);
	printxy(31,2,"S E A B A T T L E");

	/* Toggle twice to return to */
	/* Original state */
	print_sound_status();
	print_sound_status();

	set_color(C_BLUE,C_NORMAL);
	printxy(32,7,"Redraw screen");
	printxy(32,8,"Help");
	printxy(32,9,"Quit to menu");
	printxy(32,10,"eXit completely");
	set_color(C_BLUE,C_BOLD);
	printxy(32,7,"R");
	printxy(32,8,"H");
	printxy(32,9,"Q");
	printxy(33,10,"X");
	set_color(C_RED,C_BOLD);

	/* Below Centers the Name under the grid */
	printxy(14-((strlen(person->first_name))/2),14,person->first_name);
	printxy(60,14,"Computer");
	do_arrow(1,turn_num);
	set_color(C_GREEN,C_NORMAL);
	printxy(3,22,"Use The Arrows or Enter Coordinates to aim.  "
		"Then Hit Spacebar to Fire");
}

/* A generic Message Handler */
static void print_message(int color,int person,char *message) {

	set_color(color,C_BOLD);
	if(!person) {
		printxy(5,17,"                             ");
		printxy(5,17,message);
	}
	else {
		/* The following sort of centers the message slightly */
		printxy(45,17,"                               ");
		if (strlen(message)<10) {
			printxy(55,17,message);
		}
		else {
			printxy(45,17,message);
		}
	}
}


/* The Main Routine */
static void play_the_game(DATA *person) {

	int users_grid[8][8],computers_grid[8][8];
	int x=0,y=0,ch2=0,ch3=0,oldcompx=0,oldcompy=0,compx=0;
	int compy=0,turn=0,prev_hit=0;
	int direct=0,second_hit=0;
	char text[100];		/* Used for sprintfs */

	/* A useful structure */
	struct SHIPS_LEFT{
		int air;
		int batt;
		int sub;
	} computer_hits_left = {5,4,3},user_hits_left = {5,4,3};


	/* Get Human's Pieces */
	clear();
	get_users_grid(users_grid);

	/* Place computer's pieces */
	clear();
	refresh();
	get_computers_grid(computers_grid);

	/* Set up display */
	draw_the_screen(person,0);

	place_grid(users_grid,5,5,1);
	place_grid(computers_grid,55,5,0);

   while (ch3!='Q') {                      /* Main Loop */
      turn++;                              /* Count how many turns */
      do_arrow(1,turn);
      do {                        /* Stuck 30 mins looking to find bug */
	 ch2=mvgetch(6+y,56+x);   /* only to find 6,56 switched here   */

	 if((ch2==KEY_UP)&&(y>0)) y--;      /* Checks arrow keys */
	 if((ch2==KEY_DOWN)&&(y<7)) y++;
	 if((ch2==KEY_RIGHT)&&(x<14)) x+=2;
	 if((ch2==KEY_LEFT)&&(x>0)) x-=2;

	 ch3=toupper(ch2);                   /* Checks for co-ordinates */
	 if((ch3>='A')&&(ch3<'I')) y=ch3-'A';
	 if((ch3>='1')&&(ch3<'9')) x=2*(ch3-'1');

	 place_grid(computers_grid,55,5,0);  /* Puts red && cursor */
	 set_color(C_RED,C_BOLD);
	 move(6+y,56+x);
	 printw("&&");
	 refresh();

	 if (ch3=='R') draw_the_screen(person,turn);  /* Menu Items */
	 if (ch3=='S') print_sound_status();
	 if (ch3=='X') quit();
	                              /* Checks to see if repeat shot */
	 if ((ch2==' ') && ((computers_grid[x/2][y]==1)
		        || (computers_grid[x/2][y]==2))) {
	    print_message(C_GREEN,1,"YOU ALREADY WENT THERE");
	    ch2='O';
	 }
     } while ((ch2!=' ')&&(ch3!='Q'));

     switch (computers_grid[x/2][y]){       /* See where you hit */
            case 0: computers_grid[x/2][y]=2;
                    print_message(C_CYAN,1,"MISS");
                    do_sound(2);
                    break;
            case 3: print_message(C_RED,1,"HIT");
                    if (--computer_hits_left.sub==0) {
	               print_message(C_GREEN,1,"You Sank My Submarine!");
                       do_sound(3);
	            }
                    do_sound(1);
	            computers_grid[x/2][y]=1;
                    break;
            case 4: print_message(C_RED,1,"HIT");
                    if (--computer_hits_left.batt==0) {
		       print_message(C_GREEN,1,"You Sank My Battleship!");
		       do_sound(3);
	            }
                    do_sound(1);
                    computers_grid[x/2][y]=1;
                    break;
            case 5: print_message(C_RED,1,"HIT");
                    if (--computer_hits_left.air==0){
		       print_message(C_GREEN,1,"You Sank My Aircraft Carrier!");
		       do_sound(3);
	            }
                    do_sound(1);
                    computers_grid[x/2][y]=1;
                    break;
     }

     place_grid(computers_grid,55,5,0);

     if( (computer_hits_left.sub==0) &&   /* Check to see if you won */
         (computer_hits_left.air==0) &&
         (computer_hits_left.batt==0) ) {
            print_message(C_RED,1,"YOU WON! PRESS A KEY TO CONTINUE");
	    getch();
            clear(); refresh();
     set_color(C_BLUE,C_BOLD);
printxy(2,2," '||' '|'  ..|''||   '||'  '|'    '|| '||'  '|' '||' '|.   '|'   ");
printxy(2,3,"   || |   .|'    ||   ||    |      '|. '|.  .'   ||   |'|   |    ");
printxy(2,4,"    ||    ||      ||  ||    |       ||  ||  |    ||   | '|. |    ");
printxy(2,5,"    ||    '|.     ||  ||    |        ||| |||     ||   |   |||    ");
printxy(2,6,"   .||.    ''|...|'    '|..'          |   |     .||. .|.   '|    ");
     set_color(C_WHITE,C_BOLD);
        if(turn<person->best_score) {

	   sprintf(text,"NEW BEST SCORE: %i turns.  Your previous best was %i",
		   turn,person->best_score);
	   person->best_score=turn;
	}
	else sprintf(text,"You won in %i turns.  Your previous best is %i.",
		           turn,person->best_score);
           printxy(5,8,text);
	   printxy(27,20,"Press any key to continue");
           getch();
	   do_high_score(person->first_name,turn);
	   person->times_played++;
           return;
     }

     /* The Computer's Turn */
                                      /* 0=up 1=down 2=left 3=right */
     do_arrow(0,turn);

      if ((prev_hit)&&(second_hit)) {    /* Complicated Algorythm */
	 if (direct==0){                 /* To have computer follow hits */
	    compy=compy-1; compx=oldcompx; /* And be intelligent.  Works */
	    if(!INBOUNDSY(compy,compx)) direct++;  /* Pretty well */
	 }

	 if (direct==1) {
	    compy=compy+1; compx=oldcompx;
	    if(!INBOUNDSY(compy,compx)) direct++;
	 }

	 if (direct==2) {
	    compx=compx-1; compy=oldcompy;
	    if(!INBOUNDSX(compx,compy)) direct++;
	 }

	 if (direct==3) {
	    compx=compx+1; compy=oldcompy;
	    if (!INBOUNDSX(compx,compy)) direct++;
	 }
      }
      else {
	 if (direct==0){
	    compy=oldcompy-1; compx=oldcompx;
	    if(!INBOUNDSY(compy,compx)) direct++;
	 }

	 if (direct==1) {
	    compy=oldcompy+1; compx=oldcompx;
	    if(!INBOUNDSY(compy,compx)) direct++;
	 }

	 if (direct==2) {
	    compx=oldcompx-1; compy=oldcompy;
	    if(!INBOUNDSX(compx,compy)) direct++;
	 }

	 if (direct==3) {
	    compx=oldcompx+1; compy=oldcompy;
	    if (!INBOUNDSX(compx,compy)) direct++;
	 }

      }
      if (direct==4) prev_hit=0;

      if (prev_hit==0) {
         direct=0;
	 while( (users_grid[compx=rand()%8][compy=rand()%8]==1)
	   || (users_grid[compx][compy]==2) );
      }

    switch(users_grid[compx][compy]){       /* Switch to report hits/misses */
      case 0: users_grid[compx][compy]=2;
              print_message(C_CYAN,0,"MISS");
              do_sound(2);
              second_hit=0;
              break;
      case 3: print_message(C_RED,0,"HIT");
              if(--user_hits_left.sub==0) {
	        print_message(C_GREEN,0,"I Sank Your Submarine!");
                do_sound(3);
		prev_hit=0;
	      }
              else if (prev_hit==1) second_hit=1;
              else { prev_hit=1; second_hit=0;
	             oldcompx=compx; oldcompy=compy;
	      }
              do_sound(1);
	      users_grid[compx][compy]=1;
              break;
      case 4: print_message(C_RED,0,"HIT");
              if(--user_hits_left.batt==0) {
		 print_message(C_GREEN,0,"I Sank Your Battleship!");
		 do_sound(3);
		 prev_hit=0;
	      }
              else if (prev_hit==1) second_hit=1;
                   else {prev_hit=1; second_hit=0;
		         oldcompx=compx; oldcompy=compy;
		   }
              do_sound(1);
              users_grid[compx][compy]=1;
              break;
      case 5: print_message(C_RED,0,"HIT");
              if(--user_hits_left.air==0){
		 print_message(C_GREEN,0,"I Sank Your Aircraft Carrier!");
		 do_sound(3);
		 prev_hit=0;
	      }
              else if (prev_hit==1) second_hit=1;
                   else {prev_hit=1; second_hit=0;
		         oldcompx=compx; oldcompy=compy;
		   }
              do_sound(1);
              users_grid[compx][compy]=1;
              break;
   }

   place_grid(users_grid,5,5,1);

   if( (user_hits_left.sub==0) &&      /* Detect if the computer wins */
       (user_hits_left.air==0) &&
       (user_hits_left.batt==0) ) {
         print_message(C_RED,0,"I WON! Press any key to continue");
         getch();
         clear(); refresh();
      set_color(C_BLUE,C_BOLD);
printxy(2,2,"'||' '|'  ..|''||   '||'  '|'    '||'       ..|''||    .|'''.|  '||''''|");
printxy(2,3,"  || |   .|'    ||   ||    |      ||       .|'    ||   ||..  '   ||  .");
printxy(2,4,"   ||    ||      ||  ||    |      ||       ||      ||   ''|||.   ||''|");
printxy(2,5,"   ||    '|.     ||  ||    |      ||       '|.     || .     '||  ||");
printxy(2,6,"  .||.    ''|...|'    '|..'      .||.....|  ''|...|'  |'....|'  .||.....|");
      set_color(C_RED,C_BOLD);

         sprintf(text,"I won in %i turns",turn);
         printxy(5,8,text);
         printxy(27,22,"Press Any Key To Continue");
         printxy(32,10,"The Computer's Pieces:");
         place_grid(computers_grid,32,12,1);
         getch();
         do_high_score("Computer",turn);
         return;
   }
 }

}


/* Not utilized yet */
static void do_options(void) {
}

/* Not implemented yet */
static void do_help(void) {
}

/* The Main Menu */
int main_menu(DATA *person, MAIN_THINGY *main_thing) {

	int ch=0;
	char text[100];
	static int first_time=0;

	while(1){
		/* Play seabattle sound if first time through */
		if ((sound_device!=SOUND_DEVICE_SPEAKER) && (first_time==0)) {
			play_sound("opening.au");
		}

		if (!first_time) first_time=5;

		/* Draw Menu */
		clear();
		set_color(C_RED,C_BOLD);
		printxy(5,2,"SeaBattle Menu");
		set_color(C_BLUE,C_BOLD);
		for(ch=1;ch<8;ch++){
			sprintf(text,"%c",ch+'0');
			printxy(6,ch+2,text);
		}
		set_color(C_BLUE,C_NORMAL);
		printxy(7,3,". Play Normal Game vs Computer");
		printxy(7,4,". Play vs. Remote Human [NOT AVAILABLE]");
		printxy(7,5,". View High Score List");
		printxy(7,6,". Re-enter User Info");
		printxy(7,7,". Options [Non-functional]");
		printxy(7,8,". Help [Non-functional]");
		printxy(7,9,". Quit");
		set_color(C_RED,C_BOLD);
		printxy(5,10,"---->");
		ch=mvgetch(10,11);

		/* Execute the Various Cases */
		switch(ch){
			case '1': play_the_game(person); break;
			case '3': do_high_score("I Love Marie",156); break;
			case '4': person=enter_info(main_thing); break;
			case '5': do_options(); break;
			case '7': quit(); break;
			case '6': do_help(); break;
			case '2':
			default:
				set_color(C_RED,C_BOLD);
				do_sound(0);
				printxy(5,15,"INVALID RESPONSE! TRY AGAIN!");
				break;
		}
		write_data_to_disk(main_thing);
   	}
}

/********************* END bgam.c *************************************/
