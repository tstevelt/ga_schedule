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

void PrintSchedule ()
{
	CLASS_RECORD	*ptrClass;
	COURSE_RECORD	*ptrCourse;
	FILE			*ofp;

	if (( ofp = fopen ( "classes_summary.TXT", "w" )) == NULL )
	{
		printf ( "Cannot create classes_summary.TXT\n" );
		ofp = stdout;
	}

	fprintf ( ofp, "BEST PERIOD SCHEDULE\n" );
	fprintf ( ofp, "AFTER %d GENERATIONS, %d CONFLICTS\n", GenerationCount, BestFitness );
	fprintf ( ofp, "--------------------------------------------------\n" );
	fprintf ( ofp, "PERIOD   ID  CLASS  NAME\n" );
	//          99   9999  9999A  x30xxxxxxxxxxxxxxxxxxxxxxxxxxx
	fprintf ( ofp, "--------------------------------------------------\n" );
	for ( int xp = 1; xp <= MAXPERIODS; xp++ )
	{
		for ( int xc = 0; xc < ClassCount; xc++ )
		{
			if ( BestIndividual.Chromosome[xc].Period == xp )
			{
				ptrClass = &ClassArray[xc];
				ptrCourse = &CourseArray[ptrClass->CourseIndex];

				fprintf ( ofp, "  %2d   %4d  %5.5s  %s\n", 
					xp, ptrClass->ClassID, ptrClass->ClassCode, ptrCourse->Name );
				
			}
		}
		

		fprintf ( ofp, "\n" );
	}

	fprintf ( ofp, "\n" );

	if ( ofp != stdout )
	{
		fclose ( ofp );
	}
}
