#ifndef SATSOLVER_HPP
#define SATSOLVER_HPP

#include <vector>
#include <unordered_set>
#include <unordered_map>

#include <istream>

struct ChangesList;

class SatSolver {
  public:
    std::vector<int> solution;

    void read_dimacs(std::istream &in);
    bool solve();

  private:
    std::vector<std::unordered_set<int>> formula;
    std::vector<std::vector<int>> vars;

    std::unordered_set<int> unsimplified;

    bool simplify(int lit);
    void unsimplify(int lit);
    int choose();

    std::unordered_map<int,int> unit_count;
    std::unordered_set<int> units;

    void found_unit(int lit);
    void destroyed_unit(int lit);

    std::unordered_map<int,int> satisfied_count;
    int unsatisfied;

    void satisfy(int clause);
    void unsatisfy(int clause);

    friend ChangesList;
};


#endif /* SATSOLVER_HPP */
