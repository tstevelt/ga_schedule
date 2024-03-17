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

int cmpcourse ( COURSE_RECORD *a, COURSE_RECORD *b )
{
	if ( a->ID < b->ID )
	{
		return ( -1 );
	}
	if ( a->ID > b->ID )
	{
		return ( 1 );
	}
	return ( 0 );
}

void LoadCourses ()
{
	FILE	*ifp;
	char	buffer[1024];
	char	*tokens[10];
	int		tokcnt;
	int		lineno;
//	int		xe = 0;
//	int		xo;

	if (( ifp = fopen ( "courses.TXT", "r" )) == NULL )
	{
		printf ( "Cannot open courses.TXT\n" );
		exit ( 1 );
	}

	/*----------------------------------------------------------
		# required courses by year
		101,9,R,English
		102,9,R,Algebra 1
		103,9,R,Biology 1
		104,9,R,US History
		201,10,R,Literature
		202,10,R,Algebra 2
		...
	----------------------------------------------------------*/
	lineno = 0;
	while ( fgets ( buffer, sizeof(buffer), ifp ) != NULL )
	{
		lineno++;

		if (( tokcnt = GetTokensD ( buffer, ",\n\r", tokens, 10 )) < 4 )
		{
			continue;
		}
		if ( CourseCount >= MAXCOURSES )
		{
			printf ( "Exceeds MAXCOURSES\n" );
			exit ( 1 );
		}
		CourseArray[CourseCount].ID = atoi ( tokens[0] );
		CourseArray[CourseCount].Level = atoi ( tokens[1] );
		switch ( tokens[2][0] )
		{
			case 'R':
				CourseArray[CourseCount].Required = 1;
				break;
			case 'E':
				CourseArray[CourseCount].Required = 0;
				break;
			default:
				printf ( "Unknown course type in line %d\n", lineno );
				exit ( 1 );
		}
		snprintf ( CourseArray[CourseCount].Name, MAXNAME, "%s", tokens[3] );
		CourseCount++;
	}
	printf ( "Loaded %d courses\n", CourseCount );
	fclose ( ifp );

	qsort ( CourseArray, CourseCount, sizeof(COURSE_RECORD), (int(*)()) cmpcourse );
}
