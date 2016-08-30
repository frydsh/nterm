/**
    Copyright (C) 2008  <sunway>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/**
 * @file   decode.c
 * @author sunway <sunwayforever@gmail.com>
 * @date   Wed Mar  5 21:33:27 2008
 * 
 * @brief  
 * \todo ÅÐ¶ÏÎÄÕÂ½áÊøµÄ·½·¨
 * 
 */

//Alert: the screen offset of ncurses is 0, while ansi escape code take it as
//1...so the initial window size of the telnet negotiation should be COLS-2 and
//LINES-1, and [2,3H should be translated to move(1,2) instead of move(2,3)

#include "nterm.h"
#include "decode.h"

void ansi_decode (char *buff, int size) {
/*{{{  */

    curr_size=0;
    buf_size=size;
    buf=buff;
    while (curr_size<buf_size) {
	char curr=buf[curr_size];
	switch (sta) {
	case STA_NONE:
	    if (curr == '\033') { /* ESC */
		sta=STA_CSI;
		curr_size++;	/* must be [, so just ignore it */
	    } else if (curr == '\030') { /* zmodem */
		if (buf[curr_size+3]==0x30) {
		    ncurses_recv_file ();		    
		} else if (buf[curr_size+3]==0x31) {
		    ncurses_send_file ();
		} else {
		    /* maybe canit from server */
		    /* while (buf[curr_size]=='\030' || buf[curr_size]=='\b') */
		    /* 	curr_size++; */
		}

	    } else {
		show ();
		refresh ();
	    }
	    break;
	case STA_CSI:
	    if (curr>='\060' && curr<='\071') { /* digit */
		num1=get_num ();		
		sgr_num[sgr_curr_size++]=num1;
		sta=STA_NUM1;
	    } else if (curr==';') {
		sta=STA_COLON;
	    } else {
		check_action (curr);		
	    }
	    break;
	case STA_NUM1:
	    if (curr==';') {
		sta=STA_COLON;
	    } else {
		check_action (curr);
	    }
	    break;
	case STA_COLON:
	    if (curr>='\060' && curr<='\071') {
		num2=get_num (); 
		sgr_num[sgr_curr_size++]=num2;
		sta=STA_NUM2;
	    } else {
		check_action (curr);
	    }
	    break;
	case STA_NUM2:
	    if (curr==';') {
		sta=STA_COLON2;
	    } else {
		check_action (curr);		
	    }
	    break;
	case STA_COLON2:
	    if (curr>='\060' && curr<='\071') {
		num3=get_num ();
		sgr_num[sgr_curr_size++]=num3;
		sta=STA_NUM2;
	    } else {
		check_action (curr);
	    }
	    break;
	default:
	    ;
	}
	curr_size++;
    }

    if (telnet_stat==TS_LOGIN && conf.auto_login==1) {
	/* send username */
	if (buf[buf_size-1]==0x20 && buf[buf_size-2]==0x3a) { /* : */
	    char * tmp=malloc (40);
	    bzero (tmp,40);
	    strcpy (tmp,conf.user);
	    strcat (tmp,"\r");
	    strcat (tmp,conf.passwd);
	    strcat (tmp,"\r");
	    write (sockfd,tmp,40);
	    telnet_stat=TS_NONE;
	    free (tmp);
	}
    }
    return;
}
/*}}}*/

void check_action (char curr) {
/*{{{  */
    if (curr=='A') {
	int n=0;
	if (num1==-1) {
	    n=1;
	} else {
	    n=num1;
	}
	int y=0,x=0;
	getyx (stdscr,y,x);
	move (y-n,x);
    } else if (curr=='B') {
	int n=0;
	if (num1==-1) {
	    n=1;
	} else {
	    n=num1;
	}
	int y=0,x=0;
	getyx (stdscr,y,x);
	move (y+n,x);
    } else if (curr=='C') {
	int n=0;
	if (num1==-1) {
	    n=1;
	} else {
	    n=num1;
	}
	int y=0,x=0;
	getyx (stdscr,y,x);
	move (y,x+n);	
    } else if (curr=='D') {
	int n=0;
	if (num1==-1) {
	    n=1;
	} else {
	    n=num1;
	}
	int y=0,x=0;
	getyx (stdscr,y,x);
	move (y,x-n); 		
    } else if (curr=='E') {
		    
    } else if (curr=='F') {
		    
    } else if (curr=='G') {
		    
    } else if (curr=='H') {
	move (num1==-1?0:num1-1,num2==-1?0:num2-1); 
    } else if (curr=='J') {
	clear ();
    } else if (curr=='K') {
	clrtoeol ();
    } else if (curr=='S') {
		    
    } else if (curr=='T') {
		    
    } else if (curr=='f') {
		    
    } else if (curr=='m') {
	check_sgr (sgr_num);
		    
    } else if (curr=='n') {
		    
    } else if (curr=='s') {
		    
    } else if (curr=='u') {
	
    }
    num1=-1;
    num2=-1;
    sta=STA_NONE;
    sgr_curr_size=0;
    refresh ();
}
/*}}}*/
    
void check_sgr (int num[]) {
/*{{{  */
    /* todo more sgr options */
    static int last_frgrd=7;	/* white as default */
    static int last_bkgrd=0;	/* black as default */
    
    int i=0;
    if (sgr_curr_size==0) {
	standend ();
	last_bkgrd=0;
	last_frgrd=7;
	return;
    } 
    for (i=0;i<sgr_curr_size;i++) {
	int curr_num=num[i];
	if (curr_num==1) {
	    attron (A_BOLD);
	} else if (curr_num>=30 && curr_num<38) { /* forground */
	    attron (COLOR_PAIR ((curr_num-30)+last_bkgrd*10));
	    last_frgrd=curr_num-30;
	} else if (curr_num>=40 && curr_num<48) { /* background */
	    attron (COLOR_PAIR ((curr_num-40)*10+last_frgrd));
	    last_bkgrd=curr_num-40;
	} else if (curr_num==0) {
	    standend ();
	    last_bkgrd=0;
	    last_frgrd=7;
	}
    }
}
/*}}}*/

int get_num () {
/*{{{  */
    char tmp[10]={'\0'};
    int j=0;
    char curr=buf[curr_size];
    while (curr>='\060' && curr<='\071') {
	tmp[j]=curr;
	curr=buf[++curr_size];
	j++;
    }
    curr_size--;
    return (atoi (tmp));
}
/*}}}*/

void show () {
/*{{{  */
    if (telnet_stat==TS_SAVE_START) {
	/* todo: display a window to get the filename to save to */
	telnet_stat=TS_SAVE_ING;
	bzero (file_buf,FILE_SIZE);
	file_curr_size=0;
	
    } else if (
	telnet_stat==TS_SAVE_ING &&
	(
	    (buf[curr_size]=='[' &&
	     buf[curr_size+1]=='\324' &&
 	     buf[curr_size+2]=='\304' &&
	     buf[curr_size+3]=='\266' &&
	     buf[curr_size+4]=='\301' &&
	     buf[curr_size+5]=='\316' &&
	     buf[curr_size+6]=='\304' &&
	     buf[curr_size+7]=='\325' &&
	     buf[curr_size+8]=='\302' &&
	     buf[curr_size+9]==']')
	    ||
	    (buf[curr_size]=='\312' &&
	     buf[curr_size+1]=='\261' &&
	     buf[curr_size+2]=='\274' &&
	     buf[curr_size+3]=='\344' &&
	     buf[curr_size+4]=='[')
	    ||
	    (buf[curr_size]=='\326' &&
	     buf[curr_size+1]=='\367' &&
	     buf[curr_size+2]=='\314' &&
	     buf[curr_size+3]=='\342' &&
	     buf[curr_size+4]=='\324' &&
	     buf[curr_size+5]=='\304' &&
	     buf[curr_size+6]=='\266' &&
	     buf[curr_size+7]=='\301')

	    )) {
	    telnet_stat=TS_NONE; /* "ÔÄ¶ÁÎÄÕÂ",which means end of an article has
				  * been reached ,or Ê±¼ä[ or Ö÷ÌâÔÄ¶Á*/
	    int fd=open ("/tmp/nterm_save",O_CREAT|O_RDWR|O_TRUNC,0755);
	    write (fd,file_buf,file_curr_size);
	    close (fd);
	    if (article == SAVE ) {
		system ("vim /tmp/nterm_save");
	    } else if (article==SHOW) {
		system ("urlview /tmp/nterm_save");
	    } else if (article==MAIL) {
		system ("mutt -a /tmp/nterm_save");
	    }
	    clear ();
	    telnet_input (1,'q');
	}
	char curr_char=buf[curr_size];
	if (curr_char==0) {
	    return;
	} else if (curr_char=='\015') { //\r
	    if (buf[curr_size+1]=='~' && buf[curr_size+2]=='\033' && buf[curr_size+3]=='[') {
		system ("vim /tmp/nterm_article");
		curr_size=BUFSIZE;	/* flush buf */
		int fd=open ("/tmp/nterm_article",O_RDONLY);
		if (fd>0) {
		    int n=0;
		    char *tmp=malloc (BUFSIZE);
		    while (n=read (fd,tmp,BUFSIZE)) {
			write (sockfd,tmp,n);
			bzero (tmp,BUFSIZE);
		    }
		    free (tmp);
		    telnet_input (3,0x18,0x6c,0xa); /* C-x l ret */
		    unlink ("/tmp/nterm_article");
		} else {
		    telnet_input (3,0x18,0x61,0xa); /* C-x a ret */
		}
		clear ();
	    } else {
		int y=0,x=0;
		getyx (stdscr,y,x);
		move (y,0);
	    }
	} else if (curr_char=='\012') { //\n
	    if (telnet_stat==TS_SAVE_ING && file_curr_size<FILE_SIZE+1) {
		file_buf[file_curr_size++]=curr_char;
	    } else {
		int y=0,x=0;
		getyx (stdscr,y,x);
		move (y+1,x);
	    }
	} else if (curr_char>0) {
	    if (telnet_stat==TS_SAVE_ING && file_curr_size<FILE_SIZE+1) {
		file_buf[file_curr_size++]=curr_char;
	    } else {
		addch (curr_char);
	    }
	} else {			/* double bytes chars */
	    if (telnet_stat==TS_SAVE_ING && file_curr_size<FILE_SIZE+1) {
		file_buf[file_curr_size++]=curr_char;
		file_buf[file_curr_size++]=buf[++curr_size];
	    } else {
		char tmp[3];
		tmp[0]=curr_char;
		tmp[1]=buf[++curr_size];
		tmp[2]='\0';
		addstr (tmp);
	    }
	}
	}
/*}}}*/
