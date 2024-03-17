//     GA Schedule
// 
//     Copyright (C)  2024 Tom Stevelt
// 
//     This program is free software: you can redistribute it and/or modify
//     it under the terms of the GNU Affero General Public License as
//     published by the Free Software Foundation, either version 3 of the
//     License, or (at your option) any later version.
// 
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU Affero General Public License for more details.
// 
//     You should have received a copy of the GNU Affero General Public License
//     along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
