
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
/*
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<dirent.h>
#include	<stdint.h>
#include	<errno.h>
#include	<libgen.h>		// for basename()
*/
#include	"shslib.h"

#ifdef MAIN
#define		TYPE		/* */
#else
#define		TYPE		extern
#endif

#define		MODE_REQUESTS	11
#define		MODE_CLASSES	12
#define		MODE_SCHEDULE	13
TYPE	int		RunMode;

/*----------------------------
:.,$d
:r ! mkproto -p *.c
----------------------------*/

/* MakeClasses.c */
void MakeClasses ( void );

/* MakeRequests.c */
void MakeRequests ( void );

/* MakeSchedule.c */
void MakeSchedule ( void );

/* ga_schedule.c */
int main ( int argc , char *argv []);

/* getargs.c */
void getargs ( int argc , char *argv []);
