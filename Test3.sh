#!/bin/sh

COUNT=0

while true
do
	ga_schedule 3 -conflicts 1 | grep -v Minimum
	COUNT=`expr $COUNT + 1`
	mv schedule_chromosome.TXT schedule_chromosome.TXT.$COUNT
	if [ $COUNT = 100 ]
	then
		break
	fi
done
