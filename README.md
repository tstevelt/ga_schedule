
Catholic Girls High School Class Scheduling
Creating a block schedule for a high school was something I struggled with from 1987 to 1989. The closest I ever got to a solution came fine tuned the loose ends by programmatically throwing darts at a dartboard. It wasn't until years later I heard of GA and thought that might have worked. However, I was two jobs along and never tried to do it. I'll leave it as a challenge for motivated readers. 
Here's the setup. 
The day was to be split into 7 periods. The students all submitted requests for 2 or 3 electives, this gives some flexibility.
500 students
4 years
30 classrooms, including a science lab
Maximum of 20 students per classroom
A gymnasium and a study hall
4 required courses and 2 electives per year (freshmen, sophomore, etc)

Some checking and preparation is needed.
500 divided by 32 equals 16 students per classroom, so we have enough classrooms.
Assume 125 students per year, round up to 140 for flexibility. So are required course will need 140 divided by 20 equals 7 classes for each subject. For example, ENGLISH 1, ENGLISH 2, ENGLISH 3 and so on.
Assume each student has 5 electives to choose from. This means that we need 10 classes for electives. ( 40% of 500 is 200 students per elective for a total of 10 classes for all 4 years).
Total number of classes to for each day is 7 times 4 times 7 equal 112 required plus 10 electives equals 122 classes. Double checking classroom requirement, 122 divided by 7 is 18 classrooms, so we have enough classrooms.
Given that there is only one science lab, we're limited to 140 students in the science track.

My assignment was to place all the classes into 7 blocks such that everyone got what they needed and there were no (minimum) conflicts. No student could be in more than one classroom at the same time.
As with designing any GA program, the challenge is figuring out the chromosome structure and the scoring function. Here are my thoughts on the chromosome:
typedef struct
{
  int  period;  // 1 - 7
  int  classID; // 1 - 122
} ALLELE;

typedef struct
{
  ALLELE Chromosome[122]; // see calculation in article
  int    Conflicts;       // student is in more than one class at a time
  int    Missing;         // student's class is not represented.
  int    Fitness;         // total of Conflicts + Missing
} INDIVIUDAL;

Scoring function seems to be the challenge, seems that a brute force method is needed to find Conflicts and Missing for each Individual in the Population every generation.
The source code for  my not-a-GA attempt was written in Clipper for a dBase database. And is currently inaccessible on 5-1/4" floppy disks.
