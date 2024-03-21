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

void report ( int Generation, int mode )
{
	int		BestIndex, MinFit, MaxFit;

	BestIndex = 0;
	MinFit = MaxFit = CurrPop[0].Fitness;
	for ( int p = 0; p < PopCount; p++ )
	{
		if ( MinFit > CurrPop[p].Fitness )
		{
			MinFit = CurrPop[p].Fitness;
			BestIndex = p;
		}
		if ( MaxFit < CurrPop[p].Fitness )
		{
			MaxFit = CurrPop[p].Fitness;
		}
	}

	/*---------------------------------------------------------------------------
		TYPE	INDIVIDUAL		BestIndividual;
		TYPE	int				BestFitness;
	---------------------------------------------------------------------------*/
	if ( BestFitness == -1 || BestFitness > MinFit )
	{
		BestFitness = MinFit;
		memcpy ( &BestIndividual, &CurrPop[BestIndex], sizeof(INDIVIDUAL) );
		if ( mode == REPORT_MINMAX )
		{
			printf ( "Generation %d: New Minimum %d\n", Generation, MinFit );
		}
	}

	switch ( mode )
	{
		case REPORT_ALL:
			printf ( "Generation %d\n", Generation );
			for ( int p = 0; p < PopCount; p++ )
			{
				if ( mode == REPORT_ALL )
				{
					printf ( "%4d:", p );
					for ( int c = 0; c < ClassCount; c++ )
					{
						printf ( " %d", CurrPop[p].Chromosome[c].Period );
					}
					printf ( " = %d\n", CurrPop[p].Fitness );
				}
			}
			printf ( "Minimum %d, Maximum %d\n", MinFit, MaxFit );
			fflush ( stdout );
			break;

		case REPORT_MINMAX:
			printf ( "Generation %d: Minimum %d, Maximum %d\n", Generation, MinFit, MaxFit );
			fflush ( stdout );
			break;
	}

}
