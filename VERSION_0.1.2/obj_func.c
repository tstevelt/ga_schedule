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

static int DebugObjFunc = 0;

typedef struct
{
	int		Period;
	int		CourseIndex;
	int		Count;
} RECORD;

static	RECORD	*Array;
static	int		Size;
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
	if ( a->CourseIndex < b->CourseIndex )
	{
		return ( -1 );
	}
	if ( a->CourseIndex > b->CourseIndex )
	{
		return ( 1 );
	}
	return ( 0 );
}

int obj_func ( ALLELE Chromosome [], int *StudentConflicts, int *TeacherConflicts )
{
static	int		firstpass = 1;
	int		Students = 0;
	int		Teachers = 0;
	int		Total = 0;
	int		PeriodArray[MAXPERIODS];
	int		xc, xp, PeriodCount;
	RECORD	*ptr, key;

	if ( firstpass == 1 )
	{
		firstpass = 0 ;
		Size = MAXPERIODS * CourseCount;
		if (( Array = calloc ( Size, sizeof(RECORD) )) == NULL )
		{
			printf ( "obj_func: calloc Array failed\n" );
			exit ( 1 );
		}	

		Count = 0;
		for ( xp = 1; xp <= MAXPERIODS; xp++ )
		{
			for ( xc = 0; xc < CourseCount; xc++ )
			{
				if ( Count >= Size )
				{
					printf ( "obj_func: exceeds Size\n" );
					exit ( 1 );
				}	
				Array[Count].Period = xp;
				Array[Count].CourseIndex = xc;
				Array[Count].Count = 0;
				Count++;
			}
		}
	}

#define CHECK_STUDENTS
#ifdef CHECK_STUDENTS
	/*------------------------------------------------------------------------------
		tally conflicts for each student, add to StudentConflicts
	------------------------------------------------------------------------------*/
	for ( int RequestIndex = 0; RequestIndex < RequestCount; RequestIndex++ )
	{
		if ( DebugObjFunc )
		{
			printf ( "---\n" );
		}
		PeriodCount = 0;
		memset ( PeriodArray, '\0', sizeof(PeriodArray) );

		for ( int RequestClassIndex = 0; RequestClassIndex < RequestArray[RequestIndex].ClassCount; RequestClassIndex++ )
		{
			for ( int ChromoIndex = 0; ChromoIndex < ClassCount; ChromoIndex++ )
			{
				if ( ChromoIndex == RequestArray[RequestIndex].ClassIndex[RequestClassIndex] )
				{
					for ( xp = 0; xp < PeriodCount; xp++ )
					{
						if ( PeriodArray[xp] == Chromosome[ChromoIndex].Period )
						{
							break;
						}
					}
					if ( xp >= PeriodCount )
					{
						 PeriodArray[PeriodCount] = Chromosome[ChromoIndex].Period;
						 PeriodCount++;
					}
				}
			}
		}
		
		Students += (RequestArray[RequestIndex].ClassCount - PeriodCount);
	}
#endif
	/*---------------------------------------------------------------------------
		tally conflicts for number of teachers available for each course.
	---------------------------------------------------------------------------*/
	for ( int ndx = 0; ndx < Count; ndx++ )
	{
		Array[ndx].Count = 0;
	}
	for ( int ChromoIndex = 0; ChromoIndex < ClassCount; ChromoIndex++ )
	{
		key.Period = Chromosome[ChromoIndex].Period;
		key.CourseIndex = ClassArray[ChromoIndex].CourseIndex;
		if (( ptr = bsearch ( &key, Array, Count, sizeof(RECORD), (int(*)()) cmprec )) == NULL )
		{
			printf ( "obj_func: bsearch Array failed\n" );
			exit ( 1 );
		}
		ptr->Count++;
	}
	for ( int ndx = 0; ndx < Count; ndx++ )
	{
		if ( Array[ndx].Count > CourseArray[Array[ndx].CourseIndex].Teachers )
		{
			Teachers += Array[ndx].Count - CourseArray[Array[ndx].CourseIndex].Teachers;
		}
	}

	*StudentConflicts = Students;
	*TeacherConflicts = Teachers;
	Total = Students + Teachers;
	// printf ( "Students %d + Teachers %d = %d\n", Students, Teachers, Total );

	return ( Total );
}
