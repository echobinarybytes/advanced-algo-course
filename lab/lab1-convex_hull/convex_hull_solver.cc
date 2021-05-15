#include "convex_hull_solver.h"
#include <iostream>
#include <fstream>


ConvexHullSolver::ConvexHullSolver(Data& d) {
    point_set_name = d.GetInFileName();
    res_set_name = d.GetOutFileName();
    std::ifstream ifd(point_set_name, std::fstream::in);

    float x, y;
    while (ifd >> x >> y) {
        point_set.push_back(std::make_pair(x, y));
    }
    ifd.close();
}

ConvexHullSolver::~ConvexHullSolver() {

}

// 基于枚举的凸包求解算法
void ConvexHullSolver::SolverBasedEnum() {

}

// 基于Graham-Scan的凸包求解算法
void ConvexHullSolver::SolverBasedGrahamScan() {

}

// 基于分治思想的凸包求解算法
void ConvexHullSolver::SolverBasedDAndQ() {

}

// 保存到文件中
void ConvexHullSolver::SaveConvexHullSet() {
    std::ofstream ofd(res_set_name, std::fstream::out);
    for (auto point : convex_hull_set) {
        ofd << point.first << point.second << "\n";
    }

    ofd.close();
}
