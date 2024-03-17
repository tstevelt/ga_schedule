/*----------------------------------------------------------------------------
	Program : ga_schedule.c
	Author  : Tom Stevelt
	Date    : Mar 2024
	Synopsis: Use GA to create high school schedule

	Who		Date		Modification
	---------------------------------------------------------------------

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
