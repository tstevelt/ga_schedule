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

typedef struct
{
	int		Periods[MAXPERIODS];
	double	SortValue;
} PERMUTATION;

static	PERMUTATION	*PermutationArray;
static	int			 PermutationCount;
static	int			 PermutationIndex;
static	int			ShuffleCount;

static void swap(int *a, int *b) 
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

static void generatePermutations(int Periods[], int start, int end) 
{
	if (start == end) 
	{
		for ( int i = 0; i < MAXPERIODS; i++ )
		{
			PermutationArray[PermutationCount].Periods[i] = Periods[i];
		}
		PermutationArray[PermutationCount].SortValue = d_random ();
		PermutationCount++;
		return;
	}

	for (int i = start; i <= end; i++) 
	{
		swap(&Periods[start], &Periods[i]);
		generatePermutations(Periods, start + 1, end);
		swap(&Periods[start], &Periods[i]); // Backtrack
	}
}

static int cmppermutation ( PERMUTATION *a, PERMUTATION *b )
{
	if ( a-> SortValue < b->SortValue )
	{
		return ( -1 );
	}
	return ( 1 );
}

static void shuffle ()
{
	for ( int i = 0; i < PermutationCount; i++ )
	{
		PermutationArray[i].SortValue = d_random ();
	}
	qsort ( PermutationArray, PermutationCount, sizeof(PERMUTATION), (int(*)()) cmppermutation );
	PermutationIndex = 0;
	ShuffleCount++;
}

typedef struct
{
	int		CourseID;
	int		Period;
	char	ClassCode[10];
	int		Count;
} RECORD;

static	RECORD	MyArray[MAXCLASS];
static	int		MyCount;
static	RECORD	*ptrCourse, key;

static int cmpMyCourse ( RECORD *a, RECORD *b )
{
	if ( a->CourseID < b->CourseID )
	{
		return ( -1 );
	}
	if ( a->CourseID > b->CourseID )
	{
		return ( 1 );
	}
	if ( a->Period < b->Period )
	{
		return ( -1 );
	}
	if ( a->Period > b->Period )
	{
		return ( 1 );
	}

	return ( 0 );
}

static void DumpMyArray ()
{
	for ( int xc = 0; xc < ClassCount; xc++ )
	{
		printf ( "%4d %s %d %d\n", 
			MyArray[xc].CourseID, MyArray[xc].ClassCode, MyArray[xc].Period, MyArray[xc].Count );
	}
}

typedef struct
{
	int		CourseID;
	int		Period;
} REQUEST;

static	REQUEST	MyRequestArray[MAXPERIODS];
static	int		MyRequestCount;

void AssignStudents ()
{
	FILE	*ifp, *ofp;
	int		xt;
	int		Periods[MAXPERIODS];
	int		StudentID;
	int		Successes = 0;

	PermutationCount = 0;
	PermutationIndex = 1;
	for ( int i = 1; i <= MAXPERIODS; i++ )
	{
		Periods[i-1] = i;
		PermutationIndex *= i;
	}
	if (( PermutationArray = calloc ( PermutationIndex, sizeof(PERMUTATION) )) == NULL )
	{
		printf ( "AssignStudents: calloc failed, size %d\n", PermutationCount );
		exit ( 1 );
	}
	generatePermutations( Periods, 0, MAXPERIODS - 1);
	shuffle ();

	LoadClasses ( 1 );

	if (( ifp = fopen ( "schedule_chromosome.TXT", "r" )) == NULL )
	{
		printf ( "Cannot open schedule_chromosome.TXT for input\n" );
		exit ( 1 );
	}
	fgets ( buffer, sizeof(buffer), ifp );
	TrimRight ( buffer );
	fclose ( ifp );

	if ( Verbose )
	{
		printf ( "%s\n", buffer );
	}

	if ( ClassCount !=  strlen(buffer) )
	{
		printf ( "Class count %d NOT EQUAL chromo length %ld\n", ClassCount, strlen(buffer) );
		exit ( 1 );
	}
	else
	{
		printf ( "Class count %d, chromo length %ld\n", ClassCount, strlen(buffer) );
	}

	MyCount = 0;
	for ( int xc = 0; xc < ClassCount; xc++ )
	{
		if ( MyCount >= MAXCLASS )
		{
			printf ( "AssignStudents: exceeds MAXCLASS\n" );
			exit ( 1 );
		}

		MyArray[MyCount].CourseID = CourseArray[ClassArray[xc].CourseIndex].CourseID;
		sprintf ( MyArray[MyCount].ClassCode, "%s", ClassArray[xc].ClassCode );
		MyArray[MyCount].Period   = buffer[xc] - '0';
		MyArray[MyCount].Count    = 0;
		MyCount++;
	}

	qsort ( MyArray, MyCount, sizeof(RECORD), (int(*)()) cmpMyCourse );

	if ( Verbose )
	{
		// DumpMyArray ();
	}

	if (( ifp = fopen ( "requests.CSV", "r" )) == NULL )
	{
		printf ( "Cannot open requests.CSV for input\n" );
		exit ( 1 );
	}

	if (( ofp = fopen ( "assign.CSV", "w" )) == NULL )
	{
		printf ( "Cannot open assign.CSV for input\n" );
		exit ( 1 );
	}

	fprintf ( ofp, "STUDENT,CLASS,PERIOD\n" );

	/*----------------------------------------------------------
		# this file is created by MakeRequests()
		# these are fake student requests for testing
		ID,LEVEL,CRS1,CRS2,CRS3,CRS4,CRS5,CRS6,CRS7
		  1, 9, 101, 102, 103, 104, 111, 112
		  2, 9, 101, 102, 103, 104, 113, 501, 502
		  3, 9, 101, 102, 103, 104, 503, 504
		  4, 9, 101, 102, 103, 104, 505, 506, 507
		  5, 9, 101, 102, 103, 104, 111, 112
		  6, 9, 101, 102, 103, 104, 113, 501, 502
		  7, 9, 101, 102, 103, 104, 503, 504
	----------------------------------------------------------*/
	int		MinLength = 0;
	int		MaxLength = 0;
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
		if (( StudentID =  atoi ( tokens[0] )) == 0 )
		{
			continue;
		}
		if ( MinLength == 0 || MinLength > tokcnt )
		{
			MinLength = tokcnt;;
		}
		if ( MaxLength == 0 || MaxLength < tokcnt )
		{
			MaxLength = tokcnt;;
		}
	}
	rewind ( ifp );

	int		Length = MaxLength;
	while ( Length >= MinLength )
	{
		
	if ( Verbose )
	{
		printf ( "Pass for length %d\n", Length );
	}

	while ( fgets ( buffer, sizeof(buffer), ifp ) != NULL )
	{
		if ( buffer[0] == '#' )
		{
			continue;
		}
		if (( tokcnt = GetTokensD ( buffer, ",\n\r", tokens, MAXTOKS )) != Length )
		{	
			continue;
		}
		if (( StudentID =  atoi ( tokens[0] )) == 0 )
		{
			continue;
		}

		MyRequestCount = 0;
		for ( xt = 2; xt < tokcnt; xt++ )
		{
			if ( MyRequestCount > MAXPERIODS )
			{
				printf ( "AssignStudents: exceeds MAXPERIODS\n" );
				exit ( 1 );
			}
			MyRequestArray[MyRequestCount].CourseID = atoi(tokens[xt]);
			MyRequestArray[MyRequestCount].Period = 0;
			MyRequestCount++;
		}

		int		TryThis;
		int		AssignCount;
		int		xp;
		int		AttemptCount = 0;
		int		Success = 0;

		while ( 1 )
		{
			if ( AttemptCount > PermutationCount )
			{
				break;
			}

			if ( PermutationIndex >= PermutationCount )
			{
				shuffle ();
			}

			AssignCount = 0;
			xp = 0;
			for ( int xr = 0; xr < MyRequestCount; xr++ )
			{
				TryThis = 0;
				key.CourseID = MyRequestArray[xr].CourseID;
				for ( ; TryThis == 0 && xp < MAXPERIODS; xp++ )
				{
					key.Period = PermutationArray[PermutationIndex].Periods[xp];
					ptrCourse = bsearch ( &key, MyArray, MyCount, sizeof(RECORD), (int(*)()) cmpMyCourse );
					if ( ptrCourse == NULL )
					{
						continue;
					}
					if ( ptrCourse->Count < MAXPERCLASS )
					{
						TryThis = 1;
					}

				} // xxxx for ( ; TryThis == 0 && xp < MAXPERIODS; xp++ )

				if ( TryThis )
				{
					MyRequestArray[xr].Period = key.Period;
					AssignCount++;
				}

			} // xxxx for ( int xr = 0; xr < MyRequestCount; xr++ )

			PermutationIndex++;
			AttemptCount++;

			if ( AssignCount == MyRequestCount )
			{
				for ( int xr = 0; xr < MyRequestCount; xr++ )
				{
					key.CourseID = MyRequestArray[xr].CourseID;
					key.Period   = MyRequestArray[xr].Period;
					ptrCourse = bsearch ( &key, MyArray, MyCount, sizeof(RECORD), (int(*)()) cmpMyCourse );
					ptrCourse->Count = ptrCourse->Count + 1;

					fprintf ( ofp, "%4d,%s,%3d\n", StudentID, ptrCourse->ClassCode, key.Period );
				}

				Successes++;
				Success = 1;
				break;
			}
			else
			{
				for ( int xr = 0; xr < MyRequestCount; xr++ )
				{
					MyRequestArray[xr].Period = 0;
				}
				continue;
			}

		} // xxxx while ( 1 )

		if ( Verbose )
		{
			printf ( "Student: %d Attempts: %d Index %d\n", StudentID, AttemptCount, PermutationIndex );
		}

		if ( Verbose )
		{
			for ( int xr = 0; xr < MyRequestCount; xr++ )
			{
				fprintf ( ofp, "%d %d\n",  
					MyRequestArray[xr].CourseID,
					MyRequestArray[xr].Period );
			}
		}

		if ( Success == 0 )
		{
			for ( int xr = 0; xr < MyRequestCount; xr++ )
			{
				fprintf ( ofp, "%4d,%3dZ,%3d\n",  
					StudentID,
					MyRequestArray[xr].CourseID,
					MyRequestArray[xr].Period );
			}
		}

	} // xxxx while ( fgets ( buffer, sizeof(buffer), ifp ) != NULL )

		Length--;
		rewind ( ifp );

	} // xxxx while ( Length >= MinLength )

	fclose ( ifp );
	fclose ( ofp );

	if ( Verbose )
	{
		DumpMyArray ();
	}
	printf ( "Successes    %d\n", Successes );
	printf ( "ShuffleCount %d\n", ShuffleCount );


	return;
}
