/*------------------------------------------------------------------|
 | bdb.c -- module 3 of the SEABATTLE game for ENEE114              |
 |          13 May 1997  -- by Vince Weaver                         |
 |------------------------------------------------------------------*/

#include <stdio.h>
#include <ncurses.h>		/* Slang support is good for rxvt in linux */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "batt.h"

void out_of_memory_panic(void) {	/* What to do if malloc() fails */
	printf("Out of memory!\n");
	exit(2);
}

/* Declare the MAIN THINGY */
/*  (i.e. root )           */

MAIN_THINGY *make_main_thingy(void) {
	MAIN_THINGY *main_thing;
	if( (main_thing= (MAIN_THINGY *)malloc(sizeof(MAIN_THINGY)) )!=NULL) {
		main_thing->head=main_thing->tail=NULL;
		main_thing->num_elements=0;
	}
	return main_thing;
}

/* partly borrowed /usr/src/linux/lib/string.c */
/* like strcmp, but case-insensitive    */
static int my_string_comp(const char *cs, const char *ct) {

	signed char res;

	while(1) {
		if ((res = toupper(*cs)-toupper(*ct++))!=0 || !*cs++) break;
	}
	return res;
}

/* Insert Data... Sorted Alphabetically */
/* Automatically sorts it */
static DATA *insert_new_data(MAIN_THINGY *main_thing,DATA *datum) {

	ELEMENT *new_elem,*old_elem;

	if ( ( new_elem=(ELEMENT *)malloc(sizeof(ELEMENT)) )==NULL) {
		out_of_memory_panic();
	}

	new_elem->data=datum;
	main_thing->num_elements++;

	/* Special Case for first element */
	if (main_thing->num_elements==1){
		main_thing->head=main_thing->tail=new_elem;
		new_elem->prev=new_elem->next=NULL;
		return new_elem->data;
	}

	old_elem=main_thing->head;

	/* Traverse List */
	/* Inserting before if name less than*/

	while( (old_elem!=NULL) ){
		if ((my_string_comp( new_elem->data->last_name,
		        old_elem->data->last_name ) )<0)  {
			new_elem->next=old_elem;
			new_elem->prev=old_elem->prev;
			if (old_elem->prev==NULL) {
				main_thing->head=new_elem;
			}
			else {
				old_elem->prev->next=new_elem;
			}
			old_elem->prev=new_elem;
			return new_elem->data;
		}

		if ( (my_string_comp( new_elem->data->last_name,
			old_elem->data->last_name) ==0) &&
			(my_string_comp(new_elem->data->first_name,
			old_elem->data->first_name) <=0) ) {
			new_elem->next=old_elem;
			new_elem->prev=old_elem->prev;
			if (old_elem->prev==NULL) {
				main_thing->head=new_elem;
			}
			else {
				/* I forgot this; */
				/* That was what was seg-faulting */
				old_elem->prev->next=new_elem;
			}
			old_elem->prev=new_elem;
			return new_elem->data;
		}

		/* Tack to the end if hit a NULL */
		if (old_elem->next==NULL) {
			new_elem->next=old_elem->next;
			new_elem->prev=old_elem;
			old_elem->next=new_elem;
			if (new_elem->next==NULL) {
				main_thing->tail=new_elem;
			}
			return new_elem->data;
		}
		old_elem=old_elem->next;
	}
	/* This prevents a WARNING */
	return old_elem->data;

}


/* My own, SUPERIOR version of fgets */
/* Handles \n much better */
/* 2021 vince: urgh this sounds like a bad idea */
static char *read_string_from_disk(FILE *fff,char *string) {

	int ch,i=0;
	char temp[100];

	strcpy(temp,"");
	while ((ch=fgetc(fff))==' ');
	while ( (ch!='\n') && (ch!=EOF) ) {
		temp[i]=ch; i++;
		ch=fgetc(fff);
	}
	if(ch==EOF) return NULL;
	temp[i]='\0';
	strcpy(string,temp);
	return string;
}

/* Read linked list from disk */
static void read_data_from_disk(MAIN_THINGY *main_thing) {

	char temp[100];
	char first_name[100];
	char last_name[100];
	char addy1[100];
	char addy2[100];
	char phonenum[100];
	int times_played;
	int best_score;
	char *pt_f_name,*pt_l_name,*pt_addy1,*pt_addy2,*pt_phonenum;
	DATA *datum;

	FILE *fff;

	if ( (fff=fopen(DATADIR"/user_dat.sea","r+"))==NULL) return;

	while(!feof(fff)){
		if (read_string_from_disk(fff,(char*)&temp)==NULL) return;
		strcpy(first_name,temp);
		if (read_string_from_disk(fff,(char*)&temp)==NULL) return;
		strcpy(last_name,temp);
		if (read_string_from_disk(fff,(char*)&temp)==NULL) return;
		strcpy(addy1,temp);
		if (read_string_from_disk(fff,(char*)&temp)==NULL) return;
		strcpy(addy2,temp);
		if (read_string_from_disk(fff,(char*)&temp)==NULL) return;
		strcpy(phonenum,temp);

		fscanf(fff,"%i%i",&times_played,&best_score);

		if ( (pt_f_name=malloc(strlen(first_name)+1))==NULL) {
			out_of_memory_panic();
		}
		strcpy(pt_f_name,first_name);

		if ( (pt_l_name=malloc(strlen(last_name)+1))==NULL) {
			out_of_memory_panic();
		}
		strcpy(pt_l_name,last_name);

		if ( (pt_addy1=malloc(strlen(addy1)+1))==NULL) {
			out_of_memory_panic();
		}
		strcpy(pt_addy1,addy1);

		if ( (pt_addy2=malloc(strlen(addy2)+1))==NULL) {
			out_of_memory_panic();
		}
		strcpy(pt_addy2,addy2);

		if ( (pt_phonenum=malloc(strlen(phonenum)+1))==NULL) {
			out_of_memory_panic();
		}
		strcpy(pt_phonenum,phonenum);

		if ( (datum=(DATA *)malloc(sizeof(DATA)) )==NULL) {
			out_of_memory_panic();
		}

		datum->first_name=pt_f_name;
		datum->last_name=pt_l_name;
		datum->addy1=pt_addy1;
		datum->addy2=pt_addy2;
		datum->phonenum=pt_phonenum;
		datum->times_played=times_played;
		datum->best_score=best_score;
		insert_new_data(main_thing,datum);
	}
	fclose(fff);
}

/* Write linked list to disk */
void write_data_to_disk(MAIN_THINGY *main_thing) {

	/* contemporary pop culture reference? */
	ELEMENT *fifth_elem;
	DATA *data;
	int i;
	FILE *fff;

	fff=fopen(DATADIR"/user_dat.sea","w");
	if (fff==NULL) {
		printf("error");
		return;
	}

	fifth_elem=main_thing->head;

	for(i=0;i<main_thing->num_elements;i++) {
		data=fifth_elem->data;

		fprintf(fff,"%s\n%s\n",data->first_name,data->last_name);
		fprintf(fff,"%s\n%s\n",data->addy1,data->addy2);
		fprintf(fff,"%s\n",data->phonenum);
		fprintf(fff,"%d\n%d ",data->times_played,data->best_score);
		fifth_elem=fifth_elem->next;
	}
	fclose(fff);
}

/* Enter linked list info */
DATA *enter_info(MAIN_THINGY *main_thing) {

	int ch=0,name_found=0,i;
	char text[100];
	char first_name[100],last_name[100],addy1[100],addy2[100],phonenum[100];
	int times_played,best_score;
	char temp[100];
	char *pt_f_name,*pt_l_name,*pt_addy1,*pt_addy2,*pt_phonenum;
	DATA *datum;
	ELEMENT *fifth_elem;

	/* Load old data */
	read_data_from_disk(main_thing);

	/* Loop through */
	do {
		name_found=0;
		clear();
		echo();
		set_color(C_WHITE,C_BOLD);
		printxy(2,1,"Please Enter Some Information "
				"(Linked List part of Project)");

		printxy(2,3,"First Name: ");
		set_color(C_WHITE,C_NORMAL);
		wgetnstr(stdscr,(char *)&temp,25);
		strcpy(first_name,temp);

		set_color(C_WHITE,C_BOLD);
		printxy(2,4,"Last Name: ");
		set_color(C_WHITE,C_NORMAL);
		wgetnstr(stdscr,(char *)&temp,50);
		strcpy(last_name,temp);

		fifth_elem=main_thing->head;

		for(i=0;i<main_thing->num_elements;i++) {
			if ((my_string_comp(fifth_elem->data->last_name,
				last_name)==0) &&
				(my_string_comp(fifth_elem->data->first_name,
				first_name)==0)) {

				datum=fifth_elem->data;
				name_found=1;
			}
			fifth_elem=fifth_elem->next;
		}
		set_color(C_WHITE,C_BOLD);

		if (!name_found) {
			printxy(2,6,"New Player! Please enter the following"
				" info (or leave blank)");

			printxy(2,8,"Street Address: ");
			set_color(C_WHITE,C_NORMAL);
			wgetnstr(stdscr,(char *)&temp,50);
			strcpy(addy1,temp);

			set_color(C_WHITE,C_BOLD);
			printxy(2,9,"City/State/Zip: ");
			set_color(C_WHITE,C_NORMAL);
			wgetnstr(stdscr,(char *)&temp,50);
			strcpy(addy2,temp);

			set_color(C_WHITE,C_BOLD);
			printxy(2,10,"Phone Number: ");
			set_color(C_WHITE,C_NORMAL);
			wgetnstr(stdscr,(char *)&temp,50);
			strcpy(phonenum,temp);

			times_played=0;
			best_score=65;

			if ( (pt_f_name=malloc(strlen(first_name)+1))==NULL) {
				out_of_memory_panic();
			}
			strcpy(pt_f_name,first_name);

			if ( (pt_l_name=malloc(strlen(last_name)+1))==NULL) {
				out_of_memory_panic();
			}
			strcpy(pt_l_name,last_name);

			if( (pt_addy1=malloc(strlen(addy1)+1))==NULL) {
				out_of_memory_panic();
			}
			strcpy(pt_addy1,addy1);

			if ( (pt_addy2=malloc(strlen(addy2)+1))==NULL) {
				out_of_memory_panic();
			}
			strcpy(pt_addy2,addy2);

			if ( (pt_phonenum=malloc(strlen(phonenum)+1))==NULL) {
				out_of_memory_panic();
			}
			strcpy(pt_phonenum,phonenum);

			if ( (datum=(DATA *)malloc(sizeof(DATA)) )==NULL) {
				out_of_memory_panic();
			}

			datum->first_name=pt_f_name;
			datum->last_name=pt_l_name;
			datum->addy1=pt_addy1;
			datum->addy2=pt_addy2;
			datum->phonenum=pt_phonenum;
			datum->times_played=times_played;
			datum->best_score=best_score;
		}
		else {
			/* Found previous entry */
			printxy(2,6,"Previous Entry.  "
				"If the below is not correct, "
				"Please use a different Name");
			sprintf(text,"%s %s",datum->first_name,
						datum->last_name);
			printxy(2,8,text);
			sprintf(text,"%s",datum->addy1);
			printxy(2,9,text);
			sprintf(text,"%s",datum->addy2);
			printxy(2,10,text);
			sprintf(text,"%s",datum->phonenum);
			printxy(2,11,text);
			sprintf(text,"Times Played: %i  Best Score: %i",
				datum->times_played,datum->best_score);
			printxy(2,13,text);
		}

		set_color(C_WHITE,C_BOLD);
		printxy(2,15,"Is the above correct?");
		set_color(C_WHITE,C_NORMAL);
		ch=mvgetch(15,24);
	} while (toupper(ch)!='Y');

	noecho();

	if (!name_found) {
		return insert_new_data(main_thing,datum);
	}

	return datum;
}


/* High Score Routine.  Top 10 Scores */
void do_high_score(char *name, int turns) {

	/* FIXME: make this more robust */

	FILE *fff;
	char names[10][100],text[1024];
	int scores[10],i;

	/* If not there, create one */
	fff=fopen(DATADIR"/hiscore.sea","r+");
	if (fff==NULL) {
		fff=fopen(DATADIR"/hiscore.sea","w+");
		if (fff!=NULL) {
			/* These were all friends, guinea pigs, or */
			/* inside jokes from 1997 */
			fprintf(fff,"Vince\n50\nMarie\n55\nJohn\n56\n");
			fprintf(fff,"Gus\n57\nLizann\n58\nKevin\n59\n");
			fprintf(fff,"Hal\n60\nHairold\n61\nChipper\n62\nBob\n63\n");
			fclose(fff);
		}
	}
	else {
		fclose(fff);
	}

	/* read high score from disk */
	fff=fopen(DATADIR"/hiscore.sea","r");
	if (fff!=NULL) {
		for(i=0;i<10;i++) {
			fscanf(fff,"%9s",names[i]);
			fscanf(fff,"%d",&scores[i]);
		}
		fclose(fff);
	}

	clear();
	refresh();

	/* If score should be on list, add it */
	if (turns<scores[9]) {
		i=8;

		while( (i>=0) && (turns<=scores[i])) {
			scores[i+1]=scores[i];
			strncpy(names[i+1],names[i],10);
			i--;
		}

		i++;

		scores[i]=turns;
		strncpy(names[i],name,10);

		set_color(C_WHITE,C_BOLD);
		snprintf(text,100,"%s got a new High Score, #%i",name,i+1);
		printxy(20,1,text);

		fff=fopen(DATADIR"/hiscore.sea","w+");
		if (fff!=NULL) {
			for(i=0;i<10;i++) {
				fprintf(fff,"%s\n%i\n",names[i],scores[i]);
			}
			fclose(fff);
		}
	}

	set_color(C_GREEN,C_BOLD);
	printxy(34,4,"High Scores");
	set_color(C_RED,C_BOLD);
	for(i=0;i<10;i++){
		snprintf(text,1024,"%2i.  %i  %s",i+1,scores[i],names[i]);
		printxy(20,i+6,text);
	}

	set_color(C_WHITE,C_NORMAL);
	printxy(5,20,"Press any key to Continue");
	getch();
}

/*  ------->   END OF bdb.c by Vince Weaver <----------- */
