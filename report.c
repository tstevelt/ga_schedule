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

void report ( int generation, int mode )
{
	printf ( "Generation %d\n", generation );

	for ( int p = 0; p < PopCount; p++ )
	{
		printf ( "%4d:", p );
		for ( int c = 0; c < ClassCount; c++ )
		{
			printf ( " (%s %d)",
					ClassArray[CurrPop[p].Chromosome[c].classIndex].ClassCode,
					CurrPop[p].Chromosome[c].period );
		}
		printf ( " = %d\n", CurrPop[p].Fitness );
	}
}
