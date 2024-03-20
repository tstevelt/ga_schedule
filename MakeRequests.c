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

void MakeRequests ()
{
	FILE	*ifp, *ofp;
	char	buffer[1024];
	char	*tokens[10];
	int		tokcnt;
	int		lineno;
	int		xe = 0;
	int		xo, xs, xl;

	if (( ofp = fopen ( "requests.CSV", "w" )) == NULL )
	{
		printf ( "Cannot create requests.CSV\n" );
		exit ( 1 );
	}

	fprintf ( ofp, "# these are fake student requests for testing\n" );
	fprintf ( ofp, "ID,LAVEL,CRS1,CRS2,CRS3,CRS4,CRS5,CRS6,CRS7\n" );

	LoadCourses ();

	if (( ifp = fopen ( "students.CSV", "r" )) == NULL )
	{
		printf ( "Cannot open students.CSV\n" );
		exit ( 1 );
	}

	/*----------------------------------------------------------
		  1,9,Gianna Amos
		  2,9,Aaron Sharpe
		  3,9,Joseph Gray
		  4,9,Adam Marley
		  5,9,Cameron Elsher
		  6,9,Santiago Thatcher
		  7,9,Adrian Lennon
		  8,9,Logan Marley
		  9,9,Adrian Sharpe
		 10,9,Hudson Bardot
		 ...
	----------------------------------------------------------*/
	lineno = xs = 0;
	while ( fgets ( buffer, sizeof(buffer), ifp ) != NULL )
	{
		lineno++;

		if (( tokcnt = GetTokensD ( buffer, ",\n\r", tokens, 10 )) < 3 )
		{
			continue;
		}

		if (( Student.StudentID = atoi ( tokens[0] )) == 0 )
		{
			continue;
		}

		xs++;

		Student.Level = atoi ( tokens[1] );
		switch ( Student.Level )
		{
			case 9:
			case 10:
			case 11:
			case 12:
				break;
			default:
				printf ( "Unknown student level in line %d\n", lineno );
				exit ( 1 );
		}
		snprintf ( Student.Name, MAXNAME, "%s", tokens[2] );
		RequestCount++;

		fprintf ( ofp, "%3d,%2d", Student.StudentID, Student.Level );

		/*----------------------------------------------------------
			print required courses for student's grade level
		----------------------------------------------------------*/
		for ( int ndx = 0; ndx < CourseCount; ndx++ )
		{
			if ( CourseArray[ndx].Level == Student.Level && CourseArray[ndx].Required == 1 )
			{
				fprintf ( ofp, ",%4d", CourseArray[ndx].CourseID );
			}
		}

		/*----------------------------------------------------------
			print two or three electives for grade level or 5xx level 0
		----------------------------------------------------------*/
		if ( xs % 2 )
		{
			xl = 2;
		}
		else
		{
			xl = 3;
		}

		for ( xo = 0; xo < xl;  )
		{
			if (( CourseArray[xe].Required == 0 ) &&
				( CourseArray[xe].Level == 0 || CourseArray[xe].Level == Student.Level ))
			{
				fprintf ( ofp, ",%4d", CourseArray[xe].CourseID );
				xo++;
			}

			xe++;
			if ( xe == CourseCount )
			{
				xe = 0;
			}
		}

		fprintf ( ofp, "\n" );
	}

	printf ( "Loaded %d students\n", RequestCount );
	fclose ( ifp );

	fclose ( ofp );
}
