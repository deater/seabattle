/*************************************************************************
 * binp.c --- The First Module of Vince Weaver's SEA BATTLE program      *
 *            For ENEE114.   Finished 15 April 1997                      *
 ************************************************************************/

#include "batt.h"                          /* Include prototypes */

extern int sound_on;                       /* It was simplest to define */
extern int sound_device;                   /* These globally */

void init_curses(void)                     /* INIT CURSES -- inits curses */
{                                          /* library */
   initscr();
   start_color();
   init_pair(7,COLOR_WHITE,COLOR_BLACK);   /* Allocate the colors we'll use */
   init_pair(1,COLOR_BLUE,COLOR_BLACK);
   init_pair(2,COLOR_GREEN,COLOR_BLACK);
   init_pair(3,COLOR_CYAN,COLOR_BLACK);
   init_pair(4,COLOR_RED,COLOR_BLACK);
   init_pair(6,COLOR_YELLOW,COLOR_BLACK);
   cbreak();                               /* Start key by key mode */
   noecho();                               /* Don't echo */
   nonl();                                 /* Um I forget view the manpage */
#ifndef USE_SLANG
   intrflush(stdscr,FALSE);                /* Slang doesn't like this */
#endif
   keypad(stdscr,TRUE);                    /* Enable arrow keys */
}


void set_color(int color,int bold)         /* Function to set colors */
{
   if(bold) attrset(COLOR_PAIR(color)|A_BOLD);
   else attrset(COLOR_PAIR(color)|A_NORMAL);
}


void printxy(int x,int y,char *tempst)     /* Simple function that saves */
{                                          /* repetitive move() printw() */
   move(y,x);                              /* sequences */
   printw(tempst);
}


void exit_curses(void)                     /* Shutdown curses nicely so the */
{                                          /* terminal works properly */
   clear();
   refresh();
   nocbreak();
   echo();
   endwin();  
}


void draw_opening(void)                    /* Draw opening screen */
{
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
   printxy(11,14,"weave@eng.umd.edu   http://www.glue.umd.edu/~weave");
   set_color(C_GREEN,C_BOLD);
   printxy(18,15,"Another Amazing Project for ENEE114");
   set_color(C_WHITE,C_BOLD);
   if (!has_colors()) { 
      printxy(0,17,"COLOR NOT AVAILABLE ON THIS TERMINAL. Using Black and White.");
   }
   refresh();
   set_color(C_WHITE,C_NORMAL);
   move(19,0);
                                           /* Sees if /dev/audio is available */
   if ((ftemp=fopen("/dev/audio","w"))==NULL) {
      printw("No digital sound available.  Press any key to continue.");
      ch=mvgetch(19,55);
   }
   else {
      fclose(ftemp);                       /* Sets up sound values */
      printw("Do you want nifty digital sound effects (y/n) ?");
      ch=mvgetch(19,49);
      if ( (ch=='y') || (ch=='Y') ) sound_device=1;
      else sound_device=0;
   }
}


void put_dumb_instructions(void)           /* This made code slightly less */
{                                          /* Cluttered */
   set_color(C_GREEN,C_BOLD);
   printxy(5,7,"Choose a ship:");
   printxy(5,8,"1. Submarine");
   printxy(5,9,"2. Battleship");
   printxy(5,10,"3. Aircraft Carrier");
   printxy(5,11,"Q. Continues with game");
   printxy(5,12,"X. Quits game entirely");
   printxy(5,13,"--->");
}


void clear_grid(int grid[8][8])            /* Clears an 8x8 grid */
{
   int i,j;
   for(i=0;i<8;i++)
     for(j=0;j<8;j++)
       grid[i][j]=0;
}

void do_sound(int which_one)               /* Plays appropriate sound */
{  
   if (!sound_on) return;                  /* through appropriate device */
   if (which_one==0) beep();               /* 0=error 1=hit 2=miss 3=sunkit*/
                                           /* Incomplete as of yet */
   if (which_one==1) 
      if (sound_device) 
         if(rand()%2) system("cat hit1.au > /dev/audio ");
         else system("cat hit2.au > /dev/audio ");
      else beep();
   if (which_one==2)
      if (sound_device)
         if(rand()%2) system("cat miss1.au > /dev/audio ");
         else system("cat miss2.au > /dev/audio ");
   if (which_one==3) 
      if (sound_device) system("cat sunkit.au > /dev/audio ");
      else beep();
}


int get_users_grid(int grid[8][8])         /* Main function to allow user */
{                                          /* to place ships */
   int i,ch,shiptype=0,x,y,ch2,ch3,ch4;
   char shipname[3][19]={"Submarine","Battleship","Aircraft Carrier"};
   int ship_placed[3];     
   char text[250];
   int exit_now=0;
  
   for(i=0;i<3;i++) ship_placed[i]=0;      /* Clear placement memory */
   set_color(C_RED,C_BOLD);
   printxy(32,1,"Place Your Ships");
   set_color(C_WHITE,C_NORMAL);
   printxy(1,2,"Pick the number corresponding to the type of ship you want to place. "); 
   printxy(1,3,"Then you may either type the coordinate [e.g. A1] or use the arrow keys to pick");
   printxy(1,4,"the starting block of the ship.  Then Press Spacebar.  Then pick the direction");
   printxy(1,5,"to put the ship [using NESW or the arrows].  Then type 'Q' to continue.");
   put_dumb_instructions();
   set_color(C_CYAN,C_BOLD);
   printxy(60,7,"   N");                   /* Draw Compas points */
   printxy(60,8,"   |");
   printxy(60,9,"W<-+->E");
   printxy(60,10,"   | ");
   printxy(60,11,"   S");
   clear_grid(grid);                       /* Clear the grid */
   place_grid(grid,30,7,1);
   
   do {                                    /* The big loop */
       ch=mvgetch(13,9);
       if ( (ch>'0') && (ch<'z') ) { 
          if ( (ch>'0') && (ch<'4') )shiptype=ch-'1';  
          if ( (ch=='1') || (ch=='2') || (ch=='3') ){
	     if (ship_placed[shiptype]){
	        clear_status_lines();
	        sprintf(text,"You have already placed the %s!",shipname[shiptype]);
	        printxy(5,20,text);
	        do_sound(0);
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
			  simple_message(0); break;
		       }
		       else if (test_overlap(grid,x,y,0,-1,shiptype+3) ){
		           simple_message(1); break;
		       }
		       else {
			   put_ship(grid,x,y,0,-1,shiptype+3); 
			   ship_placed[shiptype]=1;  
		       }
		       break;
	          case 'S': case 's': case KEY_DOWN:
	               if (y>8-(shiptype+3)) {
			  simple_message(0); break;
		       }
	               else if (test_overlap(grid,x,y,0,1,shiptype+3) ){
		          simple_message(1); break;
		       }
	               else {
			  put_ship(grid,x,y,0,1,shiptype+3); 
			  ship_placed[shiptype]=1;
	               }
	               break;
	          case 'E': case 'e': case KEY_RIGHT:
		       if (x>8-(shiptype+1)) {
			  simple_message(0); break;
		       }
		       else if (test_overlap(grid,x,y,1,0,shiptype+3) ){
		          simple_message(1); break;
		       }
		       else {
			  put_ship(grid,x,y,1,0,shiptype+3); 
			  ship_placed[shiptype]=1;
		       }
	               break;
		  case 'W': case 'w': case KEY_LEFT:
	               if (x<(shiptype+2)) {
			  simple_message(0); break;
		       }
	               else if (test_overlap(grid,x,y,-1,0,shiptype+3)){
	                  simple_message(1); break;
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
         if (toupper(ch)=='Q'){
            if ((ship_placed[0]==1)&&(ship_placed[1]==1)&&(ship_placed[2]==1) )
	       exit_now=1;
	    else {
	       set_color(C_RED,C_BOLD);
	       printxy(5,20,"You need to place one of each kind of ship before you continue!");
	       do_sound(0);
	    }	 
         }
         if (toupper(ch)=='X') quit();
       }
   } while(!exit_now);
   return 0;
}

void simple_message(int message)           /* Prints some simple messages */
{
   clear_status_lines();
   set_color(C_BLUE,C_BOLD);
   do_sound(0);
   if (message==0) printxy(10,20,"Invalid! Leaves Board! Try Again.");
   if (message==1) printxy(10,20,"Invalid! Overlaps another ship! Try Again.");
}

void clear_status_lines(void)              /* Clear the bottom lines */
{
     move(20,1); clrtoeol();
     move(21,1); clrtoeol();
}

void clear_side(void)                      /* Clear the side area */
{
   printxy(5,7,"                      ");
   printxy(5,8,"                      ");
   printxy(5,9,"                      ");
   printxy(5,10,"                     ");
   printxy(5,11,"                      ");
   printxy(5,12,"                      ");
   printxy(5,13,"                      ");
   refresh();  
}

void quit(void)                            /* Create a Quit Pop-UP */
{
   char ch;
   WINDOW *quit_window;                    /* It's a sub-window */
   
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
   if ((ch=='Y') || (ch=='y')) {exit_curses(); exit(0); }
#ifndef BROKEN_CURSES
   redrawwin(stdscr);
#endif
}

void get_computers_grid(int grid[8][8])    /* Place computers ships */
{
   clear_grid(grid);
   while(place_ship(grid,3));
   while(place_ship(grid,4));
   while(place_ship(grid,5));
}
   
                                           /* Make sure ships don't overlap */
int test_overlap(int grid[8][8],int x,int y,int xdir,int ydir,int size)
{
   int i,j;
   j=0;
   for(i=0;i<size;i++)
     if(grid[x+(i*xdir)][y+(i*ydir)]!=0) j++;
   return j;
}

                                           /* Put ship in a grid */
void put_ship(int grid[8][8],int x,int y,int xdir,int ydir,int size)
{
   int i;
   for(i=0;i<size;i++)
     grid[x+(i*xdir)][y+(i*ydir)]=size;
}


int place_ship(int grid[8][8],int size)    /* Place the computers ships */
{                                          /* in random locations, but no */
   int x,y,direction;                      /* overlaps or out of bounds */
   
   if((grid[x=rand()%8][y=rand()%8])==0){
        direction=rand()%4;
      switch(direction){
       case 0: /*north*/ if ((y>=size)&&(test_overlap(grid,x,y,0,-1,size)==0))  {
	                    put_ship(grid,x,y,0,-1,size);
                         }
	                 else return -1;
	                 break;
       case 1: /*east*/ if ((x<(8-size))&&(test_overlap(grid,x,y,1,0,size)==0)) {
                           put_ship(grid,x,y,1,0,size);
                        }
	                else return -1;
	                break;
       case 2: /*south*/ if ((y<(8-size))&&(test_overlap(grid,x,y,0,1,size)==0)) {
                            put_ship(grid,x,y,0,1,size);
                         }
	                 else return -1;
	                 break;
       case 3: /*west*/ if ((x>=size)&&(test_overlap(grid,x,y,-1,0,size)==0)) {
                           put_ship(grid,x,y,-1,0,size);
                        }
	               else return -1;
	               break;
      }
   return 0;
   }
   else return -1;
}
   
                                           /* Draw very simple screen */
void display_screen(int users_grid[8][8],int computers_grid[8][8])
{                                          /* will be fancier with actual game */
   place_grid(users_grid,10,5,1);
   place_grid(computers_grid,50,5,1);
   refresh();
}

void purge_grid(int grid[8][8],int type)   /* Remove all of a value from */
{                                          /* a grid */
   int i,j;
   for(i=0;i<8;i++) for(j=0;j<8;j++) 
     if (grid[i][j]==(type+3)) grid[i][j]=0;
}


void place_grid(int grid[8][8],int x,int y,int show_ships) 
/* Display a grid on the screen */
{
   int i,j;
   
   move(y,x);
   set_color(C_WHITE,C_BOLD);
   printw("  1 2 3 4 5 6 7 8");
   for(i=0;i<8;i++){
      move(y+1+i,x);
      set_color(C_WHITE,C_BOLD);
      printw("%c",65+i);
      for(j=0;j<8;j++)
         switch(grid[j][i]){
	  case 0: printw("  "); break;
	  case 3: set_color(C_WHITE,C_NORMAL); 
	          if(show_ships) printw("SS"); 
	          else printw("  ");
	          break;
	  case 4: set_color(C_WHITE,C_NORMAL); 
	          if(show_ships) printw("BB");
	          else printw("  ");
	          break;
	  case 5: set_color(C_WHITE,C_NORMAL); 
	          if(show_ships) printw("AA"); 
	          else printw("  ");
	          break;
	  case 1: set_color(C_RED,C_BOLD);  printw("**"); break;
	  case 2: set_color(C_CYAN,C_NORMAL); printw("',"); 
	 }
   }
}

/* ************************END binp.c ******************************** */

