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

static void Usage ()
{
	printf ( "USAGE: ga_schedule mode [options]\n" );
	printf ( "  1 = make requests from students and courses\n" );
	printf ( "  2 = make classes from requests\n" );
	printf ( "  3 = make schedule using GA\n" );
	printf ( "  4 = students pick classes\n" );
	printf ( "  5 = print schedules\n" );
	printf ( "Options\n" );
	printf ( "  -pop #     - mode 3, population count (default %d)\n", PopCount );
	printf ( "  -probX #.# - mode 3, probability of crossover (default %.4f)\n", ProbCross );
	printf ( "  -probM #.# - mode 3, probability of mutation (default %.4f)\n", ProbMutate );
	printf ( "  -maxgen #  - mode 3, maximum generations (default %d)\n", MaxGenerations );
	printf ( "  -conflicts # - mode 3, stop if teacher conflicts less than #\n" );
	printf ( "  -v         - verbose\n" );
	exit ( 1 );

}

void getargs ( int argc, char *argv[] )
{
	RunMode = 0;
	Verbose = 0;
	PopCount = 500;
	ProbCross = 0.995;
	ProbMutate = 0.05;
	MaxGenerations =  500;
	TeacherStop = -1;

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
		else if ( argv[xa][0] == '4' )
		{
			RunMode = MODE_ASSIGN;
		}
		else if ( argv[xa][0] == '5' )
		{
			RunMode = MODE_PRINT;
		}
		else if ( strcmp ( argv[xa], "-v" ) == 0 )
		{
			Verbose = 1;
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-pop" ) == 0 )
		{
			xa++;
			PopCount = atoi ( argv[xa] );
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-probX" ) == 0 )
		{
			xa++;
			ProbCross = atof ( argv[xa] );
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-probM" ) == 0 )
		{
			xa++;
			ProbMutate = atof ( argv[xa] );
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-maxgen" ) == 0 )
		{
			xa++;
			MaxGenerations = atoi ( argv[xa] );
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-conflicts" ) == 0 )
		{
			xa++;
			TeacherStop = atoi ( argv[xa] );
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
