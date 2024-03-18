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

void init ()
{
	if (( CurrPop = calloc ( PopCount, sizeof(INDIVIUDAL) )) == NULL )
	{
		printf ( "init: calloc CurrPop failed, %s\n", strerror(errno) );
		exit ( 1 );
	}

	if (( NextPop = calloc ( PopCount, sizeof(INDIVIUDAL) )) == NULL )
	{
		printf ( "init: calloc NextPop failed, %s\n", strerror(errno) );
		exit ( 1 );
	}

	LoadClasses ();

	for ( int p = 0; p < PopCount; p++ )
	{
		for ( int c = 0; c < ClassCount; c++ )
		{
			CurrPop[p].Chromosome[c].period  = random_range ( 1, MAXPERIODS );
			CurrPop[p].Chromosome[c].classIndex  = c;
		}
	}

	report ( 0, REPORT_ALL );

}
