#include "SatSolver.hpp"


#include <istream>
#include <limits>

#include <vector>
#include <unordered_set>
#include <unordered_map>

#include <iostream>

using namespace std;

void SatSolver::read_dimacs(istream &in) {
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


struct ChangesList {
    SatSolver * solver;
    vector<int> simplified;

    ChangesList(SatSolver *_solver) : solver(_solver) {}

    ~ChangesList() {
        for(auto it=simplified.rbegin(); it!=simplified.rend(); it++) {
            int lit = *it;
            solver->unsimplify(lit);
            solver->units.insert(lit);
        }
    }

    void add(int lit) {
        simplified.push_back(lit);
    }
};
bool SatSolver::solve() {
    ChangesList changes(this);
    while(!units.empty()) {
        int lit = *units.begin();
        changes.add(lit);
        units.erase(lit);

        if(!simplify(lit))
            return false;
    }

    if(unsatisfied == 0) {
        for(auto i : changes.simplified) solution.push_back(i);
        return true;
    }
    
    int chosen = choose();
    if(chosen == 0)
        return false;

    found_unit(chosen);
    if(solve()) {
        for(auto i : changes.simplified) solution.push_back(i);
        return true;
    }
    destroyed_unit(chosen);
    
    found_unit(-chosen);
    if(solve()) {
        for(auto i : changes.simplified) solution.push_back(i);
        return true;
    }
    destroyed_unit(-chosen);

    return false;
}


bool SatSolver::simplify(int lit) {
    int sign = (lit > 0 ? 1 : -1);
    int var = sign * lit;

    unsimplified.erase(var);

    bool ok = true;
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
                //return false;
                ok = false;
            }
        }
    }

    //return true;
    return ok;
}

void SatSolver::unsimplify(int lit) {
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

int SatSolver::choose() {
    if(unsimplified.empty())
        return 0;
    else
        return *unsimplified.begin();
}

void SatSolver::found_unit(int lit) {
    int cnt = unit_count[lit] + 1;
    if(cnt == 1)
        units.insert(lit);
    unit_count[lit] = cnt;
}

void SatSolver::destroyed_unit(int lit) {
    int cnt = unit_count[lit] - 1;
    if(cnt == 0) {
        units.erase(lit);
    }
    unit_count[lit] = cnt;
}

void SatSolver::satisfy(int clause) {
    int cnt = satisfied_count[clause] + 1;
    if(cnt == 1) {
        unsatisfied--;
    }
    satisfied_count[clause] = cnt;
}

void SatSolver::unsatisfy(int clause) {
    int cnt = satisfied_count[clause] -1;
    if(cnt == 0) {
        unsatisfied++;
    }
    satisfied_count[clause] = cnt;
}
