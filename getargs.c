
#include	"ga_schedule.h"

static void Usage ()
{
	printf ( "USAGE: ga_schedule mode\n" );
	printf ( "  1 = make requests from students and courses\n" );
	printf ( "  2 = make classes from requests\n" );
	printf ( "  3 = make schedule using GA\n" );
}

void getargs ( int argc, char *argv[] )
{
	RunMode = 0;

	for ( int xa = 1; xa < argc; xa++ )
	{
		if ( argv[xa][0] == '1' )
		{
			RunMode = MODE_REQUESTS;
		}
		else if ( argv[xa][0] == '2' )
		{
			RunMode = MODE_CLASSES;
		}
		else if ( argv[xa][0] == '3' )
		{
			RunMode = MODE_SCHEDULE;
		}
		else
		{
			Usage ();
		}
	}
	if ( RunMode == 0 )
	{
		Usage ();
	}
}
