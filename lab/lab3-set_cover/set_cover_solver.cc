#include "set_cover_solver.h"

SetCoverSolver::~SetCoverSolver() {

}

SetCoverSolver::SetCoverSolver(SetCoverSolver& solver) {
    result = solver.GetResult();
    problem_name = solver.GetProblemName();
}

SetCoverSolver& SetCoverSolver::operator=(SetCoverSolver& solver) {
    result = solver.GetResult();
    problem_name = solver.GetProblemName();
    return *this;

}
 
static int FindCoverMost(Set& U, Sets& s_s, std::vector<bool>& set_used) {
    int max_pair = 0;
    int max_idx = 0;
    // Error: 'this' argument to member function 'GetSize' has type 'const Sets', but function is not marked const
    for (int i = 0; i < s_s.GetSize(); ++ i) {
        if (!set_used[i]) {// 如果没有被使用过
            int tmp_pair = 0;
            // 比较*(s_s.GetSets()[i]) 和 U.GetSet()中元素的相同个数
            for (auto i : *(s_s.GetSets()[i]) ) {
                for (auto j : U.GetSet()) {
                    if (i == j) ++ tmp_pair;
                }
            }

            if (max_pair < tmp_pair) {
                max_pair = tmp_pair;
                max_idx = i;
            }
        }
    }
    set_used[max_idx] = true;
    return max_idx;
}

// 基于贪婪的集合覆盖问题
void SetCoverSolver::GreedySetCover(Set& s, Sets& s_s) {
    _mode = Mode::Greedy; 
    // 1. U是s中尚未被覆盖的元素集
    Set U = s;
    std::vector<bool> set_used(s_s.GetSize(), false); // 如果为true表示Sets中对应的元素已经被使用过

    // 2. 
    result.clear();
    std::vector<int>& C = result;

    // 3-6
    while (U.GetSet().size() != 0) {
        // 4. Greedy选择-选择能覆盖最多U元素的子集
        int idx = FindCoverMost(U, s_s, set_used);

        // 5.
        for (auto& ele : *(s_s.GetSets()[idx])) {
            U.GetSet().erase(ele);
        }

        // 6
        C.push_back(idx);
    }
}

 // 基于线性规划的集合覆盖问题
void SetCoverSolver::LinearProgrammingSetCover() {

}
 
 void SetCoverSolver::PrintResult() {
     if (_mode == Mode::Greedy) {
         std::cout << problem_name << " in greedy"<<  std::endl;
     } else if (_mode == Mode::LP) {
         std::cout << problem_name << " in linear programming"<<  std::endl;

     } else return;
     for (auto idx : result) {
         std::cout << idx  << " ";
     }
     std::cout << std::endl;
     
 }

