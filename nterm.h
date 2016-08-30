/**
* @file   nterm.h
* @author sunway <sunwayforever@gmail.com>
* @date   Sat Mar  1 15:05:31 2008
* 
* @brief  
* 
* 
*/
#ifndef _NTERM_H
#define _NTERM_H 1

#include <ncurses.h>
#include <cdk.h>
#include <libconfig.h>
#include <stdarg.h>
#include <panel.h>
#include <sys/types.h>	/* basic system data types */
#include <sys/socket.h>	/* basic socket definitions */
#include <sys/time.h>	/* timevalfor select() */
#include <time.h>		/* timespec for pselect() */
#include <netinet/in.h>	/* sockaddr_in and other Internet defns */
#include <arpa/inet.h>	/* inet(3) functions */
#include <errno.h>
#include <assert.h>
#include <fcntl.h>		/* for nonblocking */
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>	/* for S_xxx file mode constants */
#include <sys/uio.h>		/* for iovec and readv/writev */
#include <unistd.h>
#include <sys/wait.h>
#include <sys/un.h>		/* for Unix domain sockets */
#include <sys/select.h>	/* for convenience */
#include <arpa/telnet.h>
#include <termios.h>
#include <assert.h>

/* some wrapper functions */
#define	SA struct sockaddr
int Accept(int, SA *, socklen_t *);
void Bind(int, const SA *, socklen_t);
int Connect(int, const SA *, socklen_t);
void Listen(int, int);
int Socket(int, int, int);
void Write(int,void *,size_t);
struct hostent *Gethostbyname(const char *);	

/* telnet functions */
int telnet_init(const char *);
void telnet_finit();
void telnet_input(int num,...);
int telnet_output(char *);

/* ncurses functions */
void ncurses_send_file();
void ncurses_recv_file ();


/* zmodem functions */
void zmodem_send_file (const char *);
void zmodem_recv_file ();

/* decode functions */
void ansi_decode(char *,int);


enum telnet_stat_t {TS_LOGIN=0,TS_SAVE_START=1,TS_SAVE_ING=2,TS_NONE=3,TS_NEGO=4};
enum telnet_stat_t telnet_stat;

enum {SHOW,SAVE,MAIL} article;


#define BUFSIZE 1000
#define FOREVER while (1)
int sockfd;

typedef struct {
    char * host;
    int auto_login;
    char * user;
    char * passwd;
    char  key_quit;
    char  key_save;
    char  key_show;
    char  key_refresh;
    char  key_mail;
} config;

config conf;

#endif
