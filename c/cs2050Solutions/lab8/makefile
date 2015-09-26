all: checker

checker: checker.o utility.o commands.o
	gcc checker.o utility.o commands.o -g -o checker

checker.o: checker.c utility.h commands.h
	gcc checker.c -c -g

commands.o: commands.c commands.h utility.h
	gcc commands.c -c -g

utility.o: utility.c utility.h
	gcc utility.c -c -g

clean:
	rm *.o checker
