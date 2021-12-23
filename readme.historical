   .|'''|                    '||'''|,            ||      ||    '||`
   ||                         ||   ||            ||      ||     ||
   `|'''|, .|''|,  '''|.      ||;;;;    '''|.  ''||''  ''||''   ||  .|''|,
    .   || ||..|| .|''||      ||   ||  .|''||    ||      ||     ||  ||..||
    |...|' `|...  `|..||.    .||...|'  `|..||.   `|..'   `|..' .||. `|...

                      version 1.0 -- 14 May 1997

        A Game By Vince Weaver -- Who cannot draw ASCII ships
          weave@eng.umd.edu  http://www.glue.umd.edu/~weave
                 Another Amazing Project for ENEE114
		 
\---------------------------------------------------------------------------/
                   Works Best on Linux (color and sound) 
          Also works on most UNIX systems and Windows/DOS (DJGPP)
/---------------------------------------------------------------------------\

Compiling and Installation Instructions:

  For Unix (Compiling from Source):
  
      Untar the file.  All the untarred files should end up in a directory
      called "seabattle-1.0".
  
      Edit the Makefile.  Be sure to uncomment the lines applicable to your
      Particular Platform/Compiler.

      Type "make clean", then "make all"  [w/o the quotes of course]

      This will compile on most Unixes with the "curses" library.
      SUN, SGI and Linux known to work.  Also works on DJGPP for Dos/Windows.
      DJGPP requires the pdcurses library, obtainable at
      http://oak.oakland.edu/pub/simtelnet/gnu/djgpp/v2tk/pdc22.zip
      
      SEABATTLE WILL NOT COMPILE ON DIGITAL UNIX.  (faulty curses lib?)

      To run [assuming it compiled fine] type "battleship"
  
  For Dos/Windows:
  
      Unzip zip file.  If using pkzip, use "pkunzip -d seab10.zip" to 
      preserve directory.
      
      All the files should be there.  Run "seabattl.exe" to play.
  
\---------------------------------------------------------------------------/  
  ERRORS:
/---------------------------------------------------------------------------\

Problem:
   "Sorry, I don't know anything about your "XXXX" terminal"
    (where XXXX is the terminal you are using) appears.

Solution:    
    Try running it as 'env TERM="vt102" seabattle' or
    'env TERM="vt100" seabattle' .  This might be necessary if your
    termcap is not set up right.
    
Problem:
   Garbage on the screen and things are not lining up right.
   
Solution:
   As with the previous program, try running the program with one of the 
   following commands: "env TERM=vt102 seabattle" or 
   "env TERM=vt100 seabattle"

Problem:
   High score list/ user info is not saving.
   
Solution:
   Make sure you have write permissions to the directory seabattle is
   installed in.

Problem: 
   I hate the sounds!
   
Solution: 
   Sorry.  Press "S" to turn them off while playing the game.
   If it is extremely annoying, try changing the value of the
   sound variable in batt.c .
   
\----------------------------------------------------------------------------/
  USER'S GUIDE:
/----------------------------------------------------------------------------\

  Run "seabattle" or "env TERM=vt102 seabattle" etc, so that the title 
  screen comes up.
  
  If you are on a Linux text console, or in Windows/DOS it should be full 
  color.  All other Unixes and xterms will be black/white.  This seems to 
  be a limitation of the curses library; only Linux seems to implement 
  standard color functions. SeaBattle will tell you if color is not supported.
  
  [Linux color will not work on xterm or rxvt.  To get to a linux text
   console from X, press "CTRL-ALT-F1" or "CTRL-ALT-F2" etc and log into
   the text screen.  Then run it]
   
  If a sound card is installed and '/dev/audio' is found, SeaBattle will ask
  if you want digital sound effects.  These will play during the game.

  Next you will be prompted for some information.  This was a 
  requirement of the project I wrote this for.  The only really
  necessary info is your first name; you can leave the rest blank.
  
  Next the menu will come up.  To start a new game, press '1'.
  
  SeaBattle will ask you to place your ships.  Choose the number
  corresponding to the ship you want.  Then use the arrow keys to pick
  where one end of the ship will go.  [You can also type the co-ordinate
  out, e.g. A1 or B5].  Press the spacebar.  Then type N,E,S or W to
  point the ship in the direction you want.  [You can also use the arrow
  keys].
  
  Repeat this for each ship.  If you want to re-position the ship, type
  the number for it again.  You will be prompted if you want to move it.
  When done, press "Q" to continue.
  
  Now it is time to play the game.  Place the cursor to where you want to 
  shoot using the same keys you used to position the ships.
  
  Press SpaceBar to fire.  It should be self explanatory.
  
  The sound can be toggled on/off using the "S" key.  The menu bar in
  the middle should also be fairly self-explanatory.
  
\-----------------------------------------------------------------------/
  Extra Stuff:
/-----------------------------------------------------------------------\
  
  The newest version of sea-battle can be obtained at
      http://www.glue.umd.edu/~weave/vmwprod/seabattle.html
     
  I will also upload it to the SunSite linux archive
      http://sunsite.unc.edu
     
  Send any bug reports, comments, or anything else to
      Vince Weaver, weave@eng.umd.edu
      
   Check out other VMW Software productions at 
      http://www.glue.umd.edu/~weave/vmwprod
  
  
  I wrote this as the final project in my ENEE114 C programming course
  at the University of Maryland College Park.  It is released as 
  freeware.  Feel free to distribute.
  
  I would like to thank my TA Shiloh Heurich.  Also I'd like to thank
  my girlfriend Marie, and various people who come to my talker
  (telnet hal.dorm.umd.edu 7000   up during the school year)
  
  Be sure to try other programs I have written.  Have fun!
                                ------Vince Weaver
				      13 May 1997 - 16:16:52
  
  Sea Battle -- A VMW Software Production ----------------------------
