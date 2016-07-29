
OBJECTS := main.o asteroid.o blast.o nave.o

Blastoid: $(OBJECTS)
	gcc $(OBJECTS) -o Blastoid	-lm	`allegro-config --libs` &&./Blastoid
	
	@echo "Compilado mi amo"

main.o: main.c main.h
	gcc main.c -c 

asteroid.o: asteroid.c asteroid.h
	gcc asteroid.c -c

blast.o: blast.c blast.h
	gcc blast.c -c

nave.o: nave.c nave.h
	gcc nave.c -c

clean:
	rm *.o Blastoid
