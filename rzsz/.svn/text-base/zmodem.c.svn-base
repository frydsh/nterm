/**
* @file   zmodem.c
* @author sunway <sunwayforever@gmail.com>
* @date   Thu Mar 13 00:48:09 2008
* 
* @brief  
* 
* 
*/

#include "../nterm.h"
#include "zglobal.h"

void zmodem_send_file (const char * filename) {
    int fd0,fd1;
    fd0=dup(STDIN_FILENO);
    fd1=dup(STDOUT_FILENO);
    dup2 (sockfd,STDOUT_FILENO);
    dup2 (sockfd,STDIN_FILENO);
    bbs_zsendfile (filename,NULL);
    dup2 (fd0,STDOUT_FILENO);
    dup2 (fd1,STDIN_FILENO);
}

void zmodem_recv_file () {
    int fd0,fd1;
    fd0=dup(STDIN_FILENO);
    fd1=dup(STDOUT_FILENO);
    dup2 (sockfd,STDOUT_FILENO);
    dup2 (sockfd,STDIN_FILENO);
    bbs_zrecvfile();
    dup2 (fd0,STDOUT_FILENO);
    dup2 (fd1,STDIN_FILENO);
}

