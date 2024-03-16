
#include	"ga_schedule.h"

#define		MAXNAME		30

typedef struct
{
	int		ID;
	int		Level;
	int		Required;
	char	Name[MAXNAME];
} COURSE_RECORD;

#define		MAXCOURSES		100

static	COURSE_RECORD	CourseArray[MAXCOURSES];
static	int				CourseCount = 0;

typedef struct
{
	int		ID;
	int		Level;
	char	Name[MAXNAME];
} STUDENT_RECORD;

static	STUDENT_RECORD	Student;
static	int				StudentCount = 0;

void MakeRequests ()
{
	FILE	*ifp, *ofp;
	char	buffer[1024];
	char	*tokens[10];
	int		tokcnt;
	int		lineno;
	int		xe = 0;
	int		xo;

	if (( ofp = fopen ( "requests.TXT", "w" )) == NULL )
	{
		printf ( "Cannot create requests.TXT\n" );
		exit ( 1 );
	}

	fprintf ( ofp, "# these are fake student requests for testing\n" );

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

	if (( ifp = fopen ( "students.TXT", "r" )) == NULL )
	{
		printf ( "Cannot open students.TXT\n" );
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
	lineno = 0;
	while ( fgets ( buffer, sizeof(buffer), ifp ) != NULL )
	{
		lineno++;

		if (( tokcnt = GetTokensD ( buffer, ",\n\r", tokens, 10 )) < 3 )
		{
			continue;
		}
		Student.ID = atoi ( tokens[0] );
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
		StudentCount++;

		fprintf ( ofp, "%3d,%2d", Student.ID, Student.Level );

		/*----------------------------------------------------------
			print required courses for student's grade level
		----------------------------------------------------------*/
		for ( int ndx = 0; ndx < CourseCount; ndx++ )
		{
			if ( CourseArray[ndx].Level == Student.Level && CourseArray[ndx].Required == 1 )
			{
				fprintf ( ofp, ",%4d", CourseArray[ndx].ID );
			}
		}

		/*----------------------------------------------------------
			print three electives for grade level or 5xx level 0
		----------------------------------------------------------*/
		for ( xo = 0; xo < 3; )
		{
			if (( CourseArray[xe].Required == 0 ) &&
				( CourseArray[xe].Level == 0 || CourseArray[xe].Level == Student.Level ))
			{
				fprintf ( ofp, ",%4d", CourseArray[xe].ID );
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

	printf ( "Loaded %d students\n", StudentCount );
	fclose ( ifp );

	fclose ( ofp );
}
