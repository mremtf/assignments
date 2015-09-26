all: garbageman

garbageman: utilities.o items.o commands.o garbageman.o game_engine.o
	gcc items.o comamnds.o garbageman.o game_engine.o -o garbageman

garbageman.o: garbageman.c items.h enemies.h commands.h utilities.h
	gcc garbageman.c -c 
	
items.o: items.c items.h
	gcc items.c -c
	
game_engine.o: game_engine.c game_engine.h
	gcc game_engine.c -c
	
commands.o: commands.c commands.h
	gcc commands.c -c

utilities.o: utilities.c utilities.h
	gcc utilities.c -c 

clean:
	rm *.o garbageman
