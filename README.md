
## Catholic Girls High School Class Scheduling

### Here's the setup.

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

### Current Usage Message
```
USAGE: ga_schedule mode [options]
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
  -conflicts # - mode 3, stop if teacher conflicts less than #
  -v         - verbose
```

### Version history

#### version 0.1.3
	Remove student conflcts. By eliminating student conflicts from the GA scoring function, program now generates a class schedule with ZERO teacher conflicts. This is achieved quickly, usually in less than 100 generations.

	Added option 4 to assign students to class schedule created with option 3.

	Added final reports (option 5), classes_detail.TXT and schedule_students.TXT

#### version 0.1.2
	Added teacher conflicts. Realized that previous version might overload courses into a period, showing no regard for how many teachers could teach that course.  Program slowed down a bit as expected, but never achieved a suitable low level of conflicts.

#### version 0.1.1
	Initial attempt to schedule classes for xxx courses and 500 students, using GA to minimize student conflicts.  Starts with approx 1200 conflicts and after about an hour reduced that to less than 100 conflicts.



