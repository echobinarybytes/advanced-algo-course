#include "sets.h"
#include "set_cover_solver.h"
#include <algorithm>

int main() {
#if 0 //检查代码运行情况
    std::vector<int> situation = {20};
#else //完成实验
    std::vector<int> situation = {100, 1000, 5000};
#endif
    for (auto iter = situation.begin(); iter != situation.end(); ++ iter) {
        int i = *iter; 
        // 产生set集
        std::string set_file_name = std::string ("./data/set_data") + std::to_string(i) + ".txt";
        Set s(i, set_file_name);
        s.Generator();
        s.Store(set_file_name);

        // 更具set集产生sets
        std::string sets_file_name = std::string ("./data/sets_data") + std::to_string(i) + ".txt";
        Sets s_s(s.Size(), sets_file_name);
        s_s.Generator(s);
        s_s.Store(sets_file_name);

        // 找到最小覆盖
        SetCoverSolver solver(set_file_name + "---" + sets_file_name);
        solver.GreedySetCover(s, s_s);
        solver.PrintResult();
    }
}
