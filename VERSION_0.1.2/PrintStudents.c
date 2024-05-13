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

static STUDENT_RECORD *FindStudent ( int StudentID )
{
	for ( int xs = 0; xs < StudentCount; xs++ )
	{
		if ( StudentArray[xs].StudentID == StudentID )
		{
			return ( &StudentArray[xs] );
		}
	}

	return ( NULL );
}

typedef struct
{
	int		Period;
	int		ClassIndex;
} RECORD;

static	RECORD	Array[MAXPERIODS];
static	int		Count;

static int cmprec ( RECORD *a, RECORD *b )
{
	if ( a->Period < b->Period )
	{
		return ( -1 );
	}
	if ( a->Period > b->Period )
	{
		return ( 1 );
	}
	return ( 0 );
}


void PrintStudents ()
{
	STUDENT_RECORD	*ptrStudent;
	FILE			*ofp;
	int				StudentsWithConflicts = 0;

	if ( StudentCount == 0 )
	{
		LoadStudents ();
	}

	for ( int xc = 0; xc < ClassCount; xc++ )
	{
		ClassArray[xc].Period = BestIndividual.Chromosome[xc].Period;
	}

	if (( ofp = fopen ( "schedule_students.TXT", "w" )) == NULL )
	{
		printf ( "Cannot create schedule_students.TXT\n" );
		ofp = stdout;
	}

	for ( int xr = 0; xr < RequestCount; xr++ )
	{
		ptrStudent = FindStudent ( RequestArray[xr].StudentID );
		fprintf ( ofp, "%s  (%d) %d classes\n", 
				ptrStudent->Name, RequestArray[xr].StudentID, RequestArray[xr].ClassCount );

		Count = 0;
		CLASS_RECORD	*ptrClass;
		COURSE_RECORD	*ptrCourse;
		for ( int xc = 0; xc < RequestArray[xr].ClassCount; xc++ )
		{
			int xp = Array[Count].ClassIndex = RequestArray[xr].ClassIndex[xc];
			Array[Count].Period = ClassArray[xp].Period;
			Count++;
		}
		qsort ( Array, Count, sizeof(RECORD), (int(*)()) cmprec );

		int	ConflictCount = 0;
		for ( int xp = 0; xp < Count; xp++ )
		{
			ptrClass = &ClassArray[Array[xp].ClassIndex];
			ptrCourse = &CourseArray[ptrClass->CourseIndex];
			fprintf ( ofp, "  %d: %s\n", Array[xp].Period, ptrCourse->Name );
			if ( xp > 0 && Array[xp-1].Period == Array[xp].Period )
			{
				ConflictCount++;
			}
		}
		if ( ConflictCount )
		{
			fprintf ( ofp, "  %d conflicts\n", ConflictCount );
			StudentsWithConflicts++;
		}

		fprintf ( ofp, "\n\n" );
	}

	if ( StudentsWithConflicts )
	{
		fprintf ( ofp, "%d students with conflicts\n", StudentsWithConflicts );
	}

	if ( ofp != stdout )
	{
		fclose ( ofp );
	}
}
