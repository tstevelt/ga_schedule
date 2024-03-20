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

static void PrintChromo ( char *description, int xsite, ALLELE Chromosome[] )
{
	return;

	printf ( "%s:", description );

	for ( int xc = 0; xc < ClassCount; xc++ )
	{
		if ( xc == xsite )
		{
			printf ( " " );
		}

		printf ( "%d", Chromosome[xc].Period );
	}
	printf ( "\n" );
	fflush ( stdout );
}

void generation ( int Generation )
{
	int		xi, mate1, mate2, xsite;
	ALLELE	SaveMe;
	int		xs, xd;

	for ( xi = 0; xi < PopCount; xi += 2 )
	{
		mate1 = select_shuffle ();
		mate2 = select_shuffle ();

		xsite = crossover ( CurrPop[mate1].Chromosome, CurrPop[mate2].Chromosome, 
							NextPop[xi   ].Chromosome, NextPop[xi+1 ].Chromosome );

		PrintChromo ( "parent1", xsite, CurrPop[mate1].Chromosome );
		PrintChromo ( "parent2", xsite, CurrPop[mate2].Chromosome );
		PrintChromo ( "child 1", xsite, NextPop[xi].Chromosome );
		PrintChromo ( "child 2", xsite, NextPop[xi+1].Chromosome );

		if ( flip ( ProbMutate ) )
		{
			xs = random_range ( 0, ClassCount - 1 );

			do 
			{
				xd = random_range ( 0, ClassCount - 1 );

			} while ( xd == xs );

			memcpy ( &SaveMe, &NextPop[xi].Chromosome[xs], sizeof(ALLELE) );
			NextPop[xi].Chromosome[xs].Period = NextPop[xi].Chromosome[xd].Period;
			NextPop[xi].Chromosome[xd].Period = SaveMe.Period;
		}

		NextPop[xi].Fitness   = obj_func ( NextPop[xi].Chromosome );
		NextPop[xi+1].Fitness = obj_func ( NextPop[xi+1].Chromosome );
	}
}
