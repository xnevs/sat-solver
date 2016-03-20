#include <iostream>
#include <fstream>

#include "SatSolver.hpp"

using namespace std;

string help =
"usage: satsolve filename";


int main(int argc, char *argv[]) {
    if(argc < 2) {
        cout << help << endl;
    }

    SatSolver solver;

    ifstream ifile(argv[1]);
    if(ifile.is_open()) {
        solver.read_dimacs(ifile);
        ifile.close();

        bool satisfiable = solver.solve();

        if(satisfiable) {
            for(int lit : solver.solution)
                cout << lit << " ";
            cout << endl;
        } else {
            cout << "The formula is NOT satisfiable." << endl;
        }
    } else {
        cout << "Error opening file." << endl;
    }
}
