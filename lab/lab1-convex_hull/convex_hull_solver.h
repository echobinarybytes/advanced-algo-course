#ifndef _H_CONVEX_HULL_SOLVER_H_
#define _H_CONVEX_HULL_SOLVER_H_

#include "data.h"
#include <vector>
#include <string>
#include <set>

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
    void SaveConvexHullSet(const std::string& store_file_name);

public:
    // 返回保存的文件名字
    string GetStoreFileName() { return res_set_name; }

public:
    double p0_x, p0_y; // 用在Graham-Scan表示p0
    // const static double PI;
private:
    std::vector<std::pair<double, double>> point_set; // 初始点集
    std::set<std::pair<double, double>> convex_hull_set; // 凸包点集, 去重
    std::string point_set_name;
    std::string res_set_name;
};

const static double PI = 3.141592653;
#endif
