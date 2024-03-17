//     GA Schedule
// 
//     Copyright (C)  2024 Tom Stevelt
// 
//     This program is free software: you can redistribute it and/or modify
//     it under the terms of the GNU Affero General Public License as
//     published by the Free Software Foundation, either version 3 of the
//     License, or (at your option) any later version.
// 
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU Affero General Public License for more details.
// 
//     You should have received a copy of the GNU Affero General Public License
//     along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
