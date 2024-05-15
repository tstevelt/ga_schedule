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

int crossover ( ALLELE parent1[], ALLELE parent2[],
			  	ALLELE child1[], ALLELE child2[] )
{
	int		cross_site;
	int		xi;

	if ( flip ( ProbCross ) )
	{
		cross_site = (int) random_range ( 0, ClassCount - 2 );
	}
	else
	{
		cross_site = ClassCount;
	}

	/*---------------------------------------------------------------------------
		typedef struct
		{
			int	Period;
		} ALLELE;

		From start to site-1, children are the same as their parents
		From site to end, children get the other parent's period
	---------------------------------------------------------------------------*/

	for ( xi = 0; xi < cross_site; xi++ )
	{
		child1[xi].Period = parent1[xi].Period;
		child2[xi].Period = parent2[xi].Period;
	}

	for ( ; xi < ClassCount; xi++ )
	{
		child1[xi].Period = parent2[xi].Period;
		child2[xi].Period = parent1[xi].Period;
	}

	return ( cross_site );

}
