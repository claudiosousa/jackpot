FLAGS=-g -Wall -Werror -Wextra -std=gnu99 -D_GNU_SOURCE
LIBS=-lpthread -lrt

jackpot: main.c wheel.o display.o timer.o controller.o
	gcc $(FLAGS) -o jackpot main.c *.o $(LIBS)

wheel.o: wheel.c wheel.h
	gcc $(FLAGS) -c wheel.c

display.o: display.c display.h
	gcc $(FLAGS) -c display.c

controller.o: controller.c controller.h
	gcc $(FLAGS) -c controller.c

timer.o: timer.c timer.h
	gcc $(FLAGS) -c timer.c


clean:
	rm jackpot *.o