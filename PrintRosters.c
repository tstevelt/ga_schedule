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

#include	"ga_schedule.h"

typedef struct
{
	int		StudentID;
	char	ClassCode[10];
	int		Period;

	int		Level;
	char	StudentName[MAXNAME];
	char	ClassName[MAXNAME];
} RECORD;

static	RECORD	*Array;
static	int		Count;

static	int		CompareBy;

static int cmprec ( RECORD *a, RECORD *b )
{
	int		rv;

	if ( CompareBy == 1 )
	{
		if ( a->Level < b->Level )
		{
			return ( -1 );
		}
		if ( a->Level > b->Level )
		{
			return ( 1 );
		}
		rv = strcmp ( a->StudentName, b->StudentName );
		if ( rv < 0 )
		{
			return ( -1 );
		}
		if ( rv > 0 )
		{
			return ( 1 );
		}
		if ( a->Period < b->Period )
		{
			return ( -1 );
		}
		if ( a->Period > b->Period )
		{
			return ( 1 );
		}
		/*----------------------------------------------------------
			un-assigned students will get here, periods will be 0
		----------------------------------------------------------*/
		rv = strcmp ( a->ClassCode, b->ClassCode );
	}
	else if ( CompareBy == 2 )
	{
		rv = strcmp ( a->ClassCode, b->ClassCode );
		if ( rv < 0 )
		{
			return ( -1 );
		}
		if ( rv > 0 )
		{
			return ( 1 );
		}
		rv = strcmp ( a->StudentName, b->StudentName );
	}
	else
	{
		printf ( "Unknown value for CompareBy %d\n", CompareBy );
		exit ( 1 );
	}

	return ( rv );
}

static int LookUpPeriod ( char ClassCode[] )
{
	int		rv = 0;

	for ( int ndx = 0; ndx < Count; ndx++ )
	{
		if ( strcmp ( ClassCode, Array[ndx].ClassCode ) == 0 )
		{
			rv = Array[ndx].Period;
			break;
		}
	}
	return ( rv );
}

void PrintRosters ()
{
	STUDENT_RECORD	*ptrStudent, keyStudent;
	CLASS_RECORD	*ptrClass, keyClass;
	COURSE_RECORD	*ptrCourse, keyCourse;
	FILE			*ifp, *ofp;
	int				StudentsWithConflicts = 0;

	if ( StudentCount == 0 )
	{
		LoadStudents ();
	}

	if ( ClassCount == 0 )
	{
		LoadClasses ( 2 );
	}

	Count = StudentCount * MAXPERIODS;
	if (( Array = calloc ( Count, sizeof(RECORD))) == NULL )
	{
		printf ( "calloc failed\n" );
		exit ( 1 );
	}
	Count = 0;

	if (( ifp = fopen ( "assign.CSV", "r" )) == NULL )
	{
		printf ( "Cannot open assign.CSV for input\n" );
		exit ( 1 );
	}

	/*----------------------------------------------------------
	STUDENT,CLASS,PERIOD
	   2,101E,  7
	   2,102D,  5
	   2,103A,  6
	   2,104D,  3
	   2,113A,  2
	   2,501F,  4
	   2,502A,  1
	   4,101A,  6
	   4,102A,  4
	----------------------------------------------------------*/
	while ( fgets ( buffer, sizeof(buffer), ifp ) != NULL )
	{
		if ( buffer[0] == 0 )
		{
			continue;
		}
		if (( tokcnt = GetTokensD ( buffer, ",\n\r", tokens, MAXTOKS )) < 3 )
		{
			continue;
		}

		if (( Array[Count].StudentID =  atoi(tokens[0])) == 0 )
		{
			continue;
		}
		sprintf ( Array[Count].ClassCode, "%s", tokens[1] );
		Array[Count].Period = atoi(tokens[2]);

		keyStudent.StudentID = Array[Count].StudentID;
		ptrStudent = bsearch ( &keyStudent, StudentArray, StudentCount, sizeof(STUDENT_RECORD), (int(*)()) cmpstudent );
		Array[Count].Level = ptrStudent->Level;
		sprintf ( Array[Count].StudentName, "%s", ptrStudent->Name );

		sprintf ( keyClass.ClassCode, "%s", Array[Count].ClassCode );
		if (( ptrClass = bsearch ( &keyClass, ClassArray, ClassCount, sizeof(CLASS_RECORD), (int(*)()) cmpclass )) == NULL )
		{	
			keyCourse.CourseID = atoi ( keyClass.ClassCode );
			ptrCourse = bsearch ( &keyCourse, CourseArray, CourseCount, sizeof(COURSE_RECORD), (int(*)()) cmpcourse );
		}
		else
		{
			ptrCourse = &CourseArray[ptrClass->CourseIndex];
		}
		sprintf ( Array[Count].ClassName, "%s", ptrCourse->Name );

		Count++;
	}

	fclose ( ifp );

	/*---------------------------------------------------------------------------
		print class rosters
		store student counts in Class.Period since that is not used .
	---------------------------------------------------------------------------*/
	CompareBy = 2;
	qsort ( Array, Count, sizeof(RECORD), (int(*)()) cmprec );

	if (( ofp = fopen ( "classes_detail.TXT", "w" )) == NULL )
	{
		printf ( "Cannot create classes_detail.TXT\n" );
		exit ( 1 );
	}

	char	CurrentClassCode[10];
	sprintf ( CurrentClassCode, "zzzz" );
	for ( int xa = 0; xa < Count; xa++ )
	{
		if ( strcmp ( CurrentClassCode, Array[xa].ClassCode ) != 0 )
		{
			fprintf ( ofp, "\n\n%s %-20.20s (%d)\n", Array[xa].ClassCode, Array[xa].ClassName, Array[xa].Period );
			sprintf ( CurrentClassCode, "%s", Array[xa].ClassCode );

			sprintf ( keyClass.ClassCode, "%s", Array[xa].ClassCode );
			ptrClass = bsearch ( &keyClass, ClassArray, ClassCount, sizeof(CLASS_RECORD), (int(*)()) cmpclass );
		}

		fprintf ( ofp, "  %s  (%d)\n", Array[xa].StudentName, Array[xa].StudentID );
		if ( ptrClass != NULL )
		{
			ptrClass->Period++;
		}
	}

	fprintf ( ofp, "\n\n" );
	fclose ( ofp );

	/*---------------------------------------------------------------------------
		print student schedules.  
	---------------------------------------------------------------------------*/
	CompareBy = 1;
	qsort ( Array, Count, sizeof(RECORD), (int(*)()) cmprec );

	if (( ofp = fopen ( "schedule_students.TXT", "w" )) == NULL )
	{
		printf ( "Cannot create schedule_students.TXT\n" );
		exit ( 1 );
	}

	int		CurrentID = 0;
	for ( int xa = 0; xa < Count; xa++ )
	{
		if ( CurrentID != Array[xa].StudentID )
		{
			fprintf ( ofp, "\n\n%s  (%d)\n", Array[xa].StudentName, Array[xa].StudentID );
			CurrentID = Array[xa].StudentID;
			if ( Array[xa].Period == 0 )
			{
				StudentsWithConflicts++;
			}
		}

		if ( Array[xa].Period ==  0 )
		{
			fprintf ( ofp, "  %2d %-4.4s %-20.20s", 
				Array[xa].Period, Array[xa].ClassCode, Array[xa].ClassName );

			/*----------------------------------------------------------
				print periods for classes for this course where count < 0;
			----------------------------------------------------------*/
			for ( int xc = 0; xc < ClassCount; xc++ )
			{
				if ( strncmp ( ClassArray[xc].ClassCode, Array[xa].ClassCode, 3 ) != 0 )
				{
					continue;
				}
				if ( ClassArray[xc].Period >= MAXPERCLASS )
				{
					continue;
				}

				int Period = LookUpPeriod ( ClassArray[xc].ClassCode );
				fprintf ( ofp, " %2d", Period );
			}

			fprintf ( ofp, "\n" );
		}
		else
		{
			fprintf ( ofp, "  %2d %s %s\n", 
				Array[xa].Period, Array[xa].ClassCode, Array[xa].ClassName );
		}
	}

	if ( StudentsWithConflicts )
	{
		fprintf ( ofp, "\n\n%d students with conflicts\n", StudentsWithConflicts );
	}

	fprintf ( ofp, "\n\n" );
	fclose ( ofp );

}
