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

static int cmpnew ( NEW_RECORD *a, NEW_RECORD *b )
{
	/*----------------------------------------------------------
		sort descending
	----------------------------------------------------------*/
	if ( a->Fitness < b->Fitness )
	{
		return ( 1 );
	}
	return ( -1 );
}

void MakeSchedule ()
{
    int     BestFitness = -1;
    int     ThisFitness;
    int     Consecutive = 0;

	init ();

	for ( GenerationCount = 1; GenerationCount <= MaxGenerations && Consecutive < MaxConsecutive; GenerationCount++ )
	{
		generation ( GenerationCount );
	
		for ( int xp = 0; xp < PopCount; xp++ )
		{
			memcpy ( &CurrPop[xp], &NextPop[xp], sizeof(INDIVIDUAL) );
		}

		ThisFitness = report ( GenerationCount, REPORT_MINMAX );

		if ( ThisFitness < 0 )
		{
			break;
		}

		if ( BestFitness == -1 || BestFitness > ThisFitness )
		{
			BestFitness = ThisFitness;
			Consecutive = 0;
		}

		Consecutive++;

		if ( NewBloodCount > 0 && (GenerationCount%NewBloodCount) == 0 )
		{
			printf ( "Getting %d fresh bloodlines\n", ReplaceCount );

			qsort ( NewArray, PopCount, sizeof(NEW_RECORD), (int(*)()) cmpnew );

			for ( int ndx = 0; ndx < ReplaceCount; ndx++ )
			{
				int		p = NewArray[ndx].Index;

				for ( int c = 0; c < ClassCount; c++ )
				{
					CurrPop[p].Chromosome[c].Period  = random_range ( 1, MAXPERIODS );
				}

				CurrPop[p].Fitness = obj_func ( CurrPop[p].Chromosome, &CurrPop[p].StudentConflicts, &CurrPop[p].TeacherConflicts );
			}
		}

	}
	GenerationCount--;
}
