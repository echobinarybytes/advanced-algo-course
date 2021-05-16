#ifndef _H_SET_COVER_SOLVER_H_
#define _H_SET_COVER_SOLVER_H_

enum class Mode{Greedy , LP, None};

#include "sets.h"
class SetCoverSolver {
public:
    SetCoverSolver(std::string prob_name = "") : problem_name(prob_name), _mode(Mode::None) {  }
    ~SetCoverSolver();
    SetCoverSolver(SetCoverSolver& solver);
    SetCoverSolver& operator=(SetCoverSolver& solver);

public:
    // 基于贪婪的集合覆盖问题
    void GreedySetCover(Set& s, Sets& s_s); 

    // 基于线性规划的集合覆盖问题
    void LinearProgrammingSetCover();

public:
    void PrintResult();
    std::vector<int>& GetResult() { return result;}
    std::string& GetProblemName() { return problem_name;}

private:
    std::vector<int> result; // 存储子集和的序号
    std::string problem_name; // 在解答是构造出解答的问题名字, 以及解答策略
    Mode _mode; 
};

#endif
