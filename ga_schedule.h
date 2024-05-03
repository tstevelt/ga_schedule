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
#include	"shslib.h"

#ifdef MAIN
#define		TYPE		/* */
#else
#define		TYPE		extern
#endif

#define		MODE_REQUESTS	11
#define		MODE_CLASSES	12
#define		MODE_SCHEDULE	13
#define		MODE_ASSIGN		14
#define		MODE_PRINT		15
TYPE	int		RunMode;
TYPE	int		Verbose;

#define		MAXNAME			30
#define		MAXCOURSE		50
#define		MAXCLASS		200
#define		MAXPERIODS		7
#define		MAXPERCLASS		20

#define		MAXTOKS			16
TYPE	char	buffer[1024];
TYPE	char	*tokens[10];
TYPE	int		tokcnt;

typedef struct
{
	int		CourseID;
	int		Level;
	int		Required;
	char	Name[MAXNAME];
	int		Teachers;
	int		Counter;
	int		RingIndex;
} COURSE_RECORD;

TYPE	COURSE_RECORD	CourseArray[MAXCOURSE];
TYPE	int				CourseCount;

typedef struct
{
	int		StudentID;
	int		Level;
	char	Name[MAXNAME];
} STUDENT_RECORD;

TYPE	STUDENT_RECORD	Student;

TYPE	STUDENT_RECORD	*StudentArray;
TYPE	int				StudentCount;

typedef struct
{
	int		ClassID;
	int		CourseIndex;
	char	ClassCode[10];
	int		Period;
} CLASS_RECORD;

TYPE	CLASS_RECORD	ClassArray[MAXCLASS];
TYPE	int				ClassCount;

typedef struct
{
	int		StudentID;
	int		Level;
	int		ClassCount;
	int		ClassIndex[MAXPERIODS];
} REQUEST_RECORD;

TYPE	REQUEST_RECORD	*RequestArray;
TYPE	int				RequestCount;

/*----------------------------------------------------------
	Genetic Algorithm defines and variables
----------------------------------------------------------*/

TYPE	int		PopCount;
TYPE	double	ProbCross;
TYPE	double	ProbMutate;
TYPE	int		MaxGenerations;
TYPE	int		MaxConsecutive;
TYPE	int		GenerationCount;
TYPE	int		TeacherStop;

typedef struct
{
	int	Period;
} ALLELE;

typedef struct
{
	ALLELE	Chromosome[MAXCLASS];
	int		Fitness;
} INDIVIDUAL;

TYPE	INDIVIDUAL		*CurrPop;
TYPE	INDIVIDUAL		*NextPop;
TYPE	INDIVIDUAL		BestIndividual;
TYPE	int				BestFitness;

#define		REPORT_ALL		1
#define		REPORT_BEST		2
#define		REPORT_MINMAX	3

/*----------------------------
:.,$d
:r ! mkproto -p *.c
----------------------------*/

/* AssignStudents.c */
void AssignStudents ( void );

/* LoadClasses.c */
int cmpclass ( CLASS_RECORD *a , CLASS_RECORD *b );
void LoadClasses ( int SortBy );
void DumpClasses ( void );

/* LoadCourses.c */
int cmpcourse ( COURSE_RECORD *a , COURSE_RECORD *b );
void LoadCourses ( void );
void DumpCourses ( void );

/* LoadRequests.c */
void LoadRequests ( void );
void DumpRequests ( void );

/* LoadStudents.c */
int cmpstudent ( STUDENT_RECORD *a , STUDENT_RECORD *b );
void LoadStudents ( void );
void DumpStudents ( void );

/* MakeClasses.c */
void MakeClasses ( void );

/* MakeRequests.c */
void MakeRequests ( void );

/* MakeSchedule.c */
void MakeSchedule ( void );

/* PrintRosters.c */
void PrintRosters ( void );

/* PrintSchedule.c */
void PrintSchedule ( void );

/* crossover.c */
int crossover ( ALLELE parent1 [], ALLELE parent2 [], ALLELE child1 [], ALLELE child2 []);

/* ga_schedule.c */
int main ( int argc , char *argv []);

/* generation.c */
void generation ( int Generation );

/* getargs.c */
void getargs ( int argc , char *argv []);

/* init.c */
void init ( void );

/* obj_func.c */
int obj_func ( ALLELE Chromosome [] );

/* report.c */
int report ( int Generation , int mode );

/* select_shuffle.c */
int select_shuffle ( void );
