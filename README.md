SAT solver
==========

Building
--------

A C++ compiler with C++11 support is required. The default compiler is `g++`. If you wish to use a different compiler change the `CXX` variable in the `Makefile`.

To build the project run `make` in the root directory of the project.

The result of building is the `satsolver` executable.

Running
-------

To run the solver on a DIMACS formatted input file (e.g. `example.txt`) run `./satsolver` with the filename as the only argument.
```
./satsolve example.txt
```

The program will print a satisfying valuation to standard output or print `The formula in NOT satisfiable.` if there is none.
