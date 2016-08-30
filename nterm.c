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
 * @file   nterm.c
 * @author sunway <sunwayforever@gmail.com>
 * @date   Sat Mar  1 14:27:45 2008
 * 
 * @brief  
 * 
 * 
 */

#include "nterm.h"

static void ncurses_init();
static int ncurses_input();
static void read_config (char *);
static void ncurses_finit ();
static void init_color_pair ();

int
main(int argc,char * argv[]) {
/*{{{  */
    assert(argc==2);
    read_config (argv[1]);
    char *buf=malloc(BUFSIZE);
    ncurses_init();
    telnet_init(conf.host);
    FOREVER {
	int n=0;
	if (n=telnet_output(buf)) {
	    ansi_decode(buf,n);
	    if (telnet_stat==TS_SAVE_ING) { /* auto input SPC when saving article */
		telnet_input (1,0x20);
	    }
	} else {
	    int r=ncurses_input();
	    if (r==1) {		/* quit when C-k is inputed */
		break;
	    }
	}
    }
    ncurses_finit ();
    return 0;
}
/*}}}*/

void ncurses_init() {
/*{{{  */
    initscr();
    clear ();
    start_color ();
    init_color_pair ();
    raw();
    noecho();
}
/*}}}*/

void ncurses_finit () {
/*{{{  */
    telnet_finit ();
    endwin ();
}
/*}}}*/

int ncurses_input() {
/*{{{  */
    /* todo what if input is larger than char?
     * to get more accurate control of key strokes,
     * i should deal with some special keys by myself
     * e.g. f1 is actually 4 key strokes
     */
    char c=getch ();
    /* todo: more keys */
    if (c==conf.key_quit) {
	return 1;
    } else if (c==conf.key_save) {
	telnet_stat=TS_SAVE_START;
	article=SAVE;
	telnet_input (1,0xc);	/*C-l*/
    } else if (c==conf.key_refresh) {
	clear ();
	telnet_input (1,0xc);
    } else if (c==conf.key_show) {
	telnet_stat=TS_SAVE_START;
	telnet_input (1,0xc);	/*C-l*/
	article=SHOW;
    } else if (c==conf.key_mail) {
	telnet_stat=TS_SAVE_START;
	telnet_input (1,0xc);	/*C-l*/
	article=MAIL;
    } else {
	telnet_input(1,c);	
    }
    return 0;
}
/*}}}*/

void ncurses_send_file () {
/*{{{  */
    CDKSCREEN *cdk_screen = 0;
    CDKFSELECT *f_select= 0;
    WINDOW *curses_win	= 0;
    char *title		= "<C>Pick A File";
    char *label		= "File: ";
    char		*mesg[10];
    mesg[0] = "</29/B>wait while uploading..";

    curses_win = newwin (20,65,(LINES-20)/2,(COLS-65)/2);
    cdk_screen = initCDKScreen (curses_win);
    initCDKColor();
    f_select = newCDKFselect (cdk_screen,
			      0,
			      0,
			      19,
			      64,
			      title, label, A_NORMAL, '_', A_REVERSE,
			      "</5>", "</48>", "</N>", "</N>",
			      TRUE,
			      FALSE);
    char *filename = activateCDKFselect (f_select, 0);
    //wclear (curses_win);
    CDKLABEL * cdk_label=newCDKLabel (cdk_screen,
				      CENTER,
				      CENTER,
				      mesg, 1,
				      TRUE,
				      FALSE);
    drawCDKLabel (cdk_label,TRUE);
    zmodem_send_file (filename);
    destroyCDKScreen (cdk_screen);
    endCDK();
    wclear (curses_win);
    wrefresh (curses_win);
    delwin (curses_win);
    refresh ();
    raw();
    noecho();
    if (!filename) {
	free (filename);	
    }
}
/*}}}*/

void ncurses_recv_file () {
/*{{{  */
    CDKSCREEN *cdk_screen = 0;
    WINDOW *curses_win	= 0;
    char *mesg[10];
    mesg[0] = "</29/B>wait while downloading..";
    curses_win = newwin (20,65,(LINES-20)/2,(COLS-65)/2);
    cdk_screen = initCDKScreen (curses_win);
    initCDKColor();
    CDKLABEL * cdk_label=newCDKLabel (cdk_screen,
				      CENTER,
				      CENTER,
				      mesg, 1,
				      TRUE,
				      FALSE);
    
    drawCDKLabel (cdk_label,TRUE);
    zmodem_recv_file ();
    destroyCDKScreen (cdk_screen);
    endCDK();
    wclear (curses_win);
    wrefresh (curses_win);
    delwin (curses_win);
    refresh ();
    raw();
    noecho();
}
/*}}}*/

void read_config (char * alias) {
/*{{{  */
    conf.host=NULL;
    conf.auto_login=0;
    conf.user=NULL;
    conf.passwd=NULL;
    conf.key_quit=0xb;		/* C-k as default*/
    conf.key_save=0x6;		/* C-f as default*/
    conf.key_show=0x16;	 /* C-v as default*/
    conf.key_refresh=0xc;	/* C-l as default*/
    conf.key_mail=0x14;	/* C-t as default*/

    char * config_file=malloc(50);
    bzero (config_file,50);
    strcpy (config_file,getenv ("HOME"));
    strcat (config_file,"/.ntermrc");

    struct config_t cfg;
    config_init(&cfg);

    if (!config_read_file(&cfg, config_file)) {
	write (STDERR_FILENO,".ntermrc not found\n",17);
	conf.host=alias;
    }
    else {
	/* get host conf */
	config_setting_t *group = config_lookup(&cfg, alias);
	if (!group) {
	    conf.host=alias;
	} else {
	    config_setting_t *tmp = config_setting_get_member(group,"host");
	    if (tmp) {
	    	conf.host=(char *)config_setting_get_string(tmp);
	    }
	    tmp = config_setting_get_member (group,"auto_login");
	    if (tmp) {
	    	conf.auto_login=config_setting_get_int(tmp);
	    }
	    tmp = config_setting_get_member (group,"user");
	    if (tmp) {
	    	conf.user=(char *)config_setting_get_string(tmp);
	    }
	    tmp = config_setting_get_member (group,"passwd");
	    if (tmp) {
	    	conf.passwd=(char *)config_setting_get_string(tmp);
	    }
	}
	/* get key conf */
	/* todo:more keys */
	group = config_lookup(&cfg, "key");
	if (group) {
	    config_setting_t *tmp;
	    tmp = config_setting_get_member(group,"quit");
	    if (tmp!=0) {
	    	conf.key_quit=config_setting_get_int(tmp);
	    }
	    tmp = config_setting_get_member(group,"save");
	    if (tmp!=0) {
	    	conf.key_save=config_setting_get_int(tmp);
	    }
	    tmp = config_setting_get_member(group,"show");
	    if (tmp!=0) {
	    	conf.key_show=config_setting_get_int(tmp);
	    }
	    tmp = config_setting_get_member(group,"refresh");
	    if (tmp!=0) {
	    	conf.key_refresh=config_setting_get_int(tmp);
	    }
	    tmp = config_setting_get_member(group,"mail");
	    if (tmp!=0) {
	    	conf.key_mail=config_setting_get_int(tmp);
	    }

	}
    }
    free (config_file);
}
/*}}}*/

void init_color_pair () {
/*{{{  */
    int i=0;
    int j=0;
    int color[8]={COLOR_BLACK,COLOR_RED,COLOR_GREEN,COLOR_YELLOW,COLOR_BLUE,COLOR_MAGENTA,COLOR_CYAN,COLOR_WHITE};
    for (i=0;i<8;i++) {
	for (j=0;j<8;j++) {
	    init_pair (i*10+j,color[j],color[i]);
	}
    }
}
/*}}}*/
