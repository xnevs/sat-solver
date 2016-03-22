#ifndef SATSOLVER_HPP
#define SATSOLVER_HPP

#include <vector>
#include <unordered_set>
#include <unordered_map>

#include <istream>

struct ChangesList;

class SatSolver {
  public:
    // If a solution was found it is stored here.
    std::vector<int> solution;

    // Parse an input stream in dimacs format.
    void read_dimacs(std::istream &in);

    // The main function of the DPLL algorithm.
    bool solve();

  private:
    // A formula is a vector of sets of literals.
    std::vector<std::unordered_set<int>> formula;

    // A mapping from variables to the set of clauses
    // a given variable appears in.
    std::vector<std::vector<int>> vars;

    // A set of not yet simplified variables
    // (i.e. variables not yet given a value).
    std::unordered_set<int> unsimplified;

    bool simplify(int lit);
    void unsimplify(int lit);
    int choose();

    // A mapping from literals to the number of
    // unit clauses the literal appears in.
    std::unordered_map<int,int> unit_count;
    std::unordered_set<int> units;

    void found_unit(int lit);
    void destroyed_unit(int lit);

    // A mapping from clauses to the number of
    // satisfied literals appearing in the clause.
    std::unordered_map<int,int> satisfied_count;

    // A counter of unsatisfied clauses.
    int unsatisfied;

    void satisfy(int clause);
    void unsatisfy(int clause);

    friend ChangesList;
};


#endif /* SATSOLVER_HPP */
