/*---------------------------------------------------------------------------
    GA Schedule

    Copyright (C)  2024 Tom Stevelt

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
---------------------------------------------------------------------------*/

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include	<errno.h>
/*
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<dirent.h>
#include	<stdint.h>
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
TYPE	int		Verbose;

#define		MAXNAME			30
#define		MAXCOURSE		100
#define		MAXCLASS		300
#define		MAXPERIODS		7
#define		MAXPERCLASS		20

typedef struct
{
	int		ID;
	int		Level;
	int		Required;
	char	Name[MAXNAME];
	int		Counter;
} COURSE_RECORD;

TYPE	COURSE_RECORD	CourseArray[MAXCOURSE];
TYPE	int				CourseCount;

typedef struct
{
	int		ID;
	int		Level;
	char	Name[MAXNAME];
} STUDENT_RECORD;

TYPE	STUDENT_RECORD	Student;
TYPE	int				StudentCount;

typedef struct
{
	int		ID;
	int		CourseID;
	char	ClassCode[10];
} CLASS_RECORD;

TYPE	CLASS_RECORD	ClassArray[MAXCLASS];
TYPE	int				ClassCount;

/*----------------------------------------------------------
	Genetic Algorithm defines and variables
----------------------------------------------------------*/

TYPE	int		PopCount;
TYPE	int		MaxGenerations;
TYPE	double	ProbCross;
TYPE	double	ProbMutate;

typedef struct
{
	int	period;
	int	classIndex;
} ALLELE;

typedef struct
{
	ALLELE	Chromosome[MAXCLASS];
	int		Conflicts;		// student is in more than one class at a time
	int		Missing;		// student's class is not represented.
	int		Fitness;		// total of Conflicts + Missing
} INDIVIUDAL;

TYPE	INDIVIUDAL		*CurrPop;
TYPE	INDIVIUDAL		*NextPop;

#define		REPORT_ALL		1
#define		REPORT_BEST		2

/*----------------------------
:.,$d
:r ! mkproto -p *.c
----------------------------*/


/* LoadClasses.c */
int cmpclass ( CLASS_RECORD *a , CLASS_RECORD *b );
void LoadClasses ( void );
void DumpClasses ( void );

/* LoadCourses.c */
int cmpcourse ( COURSE_RECORD *a , COURSE_RECORD *b );
void LoadCourses ( void );
void DumpCourses ( void );

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

/* init.c */
void init ( void );

/* report.c */
void report ( int generation , int mode );
