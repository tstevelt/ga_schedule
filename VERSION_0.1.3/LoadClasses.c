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

static	int		CompareBy = 0;
int cmpclass ( CLASS_RECORD *a, CLASS_RECORD *b )
{
	if ( CompareBy == 1 )
	{
		if ( a->ClassID < b->ClassID )
		{
			return ( -1 );
		}
		if ( a->ClassID > b->ClassID )
		{
			return ( 1 );
		}
	}
	else
	{
		return ( strcmp(a->ClassCode,b->ClassCode) );
	}
	return ( 0 );
}

void LoadClasses ( int SortBy )
{
	FILE	*ifp;
	int		lineno;
//	int		xe = 0;
//	int		xo;

	switch ( SortBy )
	{
		case 1:
		case 2:
			CompareBy = SortBy;
			break;
		default:
			printf ( "LoadClasses: unknown SortBy %d\n", SortBy );
			exit ( 1 );
	}

	if ( CourseCount == 0 )
	{
		LoadCourses ();
	}

	if (( ifp = fopen ( "classes.CSV", "r" )) == NULL )
	{
		printf ( "Cannot open classes.CSV\n" );
		exit ( 1 );
	}

	lineno = 0;
	while ( fgets ( buffer, sizeof(buffer), ifp ) != NULL )
	{
		lineno++;

		if (( tokcnt = GetTokensD ( buffer, ",\n\r", tokens, MAXTOKS )) < 3 )
		{
			continue;
		}
		if (( ClassArray[ClassCount].ClassID = atoi ( tokens[0] )) == 0 )
		{
			continue;
		}
		if ( ClassCount >= MAXCLASS )
		{
			printf ( "Exceeds MAXCLASS\n" );
			exit ( 1 );
		}
		int CourseID = atoi ( tokens[1] );
		snprintf ( ClassArray[ClassCount].ClassCode, sizeof(ClassArray[ClassCount].ClassCode), "%s", tokens[2] );

		for ( int xc = 0; xc < CourseCount; xc++ )
		{
			if ( CourseArray[xc].CourseID == CourseID )
			{
				ClassArray[ClassCount].CourseIndex = xc;
				break;
			}
		}

		ClassCount++;
	}
	printf ( "Loaded %d Classs\n", ClassCount );
	fclose ( ifp );

	qsort ( ClassArray, ClassCount, sizeof(CLASS_RECORD), (int(*)()) cmpclass );

	if ( Verbose )
	{
		DumpClasses ();
	}

}

void DumpClasses ()
{
	for ( int xc = 0; xc < ClassCount; xc++ )
	{
		printf ( "%4d %4d %s\n",
			ClassArray[xc].ClassID,
			ClassArray[xc].CourseIndex,
			ClassArray[xc].ClassCode );
	}
}
