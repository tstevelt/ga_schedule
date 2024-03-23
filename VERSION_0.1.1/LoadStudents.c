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

static int cmpstudent ( STUDENT_RECORD *a, STUDENT_RECORD *b )
{
	if ( a->StudentID < b->StudentID )
	{
		return ( -1 );
	}
	if ( a->StudentID > b->StudentID )
	{
		return ( 1 );
	}
	return ( 0 );
}

void LoadStudents ()
{
	FILE	*ifp;
	char	buffer[1024];
	char	*tokens[10];
	int		tokcnt;
	int		lineno;
	int		xo;


	if (( ifp = fopen ( "students.CSV", "r" )) == NULL )
	{
		printf ( "Cannot open students.CSV\n" );
		exit ( 1 );
	}

	if ( StudentCount == 0 )
	{
		while ( fgets ( buffer, sizeof(buffer), ifp ) != NULL )
		{
			if ( buffer[0] == '#' )
			{
				continue;
			}
			if (( tokcnt = GetTokensD ( buffer, ",\n\r", tokens, 10 )) < 3 )
			{	
				continue;
			}
			if ( atoi ( tokens[0] ) == 0 )
			{
				continue;
			}

			StudentCount++;
		}

		if (( StudentArray = calloc ( StudentCount, sizeof(STUDENT_RECORD) )) == NULL )
		{
			printf ( "init: calloc StudentArray failed, %s\n", strerror(errno) );
		}

		rewind ( ifp );
	}

/*----------------------------------------------------------
# this file is created by FakeNames program
# Levels are added by hand (sed)
ID,LEVEL,NAME
1,9,Gianna Amos
2,9,Aaron Sharpe
3,9,Joseph Gray
4,9,Adam Marley
5,9,Cameron Elsher
6,9,Santiago Thatcher
7,9,Adrian Lennon
----------------------------------------------------------*/
	lineno = 0;
	xo = 0;
	while ( fgets ( buffer, sizeof(buffer), ifp ) != NULL )
	{
		lineno++;

		if ( buffer[0] == '#' )
		{
			continue;
		}
		if (( tokcnt = GetTokensD ( buffer, ",\n\r", tokens, 10 )) < 3 )
		{
			continue;
		}
		if (( StudentArray[xo].StudentID = atoi ( tokens[0] )) == 0 )
		{
			continue;
		}
		if ( xo >= StudentCount )
		{
			printf ( "Exceeds StudentCount\n" );
			exit ( 1 );
		}
		StudentArray[xo].Level = atoi ( tokens[1] );
		sprintf ( StudentArray[xo].Name, "%s", tokens[2] );
		xo++;
	}
	printf ( "Loaded %d Students\n", StudentCount );
	fclose ( ifp );

	qsort ( StudentArray, StudentCount, sizeof(STUDENT_RECORD), (int(*)()) cmpstudent );

	if ( Verbose )
	{
		DumpStudents ();
	}
}

void DumpStudents ()
{
	for ( int xr = 0; xr < StudentCount; xr++ )
	{
		printf ( "%4d %2d %s\n", 
			StudentArray[xr].StudentID, StudentArray[xr].Level, StudentArray[xr].Name );
	}
}

