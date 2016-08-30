/*
  zreadline.c - line reading stuff for lrzsz
  Copyright (C) until 1998 Chuck Forsberg (OMEN Technology Inc)
  Copyright (C) 1994 Matt Porter
  Copyright (C) 1996, 1997 Uwe Ohse

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
  02111-1307, USA.

  originally written by Chuck Forsberg
*/
/* once part of lrz.c, taken out to be useful to lsz.c too */

#include "zglobal.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>
#include <setjmp.h>


/* Ward Christensen / CP/M parameters - Don't change these! */
#define TIMEOUT (-2)

static size_t readline_readnum;
static int readline_fd;
static char *readline_buffer;
int readline_left = 0;
char *readline_ptr;

jmp_buf zmodemjmp;

static void zreadline_alarm_handler(int dummy)
{
    /* doesn't need to do anything */
    longjmp(zmodemjmp, 1);
}

/*
 * This version of readline is reasonably well suited for
 * reading many characters.
 *
 * timeout is in tenths of seconds
 */
int readline_internal(unsigned int timeout)
{

    if (!no_timeout) {
        unsigned int n;

        n = timeout / 10;
        if (n < 2 && timeout != 1)
            n = 3;
        else if (n == 0)
            n = 1;
        signal(SIGALRM, zreadline_alarm_handler);
        alarm(n);
    }

    readline_ptr = readline_buffer;
    readline_left = raw_read(readline_fd, readline_ptr, readline_readnum);
    //Ææ¹ÖµÄ
    if ((readline_left==0)&&(errno!=EINTR)) exit(0);
    if (!no_timeout)
        alarm(0);
    if (readline_left > 0 && bytes_per_error) {
        static long ct = 0;
        static int mod = 1;

        ct += readline_left;
        while (ct > bytes_per_error) {
            readline_ptr[ct % bytes_per_error] ^= mod;
            ct -= bytes_per_error;
            mod++;
            if (mod == 256)
                mod = 1;
        }
    }
    if (readline_left < 1)
        return TIMEOUT;
    --readline_left;
    return (*readline_ptr++ & 0377);
}



void readline_setup(int fd, size_t readnum, size_t bufsize)
{
    readline_fd = fd;
    readline_readnum = readnum;
    readline_buffer = malloc(bufsize > readnum ? bufsize : readnum);
    if (!readline_buffer)
        zmodem_error(1, 0, "out of memory");
}

void readline_clean(void)
{
    if (readline_buffer)
        free(readline_buffer);
    readline_buffer = NULL;
}

void readline_purge(void)
{
    readline_left = 0;
    return;
}
int raw_write(int fd,const char *buf, int len)
{
    static int lastcounter = 0;
    int nowcounter;
    static int bufcounter;
#ifndef SSHBBS
    int i, retlen=0;
#endif

#ifdef ZMODEM_RATE
        nowcounter = time(0);
        if (lastcounter == nowcounter) {
            if (bufcounter >= ZMODEM_RATE) {
                sleep(1);
                nowcounter = time(0);
                bufcounter = len;
            } else
                bufcounter += len;
        } else {
            /*
             * time clocked, clear bufcounter 
             */
            bufcounter = len;
        }
        lastcounter = nowcounter;
#endif    
#ifdef SSHBBS
    return ssh_write(fd, buf, len);
#else
    for (i = 0; i < len; i++) {
        int mylen;

        if ((unsigned char) buf[i] == 0xff)
            mylen = write(fd, "\xff\xff", 2);
        else if (buf[i] == 13)
            mylen = write(fd, "\x0d\x00", 2);
        else
            mylen = write(fd, &buf[i], 1);
        if (mylen < 0)
            break;
        retlen += mylen;
    }
    return retlen;
#endif
}
void raw_ochar(char c)
{
    raw_write(0, &c, 1);
}
int raw_read(int fd, char *buf, int len)
{
#ifdef SSHBBS
    return ssh_read(fd, buf, len);
#else
    int i,j,retlen=0;
    retlen = read(fd,buf,len);
    for(i=0;i<retlen;i++) {
        if(i>0&&((unsigned char)buf[i-1]==0xff)&&((unsigned char)buf[i]==0xff)) {
            retlen--;
            for(j=i;j<retlen;j++)
                buf[j]=buf[j+1];
            continue;
        }
        if(i>0&&buf[i-1]==0x0d&&buf[i]==0x00) {
            retlen--;
            for(j=i;j<retlen;j++)
                buf[j]=buf[j+1];
            continue;
        }
    }
    return retlen;
#endif
}
