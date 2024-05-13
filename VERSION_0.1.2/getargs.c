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
	printf ( "Options\n" );
	printf ( "  -pop #     - mode 3, population count (default %d)\n", PopCount );
	printf ( "  -probX #.# - mode 3, probability of crossover (default %.4f)\n", ProbCross );
	printf ( "  -probM #.# - mode 3, probability of mutation (default %.4f)\n", ProbMutate );
	printf ( "  -load S|R  - mode 3, load students S static or R random (default %c)\n", LoadType );
	printf ( "  -maxgen #  - mode 3, maximum generations (default %d)\n", MaxGenerations );
	printf ( "  -same #    - mode 3, if no improvment after # generations (default %d)\n", MaxConsecutive );
	printf ( "  -new # #   - mode 3, every # replace worst # percent (default %d %d)\n", NewBloodCount, NewBloodPercent );
	printf ( "  -conflicts # # - mode 3, stop if student conflicts less than #1 and teacher conflicts less than #2\n" );
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
	MaxConsecutive = MaxGenerations / 2;
	StudentStop = -1;
	TeacherStop = -1;
	LoadType = USE_RANDOMIZE;
	NewBloodCount = NewBloodPercent = 0;

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
		else if ( strcmp ( argv[xa], "-v" ) == 0 )
		{
			Verbose = 1;
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-pop" ) == 0 )
		{
			xa++;
			PopCount = atoi ( argv[xa] );
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-load" ) == 0 )
		{
			xa++;
			LoadType = toupper(argv[xa][0]);
			switch ( LoadType )
			{
				case USE_RING_INDEX:
				case USE_RANDOMIZE:
					break;
				default:
					Usage ();
			}
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
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-same" ) == 0 )
		{
			xa++;
			MaxConsecutive = atoi ( argv[xa] );
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-maxgen" ) == 0 )
		{
			xa++;
			MaxGenerations = atoi ( argv[xa] );
		}
		else if ( xa + 2 < argc && strcmp ( argv[xa], "-new" ) == 0 )
		{
			xa++;
			NewBloodCount = atoi ( argv[xa] );
			xa++;
			NewBloodPercent = atoi ( argv[xa] );
		}
		else if ( xa + 2 < argc && strcmp ( argv[xa], "-conflicts" ) == 0 )
		{
			xa++;
			StudentStop = atoi ( argv[xa] );
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
