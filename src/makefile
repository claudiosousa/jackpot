FLAGS=-g -ggdb -Wall -Werror -Wextra -std=gnu99 -D_GNU_SOURCE
LIBS=-lpthread -lrt

jackpot: main.c controller.o wheel.o display.o game_state.h timer.o
	gcc $(FLAGS) -o jackpot main.c game_state.h *.o $(LIBS)

controller.o: controller.c controller.h wheel.h display.h game_state.h
	gcc $(FLAGS) -c controller.c

wheel.o: wheel.c wheel.h game_state.h timer.h
	gcc $(FLAGS) -c wheel.c

display.o: display.c display.h game_state.h timer.h
	gcc $(FLAGS) -c display.c

game_state.o: game_state.c game_state.h
	gcc $(FLAGS) -c game_state.c

timer.o: timer.c timer.h
	gcc $(FLAGS) -c timer.c


clean:
	rm jackpot *.o
