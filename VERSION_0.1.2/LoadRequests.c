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

static	FILE	*ifp;

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

static void DumpRequests ()
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

static	char	buffer[1024];
static	char	*tokens[10];
static	int		tokcnt;

static void OpenFileAndAllocateArray ()
{
	if (( ifp = fopen ( "requests.CSV", "r" )) == NULL )
	{
		printf ( "Cannot open requests.CSV\n" );
		exit ( 1 );
	}

	/*----------------------------------------------------------
		get number of requests and allocate array
	----------------------------------------------------------*/
	RequestCount = 0;
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

		RequestCount++;
	}

	if (( RequestArray = calloc ( RequestCount, sizeof(REQUEST_RECORD) )) == NULL )
	{
		printf ( "init: calloc RequestArray failed, %s\n", strerror(errno) );
	}

	rewind ( ifp );
}

typedef struct
{
	int		StudentID;
	int		Slot;
	double	Sort;

	int		ClassIndex;
} RECORD;

static	RECORD	Array[MAXPERIODS*MAXPERCLASS];
static	int		Count;

static int cmprec ( RECORD *a, RECORD *b )
{
	if ( a->Sort < b->Sort )
	{
		return ( -1 );
	}
	return ( 1 );
}

//  Array[ndx].ClassIndex = GetNextClass ( CourseArray[xc].CourseID );
static int GetNextClass ( int CourseID )
{
			int		rv;
	static	int		ClassIndex = 0;

	rv = -1;

	while ( rv == -1 )
	{
		if ( ClassIndex >= ClassCount )
		{
			ClassIndex = 0;
		}

		if ( ClassArray[ClassIndex].CourseID == CourseID )
		{
			rv = ClassIndex;
			ClassIndex++;
			break;
		}

		ClassIndex++;
	}


	return ( rv );
}

void LoadRequestsRandom ()
{
//	int		xe = 0;
	int		lineno;
	int		xc;			// course
	int		xr;			// request
	int		xs;			// slot
//	int		CourseID;
	REQUEST_RECORD	Key, *Ptr;

	if ( ClassCount == 0 )
	{
		LoadClasses ();
	}

	if ( Verbose )
	{
		DumpCourses ();
	}

	OpenFileAndAllocateArray ();

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
	xr = 0;
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
		if (( RequestArray[xr].StudentID = atoi ( tokens[0] )) == 0 )
		{
			continue;
		}
		if ( xr >= RequestCount )
		{
			printf ( "Exceeds RequestCount\n" );
			exit ( 1 );
		}
		RequestArray[xr].Level = atoi ( tokens[1] );
		RequestArray[xr].CourseCount = RequestArray[xr].ClassCount = tokcnt - 2;

		for ( xs = 0; xs < RequestArray[xr].CourseCount; xs++ )
		{
			RequestArray[xr].CourseID[xs] = atoi ( tokens[2+xs] );
		}
		xr++;
	}
	printf ( "Loaded %d Requests\n", RequestCount );

	fclose ( ifp );

	qsort ( RequestArray, RequestCount, sizeof(REQUEST_RECORD), (int(*)()) cmprequest );

	for ( xc = 0; xc < CourseCount; xc++ )
	{
		Count = 0;
		for ( xr = 0; xr < RequestCount; xr++ )
		{
			for ( xs = 0; xs < RequestArray[xr].CourseCount; xs++ )
			{
				if ( RequestArray[xr].CourseID[xs] == CourseArray[xc].CourseID )
				{
					Array[Count].StudentID = RequestArray[xr].StudentID;
					Array[Count].Slot = xs;
					Array[Count].Sort = d_random ();
					Count++;
				}
			}
		}
		qsort ( Array, Count, sizeof(RECORD), (int(*)()) cmprec );

		for ( int ndx = 0; ndx < Count; ndx++ )
		{
			Array[ndx].ClassIndex = GetNextClass ( CourseArray[xc].CourseID );
		}

		if ( Verbose )
		{
			printf ( "%3d %s\n", CourseArray[xc].CourseID, CourseArray[xc].Name );
			for ( int ndx = 0; ndx < Count; ndx++ )
			{
				printf ( "%3d %2d %.4f %3d\n", 
						Array[ndx].StudentID, Array[ndx].Slot, Array[ndx].Sort, Array[ndx].ClassIndex );
			}
		}

// wip
		for ( int ndx = 0; ndx < Count; ndx++ )
		{
			Key.StudentID = Array[ndx].StudentID;
			Ptr = bsearch ( &Key, RequestArray, RequestCount, sizeof(REQUEST_RECORD), (int(*)()) cmprequest );
			if ( Ptr == NULL )
			{
				printf ( "bsearch failed on %d\n", Key.StudentID );	
			}
			else
			{
				Ptr->ClassIndex[Array[ndx].Slot] = Array[ndx].ClassIndex;
			}
		}
	}
}

void LoadRequestsRing ()
{
//	int		xe = 0;
	int		lineno;
	int		xo;
	int		CourseID;

	if ( ClassCount == 0 )
	{
		LoadClasses ();
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

	OpenFileAndAllocateArray ();

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
		if (( tokcnt = GetTokensD ( buffer, ",\n\r", tokens, 10 )) < 8 )
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
