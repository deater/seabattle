##############################################################
#  Makefile for Seabattle 1.0  -- by Vince Weaver            #
#                                                            #
#  Written on Linux 2.1.35                                   #
#                                                            #
#  To modify for your configuration, add or remove the #     #
#                                                            #
##############################################################

#Your compiler.  If gcc doesn't work, try CC
#CC = gcc
#CC = cc

#On Suns, SGIs, and other Unix systems uncomment the following
#
#PLATFORM = Sun_and_Standard_Unix
#C_OPTS = 
#L_OPTS = -lcurses

#On FreeBSD/Linux, uncomment the following
#
ifndef PREFIX
PREFIX = .
endif

PLATFORM = FreeBSD/Linux
DATADIR = ${PREFIX}/share/seabattle
C_OPTS = ${CFLAGS} -DDATADIR=\"${DATADIR}\"
L_OPTS = $(LDFLAGS) -lncurses

#For Dos/Windows DJGPP (you need the curses lib)
#PLATFORM = DOS_DJGPP
#C_OPTS = -O2 -Wall -DBROKEN_CURSES
#L_OPTS = -lcurso

######################################################################
#         THERE IS NO NEED TO EDIT ANYTHING BELOW THIS LINE          #
######################################################################

all:	seabattle

clean:
	rm -f *.o *~ seabattle

seabattle:	batt.o binp.o bgam.o bdb.o
	$(CC) $(C_OPTS) -o seabattle batt.o binp.o bgam.o bdb.o $(L_OPTS)
	@strip seabattle

batt.o:	batt.c batt.h
	@echo
	@echo "Compiling for platform: $(PLATFORM)"
	@echo "Be sure to edit the Makefile if the above is not correct"
	@echo
	$(CC) $(C_OPTS) -c batt.c

bgam.o:	bgam.c batt.h
	$(CC) $(C_OPTS) -c bgam.c

binp.o:	binp.c batt.h
	$(CC) $(C_OPTS) -c binp.c

bdb.o:	bdb.c batt.h
	$(CC) $(C_OPTS) -c bdb.c
