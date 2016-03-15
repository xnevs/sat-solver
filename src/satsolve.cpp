#include <iostream>
#include <fstream>
#include <limits>

#include <string>
#include <vector>
#include <set>

#include <cstdlib>

using namespace std;

string help =
"usage: satsolve filename";

vector<set<int>> formula;
vector<int> valuation;

void read_dimacs(istream &in) {
    int nbvar,nbclauses;

    string s;

    //ignore comments
    while(in.peek() == 'c') {
        in.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    //read number of vars and clauses
    in >> s >> s >> nbvar >> nbclauses;

    formula.resize(nbclauses);

    //read formula
    int clause = 0;
    int literal;
    while(in >> literal) {
        if(literal == 0) {
            clause++;
        } else {
            formula[clause].insert(literal);
        }
    }
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        cout << help << endl;
    }

    ifstream ifile(argv[1]);
    read_dimacs(ifile);
    ifile.close();

    for(auto clause : formula) {
        for(auto var : clause) {
            cout << var << " ";
        }
        cout << endl;
    }

    for(auto i : {1,2,3,4})
        cout << i << endl;
}
