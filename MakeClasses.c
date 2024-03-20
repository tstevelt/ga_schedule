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

void MakeClasses ()
{
	FILE	*ifp, *ofp;
	char	buffer[1024];
	char	*tokens[10];
	int		tokcnt;
	int		lineno;
	int		xe;
	int		xo;
	int		xc;
	COURSE_RECORD	key, *ptr;
	int		Total, Classes, Remain;
	double	PerClass;
	int		ClassID;

	LoadCourses ();

	if (( ifp = fopen ( "requests.CSV", "r" )) == NULL )
	{
		printf ( "Cannot open requests.CSV\n" );
		exit ( 1 );
	}

	/*----------------------------------------------------------
	# these are fake student requests for testing
	  1, 9, 101, 102, 103, 104, 111, 112, 113
	  2, 9, 101, 102, 103, 104, 501, 502, 503
	  3, 9, 101, 102, 103, 104, 504, 505, 506
	  4, 9, 101, 102, 103, 104, 507, 111, 112
	  5, 9, 101, 102, 103, 104, 113, 501, 502
	  6, 9, 101, 102, 103, 104, 503, 504, 505
	  7, 9, 101, 102, 103, 104, 506, 507, 111
	  8, 9, 101, 102, 103, 104, 112, 113, 501
	  9, 9, 101, 102, 103, 104, 502, 503, 504
	----------------------------------------------------------*/
	lineno = 0;
	while ( fgets ( buffer, sizeof(buffer), ifp ) != NULL )
	{
		lineno++;

		if ( buffer[0] == '#' )
		{
			continue;
		}

		if (( tokcnt = GetTokensD ( buffer, ",\n\r", tokens, 10 )) < 8 )
		{
			continue;
		}
		if ( atoi ( tokens[0] ) == 0 )
		{
			continue;
		}

		for ( int xt = 2; xt < tokcnt; xt++ )
		{
			key.CourseID = atoi ( tokens[xt] );
			if (( ptr = bsearch ( &key, CourseArray, CourseCount, sizeof(COURSE_RECORD), (int(*)()) cmpcourse )) == NULL )
			{
				printf ( "Unknown course ID %d in line %d\n", key.CourseID, lineno );
				continue;
			}
			ptr->Counter++;
		}
	}

	fclose ( ifp );

	if ( Verbose )
	{
		DumpCourses ();
	}

	if (( ofp = fopen ( "classes.CSV", "w" )) == NULL )
	{
		printf ( "Cannot create classes.CSV\n" );
		exit ( 1 );
	}

	ClassID = 1;
	fprintf ( ofp, "ID,COURSE,CLASS\n" );
	for ( xc = 0, xo = 0; xc < CourseCount; xc++ )
	{
		Total    = CourseArray[xc].Counter;
		Classes  = Total / MAXPERCLASS;
		Remain   = Total % MAXPERCLASS;
		PerClass = MAXPERCLASS;

		if ( Remain )
		{
			if ( Classes < MAXPERIODS )
			{
				Classes++;
				PerClass = (double) Total / (double) Classes;
				Remain = 0;
			}
			else
			{
				printf ( "Too many requests for course %d, overflow %d\n", CourseArray[xc].CourseID, Remain );
			}
		}

		if ( Verbose )
		{
			printf ( "%4d %4d %5.1f %3d %3d\n", 
						CourseArray[xc].CourseID, CourseArray[xc].Counter, PerClass, Classes, Remain );
		}

		
		for ( xe = 0; xe < Classes; xe++ )
		{
			fprintf ( ofp, "%3d,%3d,%3d%c\n", 
						ClassID, CourseArray[xc].CourseID, CourseArray[xc].CourseID, 'A' + xe );
			ClassID++;
			xo++;
		}
	}

	fclose ( ofp );

	printf ( "Output %d classes, which averages %.1f class rooms per period\n", xo, (double) xo / (double) MAXPERIODS );
}
