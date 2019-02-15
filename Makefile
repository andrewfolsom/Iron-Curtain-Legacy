LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm

all: test

test: iron_curtain.cpp timers.cpp nickJA.cpp BenjaminG.cpp ChadM.cpp andrewF.cpp spencerA.cpp
	g++ iron_curtain.cpp timers.cpp nickJA.cpp ChadM.cpp andrewF.cpp spencerA.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o test

clean:
	rm -f test
	rm -f *.o
