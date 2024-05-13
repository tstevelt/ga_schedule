PROG = ga_schedule
LIB = ga_schedule.a
XLIB = /usr/local/lib64/shslib.a
DEF = /usr/local/include
PRG = /usr/local/bin/$(PROG).v3

FILES = \
	$(LIB)(ga_schedule.o)	\
	$(LIB)(LoadCourses.o)	\
	$(LIB)(LoadClasses.o)	\
	$(LIB)(LoadRequests.o)	\
	$(LIB)(LoadStudents.o)	\
	$(LIB)(MakeClasses.o)	\
	$(LIB)(MakeRequests.o)	\
	$(LIB)(MakeSchedule.o)	\
	$(LIB)(PrintSchedule.o)	\
	$(LIB)(PrintRosters.o)	\
	$(LIB)(AssignStudents.o)	\
	$(LIB)(init.o)			\
	$(LIB)(report.o)		\
	$(LIB)(obj_func.o)		\
	$(LIB)(select_shuffle.o)	\
	$(LIB)(generation.o)	\
	$(LIB)(crossover.o)		\
	$(LIB)(getargs.o)

.SILENT:

$(PRG): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG)"
	gcc -o $(PRG) $(LIB) $(XLIB)

$(LIB): $(FILES)

$(FILES): ga_schedule.h $(DEF)/shslib.h

clean:
	rm -f $(LIB)

all:
	make clean
	make

.PRECIOUS: $(LIB)

