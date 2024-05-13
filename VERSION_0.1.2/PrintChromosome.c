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

void PrintChromosome ()
{
	FILE	*ofp;

	if (( ofp = fopen ( "schedule_chromosome.TXT", "w" )) == NULL )
	{
		printf ( "Cannot create schedule_chromosome.TXT\n" );
		exit ( 1 );
	}

#define PRINT_BEST_ONLY
#ifdef PRINT_BEST_ONLY
	for ( int xc = 0; xc < ClassCount; xc++ )
	{
		fprintf ( ofp, "%d", BestIndividual.Chromosome[xc].Period );
	}
	fprintf ( ofp, "\n" );
#else
	for ( int xp = 0; xp < PopCount; xp++ )
	{
		for ( int xc = 0; xc < ClassCount; xc++ )
		{
			fprintf ( ofp, "%d", CurrPop[xp].Chromosome[xc].Period );
		}
		fprintf ( ofp, "\n" );
	}
#endif
	
	fclose ( ofp );
}

