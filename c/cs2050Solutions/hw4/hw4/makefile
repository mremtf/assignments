all: collector

collector: collector.o items.o utility.o commands.o
	gcc collector.o items.o utility.o commands.o -g -o collector

collector.o: collector.c items.h utility.h commands.h
	gcc collector.c -c -g

items.o: items.c items.h utility.h
	gcc items.c -c -g

commands.o: commands.c commands.h utility.h
	gcc commands.c -c -g

utility.o: utility.c utility.h
	gcc utility.c -c -g

clean:
	rm *.o collector
