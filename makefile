PROG = ga_schedule
LIB = ga_schedule.a
XLIB = /usr/local/lib64/shslib.a
DEF = /usr/local/include
DEF1 = ../../include
DEF2 = /usr/local/include
PRG = /usr/local/bin/$(PROG)

FILES = \
	$(LIB)(ga_schedule.o)	\
	$(LIB)(LoadCourses.o)	\
	$(LIB)(LoadClasses.o)	\
	$(LIB)(MakeClasses.o)	\
	$(LIB)(MakeRequests.o)	\
	$(LIB)(MakeSchedule.o)	\
	$(LIB)(init.o)		\
	$(LIB)(report.o)	\
	$(LIB)(obj_func.o)	\
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

