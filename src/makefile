FLAGS=-g -ggdb -Wall -Werror -Wextra -std=gnu99 -D_GNU_SOURCE
LIBS=-lpthread -lrt

jackpot: main.c wheel.o display.o timer.o controller.o game_state.h
	gcc $(FLAGS) -o jackpot main.c game_state.h *.o $(LIBS)

wheel.o: wheel.c wheel.h
	gcc $(FLAGS) -c wheel.c

controller.o: controller.c controller.h
	gcc $(FLAGS) -c controller.c

game_state.o: game_state.c game_state.h
	gcc $(FLAGS) -c game_state.c

timer.o: timer.c timer.h
	gcc $(FLAGS) -c timer.c


clean:
	rm jackpot *.o