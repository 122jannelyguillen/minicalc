CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

minicalc: src/main.cpp src/lexer.cpp src/parser.cpp src/token.cpp src/lexer.h src/parser.h src/token.h
	$(CXX) $(CXXFLAGS) -o minicalc src/main.cpp src/lexer.cpp src/parser.cpp src/token.cpp

test: minicalc
	bash run_tests.sh

clean:
	rm -f minicalc minicalc.exe
