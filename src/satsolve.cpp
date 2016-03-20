#include <iostream>

#include <fstream>
#include <limits>

#include <list>
#include <vector>
#include <deque>
#include <set>
#include <map>

#include <algorithm>

#include <cstdlib>

using namespace std;

string help =
"usage: satsolve filename";

typedef set<int> Clause;
vector<Clause> formula;
vector<vector<int>> vars;

set<int> unsimplified;

map<int,int> unit_count;
set<int> units;

void found_unit(int lit) {
    int cnt = unit_count[lit] + 1;
    if(cnt == 1)
        units.insert(lit);
    unit_count[lit] = cnt;
}

void destroyed_unit(int lit) {
    int cnt = unit_count[lit] - 1;
    if(cnt == 0) {
        units.erase(lit);
    }
    unit_count[lit] = cnt;
}

map<int,int> satisfied_count;
int unsatisfied = 0;

void satisfy(int clause) {
    int cnt = satisfied_count[clause] + 1;
    if(cnt == 1) {
        unsatisfied--;
    }
    satisfied_count[clause] = cnt;
}

void unsatisfy(int clause) {
    int cnt = satisfied_count[clause] -1;
    if(cnt == 0) {
        unsatisfied++;
    }
    satisfied_count[clause] = cnt;
}

void read_dimacs(istream &in) {
    int nbvar,nbclauses;

    //ignore comments
    while(in.peek() == 'c') {
        in.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    //ignore "p"
    in.ignore(numeric_limits<streamsize>::max(), ' ');
    //ignore "cnf"
    in.ignore(numeric_limits<streamsize>::max(), ' ');

    //read number of vars and clauses
    in >> nbvar >> nbclauses;

    formula.resize(nbclauses+1); //one redundant clause at the beginning, does not count as an empty clause
    vars.resize(nbvar+1);

    for(int i=1; i<=nbvar; i++) {
        unsimplified.insert(i);
    }
    unsatisfied = nbclauses;

    //read formula
    int clause = 1;
    int literal;
    while(in >> literal) {
        if(literal == 0) {
            if(formula[clause].size() == 1) {
                found_unit(*formula[clause].begin());
            }
            clause++;
        } else {
            int sign = (literal > 0 ? 1 : -1);
            formula[clause].insert(literal);
            vars[sign*literal].push_back(sign*clause);
        }
    }
}


bool simplify(int lit) {
    int sign = (lit > 0 ? 1 : -1);
    int var = sign * lit;

    unsimplified.erase(var);

    for(int sclause : vars[var]) {
        int csign = (sclause > 0 ? 1 : -1);
        int clause = csign * sclause;

        if(csign == sign) {
            satisfy(clause);
        } else if(satisfied_count[clause] == 0){
            formula[clause].erase(-1*lit);
            if(formula[clause].size() == 1) {
                found_unit(*formula[clause].begin());
            }
            if(formula[clause].size() == 0) {
                destroyed_unit(-1*lit);
                return false;
            }
        }
    }

    return true;
}

void unsimplify(int lit) {
    int sign = (lit > 0 ? 1 : -1);
    int var = sign * lit;

    unsimplified.insert(var);

    for(int sclause : vars[var]) {
        int csign = (sclause > 0 ? 1 : -1);
        int clause = csign * sclause;

        if(csign == sign) {
            unsatisfy(clause);
        } else if(satisfied_count[clause] == 0) {
            if(formula[clause].size() == 1) {
                destroyed_unit(*formula[clause].begin());
            }
            formula[clause].insert(-1*lit);
            if(formula[clause].size() == 1) {
                found_unit(*formula[clause].begin());
            }
        }
    }
}

int choose() {
    return *unsimplified.begin();
}

bool dpll() {
    vector<int> simplified;
    while(!units.empty()) {
        int lit = *units.begin();
        simplified.push_back(lit);
        units.erase(lit);

        if(!simplify(lit))
            goto fail;
    }
    
    if(unsatisfied == 0) {
        for(auto i : simplified) cout << i << endl;
        return true;
    }

    {
    int chosen = choose();

    found_unit(chosen);
    if(dpll()) {
        for(auto i : simplified) cout << i << endl;
        return true;
    }
    destroyed_unit(chosen);
    
    found_unit(-chosen);
    if(dpll()) {
        for(auto i : simplified) cout << i << endl;
        return true;
    }
    destroyed_unit(-chosen);
    }

  fail:
    for(auto it=simplified.rbegin(); it!=simplified.rend(); it++) {
        int lit = *it;
        unsimplify(lit);
        units.insert(lit);
    }
    return false;
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        cout << help << endl;
    }

    ifstream ifile(argv[1]);
    read_dimacs(ifile);
    ifile.close();

    dpll();
}
