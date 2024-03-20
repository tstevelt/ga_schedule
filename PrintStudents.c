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

void PrintStudents ()
{
	STUDENT_RECORD	*ptrStudent;
	FILE			*ofp;

	if (( ofp = fopen ( "schedule_students.TXT", "w" )) == NULL )
	{
		printf ( "Cannot create schedule_students.TXT\n" );
		ofp = stdout;
	}

	if ( StudentCount == 0 )
	{
		LoadStudents ();
	}

	for ( int xc = 0; xc < ClassCount; xc++ )
	{
		ClassArray[xc].Period = BestIndividual.Chromosome[xc].Period;
	}
	
	for ( int xr = 0; xr < RequestCount; xr++ )
	{
		ptrStudent = FindStudent ( RequestArray[xr].StudentID );
		fprintf ( ofp, "%4d %s\n", RequestArray[xr].StudentID, ptrStudent->Name );

		for ( int xp = 1; xp < MAXPERIODS; xp++ )
		{
			int xo = 0;
			for ( int xc = 0; xc < RequestArray[xr].ClassCount; xc++ )
			{
				if ( xp == ClassArray[xc].Period )
				{
					if ( xo == 0 )
					{
						fprintf ( ofp, " %2d:", xp );
					}
					fprintf ( ofp, "  [%s %s]",
						ClassArray[RequestArray[xr].ClassIndex[xc]].ClassCode,
						CourseArray[ClassArray[RequestArray[xr].ClassIndex[xc]].CourseIndex].Name );
					xo++;
				}
			}
			if ( xo )
			{
				fprintf ( ofp, "\n" );
			}
		}

		fprintf ( ofp, "\n\n" );
	}

	if ( ofp != stdout )
	{
		fclose ( ofp );
	}
}
