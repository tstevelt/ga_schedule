
## High School Class Scheduling

### Current Usage Message
```
USAGE: ga_schedule.v5 mode [options]
  1 = make requests from students and courses
  2 = make classes from requests
  3 = make schedule using GA
  4 = students pick classes
  5 = print schedules
Options
  -pop #     - mode 3, population count (default 500)
  -probX #.# - mode 3, probability of crossover (default 0.9950)
  -probM #.# - mode 3, probability of mutation (default 0.0500)
  -maxgen #  - mode 3, maximum generations (default 500)
  -same #    - mode 3, if no improvment after # generations (default 100)
  -conflicts # - mode 3, stop if teacher conflicts less than # (default 1)
  -v         - verbose
```

After considerable experimentation, I realized that student conflicts are dependent on teacher conflict.  Since the class list (classses.TXT) contains enought classes per course to not exceed 'Maximum Students Per Class', we only need to eliminate all teacher confilcts in order to minimize studend conflicts.  Therefore, the 'Pure GA Solutions' I was persuing is not necessary. Huge speed increase!

The courses.CSV file eliminated the 4 column 'TEACHERS'.  Each course has a unique teacher.  If your school has two English teachers, then you will need two English courses (for example '101 Miss Jones English' and '102 Miss Smitth English).

Read my article on Medium <a href='https://medium.com/gitconnected/high-school-class-scheduling-problem-70d0077ae2ac'>High School Class Scheduling Problem</a>

### Version history

#### version 0.1.5
* Changed executable name to ga_schedule.v5
* Cleanup of version 0.1.3
* Removed Teachers field from the COURSE_RECORD structure.
* Removed LoadRequests() and associated structures, since no longer calculating student conflicts.
* All phases (modes) can be run with this version.
* Additional informational lines output to schedule_chromosome.TXT by mode 3. Ignored by mode 4.

#### version 0.1.4
* Experimental version, not released.

#### version 0.1.3
* Changed executable name to ga_schedule.v3
* Remove student conflcts. By eliminating student conflicts from the GA scoring function, program now generates a class schedule with ZERO teacher conflicts. This is achieved quickly, usually in less than 100 generations.
* Added option 4 to assign students to class schedule created with option 3.
* Added final reports (option 5), classes_detail.TXT and schedule_students.TXT

#### version 0.1.2
* Changed executable name to ga_schedule.v2
* Added teacher conflicts. Realized that previous version might overload courses into a period, showing no regard for how many teachers could teach that course.  Program slowed down a bit as expected, but never achieved a suitable low level of conflicts.

#### version 0.1.1
* Changed executable name to ga_schedule.v1
* Initial attempt to schedule classes for xxx courses and 500 students, using GA to minimize student conflicts.  Starts with approx 1200 conflicts and after about an hour reduced that to less than 100 conflicts.


### Original Release

The day was to be split into 7 periods. The students all submitted requests for 2 or 3 electives, this gives some flexibility.
* 500 students
* 4 years
* 30 classrooms, including a science lab
* Maximum of 20 students per classroom
* A gymnasium and a study hall
* 4 required courses and 2 electives per year (freshmen, sophomore, etc)

Some checking and preparation is needed.
* 500 divided by 32 equals 16 students per classroom, so we have enough classrooms.
* Assume 125 students per year, round up to 140 for flexibility. So are required course will need 140 divided by 20 equals 7 classes for each subject. For example, ENGLISH 1, ENGLISH 2, ENGLISH 3 and so on.
* Assume each student has 5 electives to choose from. This means that we need 10 classes for electives. ( 40% of 500 is 200 students per elective for a total of 10 classes for all 4 years).
* Total number of classes to for each day is 7 times 4 times 7 equal 112 required plus 10 electives equals 122 classes. Double checking classroom requirement, 122 divided by 7 is 18 classrooms, so we have enough classrooms.
* Given that there is only one science lab, we're limited to 140 students in the science track.

The source code for  my not-a-GA attempt was written in Clipper for a dBase database. And is currently inaccessible on 5-1/4" floppy disks.


