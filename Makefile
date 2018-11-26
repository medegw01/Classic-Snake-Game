all: snake.o termfuncs.o
	clang++ -Wall -Wextra snake.o termfuncs.o -o snake11
	chmod g+rwx snake11

snake.o: snake.cpp snake.h termfuncs.h
	clang++ -c -Wall -Wextra snake.cpp
	chmod g+rw snake.o
	
termfuncs.o: termfuncs.cpp termfuncs.h
	clang++ -c -Wall -Wextra termfuncs.cpp
	chmod g+rw termfuncs.o

clean:
	rm *.o snake11
