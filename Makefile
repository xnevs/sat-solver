CXX=g++
CXX_FLAGS=-std=c++11 -O2 -march=native -mtune=native -Wall -Wextra -Wshadow

satsolve: build/satsolve.o build/SatSolver.o
	$(CXX) $(CXX_FLAGS) -o satsolve build/satsolve.o build/SatSolver.o

build/satsolve.o: src/satsolve.cpp src/SatSolver.hpp build
	$(CXX) $(CXX_FLAGS) -c -o build/satsolve.o src/satsolve.cpp

build/SatSolver.o: src/SatSolver.cpp src/SatSolver.hpp build
	$(CXX) $(CXX_FLAGS) -c -o build/SatSolver.o src/SatSolver.cpp

build:
	mkdir build

.PHONY: clean
clean:
	rm build/*.o
