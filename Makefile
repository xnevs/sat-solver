CXX=g++
CXX_FLAGS=-std=c++11 -O2 -Wall -Wextra -Wshadow -g

satsolve: build/satsolve.o build/SatSolver.o
	$(CXX) $(CXX_FLAGS) -o satsolve build/satsolve.o build/SatSolver.o

build/satsolve.o: src/satsolve.cpp src/SatSolver.hpp
	$(CXX) $(CXX_FLAGS) -c -o build/satsolve.o src/satsolve.cpp

build/SatSolver.o: src/SatSolver.cpp src/SatSolver.hpp
	$(CXX) $(CXX_FLAGS) -c -o build/SatSolver.o src/SatSolver.cpp

.PHONY: clean
clean:
	rm build/*.o
