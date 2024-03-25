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

static int cmprequest ( REQUEST_RECORD *a, REQUEST_RECORD *b )
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

static void SetRingIndex ( int CourseIndex, int ClassIndex )
{
	CourseArray[CourseIndex].RingIndex = ClassIndex;
}

static int GetRingIndex ( int CourseIndex )
{
	int		rv, Next;

	rv =  CourseArray[CourseIndex].RingIndex;

	if ( ClassArray[rv].CourseIndex == ClassArray[rv+1].CourseIndex )
	{
		Next = rv + 1;
	}
	else
	{
		Next = rv - 1;
		while ( Next >= 0 && ClassArray[Next].CourseIndex == CourseIndex )
		{
			Next--;
		}
		Next++;
	}
	CourseArray[CourseIndex].RingIndex = Next;

	return ( rv );
}

void LoadRequests ()
{
	FILE	*ifp;
//	int		xe = 0;
	int		lineno;
	int		xo;
	int		CourseID;

	if ( ClassCount == 0 )
	{
		LoadClasses ( 1 );
	}

	/*----------------------------------------------------------
		set RingIndex for each course
	----------------------------------------------------------*/
	SetRingIndex ( ClassArray[0].CourseIndex, 0 );
	for ( int xc = 1; xc < ClassCount; xc++ )
	{
		if ( ClassArray[xc].CourseIndex != ClassArray[xc-1].CourseIndex )
		{
			SetRingIndex ( ClassArray[xc].CourseIndex, xc );
		}
	}

	if ( Verbose )
	{
		DumpCourses ();
	}

	if (( ifp = fopen ( "requests.CSV", "r" )) == NULL )
	{
		printf ( "Cannot open requests.CSV\n" );
		exit ( 1 );
	}

	RequestCount = 0;
	while ( fgets ( buffer, sizeof(buffer), ifp ) != NULL )
	{
		if ( buffer[0] == '#' )
		{
			continue;
		}
		if (( tokcnt = GetTokensD ( buffer, ",\n\r", tokens, MAXTOKS )) < 3 )
		{	
			continue;
		}
		if ( atoi ( tokens[0] ) == 0 )
		{
			continue;
		}

		RequestCount++;
	}

	if (( RequestArray = calloc ( RequestCount, sizeof(REQUEST_RECORD) )) == NULL )
	{
		printf ( "init: calloc RequestArray failed, %s\n", strerror(errno) );
	}

	rewind ( ifp );

/*----------------------------------------------------------
# these are fake student requests for testing
  1, 9, 101, 102, 103, 104, 111, 112
  2, 9, 101, 102, 103, 104, 113, 501, 502
  3, 9, 101, 102, 103, 104, 503, 504
  4, 9, 101, 102, 103, 104, 505, 506, 507
  5, 9, 101, 102, 103, 104, 111, 112
  6, 9, 101, 102, 103, 104, 113, 501, 502
  7, 9, 101, 102, 103, 104, 503, 504
  8, 9, 101, 102, 103, 104, 505, 506, 507
  9, 9, 101, 102, 103, 104, 111, 112
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
		if (( tokcnt = GetTokensD ( buffer, ",\n\r", tokens, MAXTOKS )) < 8 )
		{
			continue;
		}
		if (( RequestArray[xo].StudentID = atoi ( tokens[0] )) == 0 )
		{
			continue;
		}
		if ( xo >= RequestCount )
		{
			printf ( "Exceeds RequestCount\n" );
			exit ( 1 );
		}
		RequestArray[xo].Level = atoi ( tokens[1] );
		RequestArray[xo].ClassCount = tokcnt - 2;

		/*----------------------------------------------------------
			assign classes cyclicaly
		----------------------------------------------------------*/
		for ( int ndx = 0; ndx < RequestArray[xo].ClassCount; ndx++ )
		{
			CourseID = atoi ( tokens[2+ndx] );

			for ( int xc = 0; xc < CourseCount; xc++ )
			{
				if ( CourseArray[xc].CourseID == CourseID )
				{
					RequestArray[xo].ClassIndex[ndx] = GetRingIndex ( xc );
					break;
				}
			}
		}
		xo++;
	}
	printf ( "Loaded %d Requests\n", RequestCount );
	fclose ( ifp );

	qsort ( RequestArray, RequestCount, sizeof(REQUEST_RECORD), (int(*)()) cmprequest );

	if ( Verbose )
	{
		DumpRequests ();
	}
}

void DumpRequests ()
{
	for ( int xr = 0; xr < RequestCount; xr++ )
	{
		printf ( "%4d %2d %2d: ", 
			RequestArray[xr].StudentID, RequestArray[xr].Level, RequestArray[xr].ClassCount );

		for ( int xc = 0; xc < RequestArray[xr].ClassCount; xc++ )
		{
			printf ( " %3d", RequestArray[xr].ClassIndex[xc] );
		}
		printf ( "\n" );
	}
}

