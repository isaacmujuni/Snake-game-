CXXFLAGS = -std=c++17 -Wall -Wextra -O2

snake: snake.cpp
	$(CXX) $(CXXFLAGS) snake.cpp -lncurses -o snake

clean:
	rm -f snake
