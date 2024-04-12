/*----------------------------------------------------------------------------
	Program : ga_schedule.c
	Author  : Tom Stevelt
	Date    : Mar 2024
	Synopsis: Use GA to create high school schedule

	Who		Date		Modification
	---------------------------------------------------------------------
	tms		03/22/2024	Added number of teachers to courses.CSV, etc.

----------------------------------------------------------------------------*/
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

#define		MAIN
#include	"ga_schedule.h"

int main ( int argc, char *argv[] )
{
	time_t	StartTime, EndTime;
	int		Elapsed, Hours, Minutes, Seconds;
	double	Average;

	getargs ( argc, argv );

	shs_seed_random ( );

	switch ( RunMode )
	{
		case MODE_REQUESTS:
			MakeRequests ();
			break;

		case MODE_CLASSES:
			MakeClasses ();
			break;

		case MODE_SCHEDULE:
			time ( &StartTime );
			MakeSchedule ();
			PrintSchedule ();
			PrintStudents ();
			time ( &EndTime );
			Elapsed = EndTime - StartTime;
			Average = (double) Elapsed / (double) GenerationCount;
			Hours = Elapsed / 3600;
			if ( Hours )
			{
				Elapsed -= Hours * 3600;
			}
			Minutes = Elapsed / 60;
			Seconds = Elapsed % 60;
			if ( Hours )
			{
				printf ( "runtime: %d hours %d minutes %d seconds\n", Hours, Minutes, Seconds );
			}
			else if ( Minutes )
			{
				printf ( "runtime: %d minutes %d seconds\n", Minutes, Seconds );
			}
			else
			{
				printf ( "runtime: %d seconds\n", Seconds );
			}
			printf ( "average per generation %.4f\n", Average );
			break;
	}

	return ( 0 );
}
