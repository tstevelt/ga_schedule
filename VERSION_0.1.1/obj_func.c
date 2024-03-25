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

static int DebugObjFunc = 0;

int obj_func ( ALLELE Chromosome [] )
{
	int		Students = 0;
	int		Total = 0;
	int		PeriodArray[MAXPERIODS];
	int		xp, PeriodCount;

	/*------------------------------------------------------------------------------
		tally conflicts for each student, add to StudentConflicts
	------------------------------------------------------------------------------*/
	for ( int RequestIndex = 0; RequestIndex < RequestCount; RequestIndex++ )
	{
		if ( DebugObjFunc )
		{
			printf ( "---\n" );
		}
		PeriodCount = 0;
		memset ( PeriodArray, '\0', sizeof(PeriodArray) );

		for ( int RequestClassIndex = 0; RequestClassIndex < RequestArray[RequestIndex].ClassCount; RequestClassIndex++ )
		{
			for ( int ChromoIndex = 0; ChromoIndex < ClassCount; ChromoIndex++ )
			{
				if ( ChromoIndex == RequestArray[RequestIndex].ClassIndex[RequestClassIndex] )
				{
					for ( xp = 0; xp < PeriodCount; xp++ )
					{
						if ( PeriodArray[xp] == Chromosome[ChromoIndex].Period )
						{
							break;
						}
					}
					if ( xp >= PeriodCount )
					{
						 PeriodArray[PeriodCount] = Chromosome[ChromoIndex].Period;
						 PeriodCount++;
					}
				}
			}
		}
		
		Students += (RequestArray[RequestIndex].ClassCount - PeriodCount);
	}

	Total = Students;

	return ( Total );
}
