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
 * @file   telnet.c
 * @author sunway <sunwayforever@gmail.com>
 * @date   Wed Feb 27 20:57:24 2008
 *
 * @brief
 *
 *
 */
#include "nterm.h"
#include "telnet.h"

int telnet_init(const char *host) {
/*{{{  */
    sockfd=telnet_connect(host);
    telnet_stat=TS_NEGO;
    return 0;
}
/*}}}*/

void telnet_finit() {
/*{{{  */
    close (sockfd);
}
/*}}}*/

int telnet_connect(const char * host) {
/*{{{  */

    struct hostent * hptr=Gethostbyname(host);
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = Socket (AF_INET, SOCK_STREAM, 0);
    bzero (&servaddr, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons (23);
    memcpy(&servaddr.sin_addr,*(hptr->h_addr_list),sizeof(struct in_addr));
    Connect (sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
    return sockfd;
}
/*}}}*/

int  telnet_negotiate(char * buf) {
/*{{{  */
    to_buf=malloc(BUFSIZE);
    bzero(to_buf,BUFSIZE);
    from_buf=malloc(BUFSIZE);
    bzero(from_buf,BUFSIZE);
    to_size=0;
    from_size=0;
    telnet_putc(DO,TELOPT_SGA);
    telnet_putc(WILL,TELOPT_TTYPE);
    telnet_putc(WILL,TELOPT_NAWS);
    telnet_putc(WILL,TELOPT_LINEMODE);
    telnet_putc(WILL,TELOPT_BINARY);
    telnet_send_option();
    int n=0;
    int done=0;
    while (n=read(sockfd,from_buf,BUFSIZE)) {
        while ((from_size)<n) {
            if (from_buf[from_size++]!=IAC) {
                bzero(buf,BUFSIZE);
                memcpy(buf,from_buf+from_size-1,n-from_size+1);
                done=1;
                break;
            }

            switch (from_buf[from_size++]) {
            case SB:

                if (from_buf[from_size++]==TELOPT_TTYPE) {
                    from_size++;
                    telnet_putc_ttype();
                }

                break;

            case SE:
                break;
            case WILL:
                telnet_putc(DO,from_buf[from_size++]);
                break;
            case DO:

                if (from_buf[from_size++]==TELOPT_NAWS) {
                    telnet_putc_naws();
                }

                break;

            case DONT:
                from_size++;
                break;
            case WONT:
                from_size++;
                break;

            default:
                break;
            }
        }
        if (to_size!=0) {
            telnet_send_option();
        }
        from_size=0;
        bzero(from_buf,BUFSIZE);
        if (done==1)
            return n-from_size;
    }
    free (to_buf);
    free (from_buf);

}
/*}}}*/

void telnet_putc(char b,char c) {
/*{{{  */
    to_buf[to_size++]=IAC;
    to_buf[to_size++]=b;
    to_buf[to_size++]=c;
}
/*}}}*/

void telnet_putc_ttype() {
/*{{{  */
    to_buf[to_size++]=IAC;
    to_buf[to_size++]=SB;
    to_buf[to_size++]=TELOPT_TTYPE;
    to_buf[to_size++]=0;
    to_buf[to_size++]='x';
    to_buf[to_size++]='t';
    to_buf[to_size++]='e';
    to_buf[to_size++]='r';
    to_buf[to_size++]='m';
    to_buf[to_size++]=IAC;
    to_buf[to_size++]=SE;

}
/*}}}*/

void telnet_putc_naws() {
/*{{{  */
    to_buf[to_size++]=IAC;
    to_buf[to_size++]=SB;
    to_buf[to_size++]=TELOPT_NAWS;
    to_buf[to_size++]=0;
    to_buf[to_size++]=COLS-2; 	/* todo:why */
    to_buf[to_size++]=0;
    to_buf[to_size++]=LINES-1;
    to_buf[to_size++]=IAC;
    to_buf[to_size++]=SE;

}
/*}}}*/

void telnet_send_option() {
/*{{{  */
    write(sockfd, to_buf,to_size);
    bzero(to_buf,to_size);
    to_size=0;
}
/*}}}*/

int telnet_output(char * buf) {
/*{{{  */
    bzero(buf,BUFSIZE);

    if (telnet_stat==TS_NEGO) {
        telnet_stat=TS_LOGIN;
        return (telnet_negotiate(buf));
    }

    fd_set rset;

    FD_ZERO(&rset);
    FD_SET(STDIN_FILENO,&rset);
    FD_SET(sockfd,&rset);
    Select(4,&rset,NULL,NULL,NULL);

    if (FD_ISSET(STDIN_FILENO,&rset)) {
        return 0;
    } else if (FD_ISSET(sockfd,&rset)) {
        int n=0;
        bzero(from_buf,BUFSIZE);
        n=read(sockfd,from_buf,BUFSIZE);
        memcpy(buf,from_buf,n);
        return n;
    }
}
/*}}}*/

void telnet_input(int num,...) {
/*{{{  */
    va_list ap;
    va_start (ap,num);
    char c;
    for (;num>0;num--) {
	c=(char)va_arg (ap,int);
	write(sockfd,&c,1);
    } 
    va_end (ap);
}
/*}}}*/
