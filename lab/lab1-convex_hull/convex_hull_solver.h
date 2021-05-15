#ifndef _H_CONVEX_HULL_SOLVER_H_
#define _H_CONVEX_HULL_SOLVER_H_

#include "data.h"
#include <vector>
#include <string>

class ConvexHullSolver {
public:
    // 构造函数初始化vector
    ConvexHullSolver(Data& d);
    ConvexHullSolver(ConvexHullSolver& con) = delete;
    ~ConvexHullSolver();

public:
    // 求解器
    void SolverBasedEnum();
    void SolverBasedGrahamScan();
    void SolverBasedDAndQ();

    // 保存到文件中
    void SaveConvexHullSet();
private:
    std::vector<std::pair<float, float>> point_set; // 初始点集
    std::vector<std::pair<float, float>> convex_hull_set; // 凸包点集
    std::string point_set_name;
    std::string res_set_name;
};


#endif
