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
	int    Index;
	double  RandomValue;
} SHUFFLE_RECORD;

static  SHUFFLE_RECORD  *shufflearray;
static  int		pick;
static	int		firstpass = 1;

static int cmpshuffle ( SHUFFLE_RECORD *a, SHUFFLE_RECORD *b )
{
	if ( a->RandomValue < b->RandomValue )
	{
		return ( -1 );
	}
	if ( a->RandomValue > b->RandomValue )
	{
		return ( 1 );
	}

	return ( 0 );
}

static void makeshuffle ()
{
	pick = 0;

	for ( int xi = 0; xi < PopCount; xi++ )
	{
		shufflearray[xi].Index = xi;
		shufflearray[xi].RandomValue = d_random ();
	}

	qsort ( shufflearray, PopCount, sizeof(SHUFFLE_RECORD), (int(*)()) cmpshuffle );
}

int select_shuffle () 
{
	if ( firstpass == 1 )
	{
		firstpass = 0;
		shufflearray = calloc ( PopCount, sizeof(SHUFFLE_RECORD) );
		pick = PopCount + 3;
	}

	if ( pick + 1 >= PopCount )
	{
		makeshuffle ();
	}

	int first  = shufflearray[pick++].Index;
	int second = shufflearray[pick++].Index;

	if ( CurrPop[first].Fitness <= CurrPop[second].Fitness )
		return ( first );
	else
		return ( second ); 
}

