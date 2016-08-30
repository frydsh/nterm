/**
* @file   decode.h
* @author sunway <sunwayforever@gmail.com>
* @date   Mon Mar 10 21:33:02 2008
* 
* @brief  
* 
* 
*/
#ifndef _DECODE_H
#define _DECODE_H 1

static int get_num ();
static void check_action (char);
static void check_sgr (int []);
static void show ();

enum _state {STA_NONE,STA_CSI,STA_NUM1,STA_NUM2,STA_COLON,STA_COLON2};
static enum _state  sta=STA_NONE;
int num1=-1;
int num2=-1;
int num3=-1;
int sgr_num[10]={-1};
int sgr_curr_size=0;
int curr_size=0;
int buf_size=0;
char * buf;
#define FILE_SIZE 3000
static char file_buf[FILE_SIZE];
static int file_curr_size;


#endif
