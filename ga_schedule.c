/*----------------------------------------------------------------------------
	Program : ga_schedule.c
	Author  : Tom Stevelt
	Date    : Mar 2024
	Synopsis: Use GA to create high school schedule

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/

#define		MAIN
#include	"ga_schedule.h"

int main ( int argc, char *argv[] )
{
	getargs ( argc, argv );

	seed_random_with_usec ( );

	switch ( RunMode )
	{
		case MODE_REQUESTS:
			MakeRequests ();
			break;

		case MODE_CLASSES:
			MakeClasses ();
			break;

		case MODE_SCHEDULE:
			MakeSchedule ();
			break;
	}

	return ( 0 );
}
