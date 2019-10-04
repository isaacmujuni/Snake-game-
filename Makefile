CXXFLAGS = -std=c++17 -Wall -Wextra -O2
CURSES = -lncurses
ifeq ($(OS),Windows_NT)
CURSES = -lpdcurses
endif

snake: snake.cpp
	$(CXX) $(CXXFLAGS) snake.cpp $(CURSES) -o snake

clean:
	rm -f snake
