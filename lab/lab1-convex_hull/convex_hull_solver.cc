#include "convex_hull_solver.h"
#include <iostream>
#include <fstream>
#include "point_triangle_vec.h"


ConvexHullSolver::ConvexHullSolver(Data& d) {
    point_set_name = d.GetInFileName();
    res_set_name = d.GetOutFileName();
    std::ifstream ifd(point_set_name, std::fstream::in);

    double x, y;
    while (ifd >> x >> y) {
        point_set.push_back(std::make_pair(x, y));
    }
    ifd.close();
}

ConvexHullSolver::~ConvexHullSolver() {

}

// 基于枚举的凸包求解算法
void ConvexHullSolver::SolverBasedEnum() {
    int point_cnt = point_set.size();
    std::vector<bool> flag(point_set.size(), false); // true表示已经删除，false表示没有被删除
   // 选择某三个点构成三角形A, B, C
    for (int i = 0; i < point_cnt; ++ i) { // A
        if (flag[i] == true) continue;
        Point A(point_set[i].first, point_set[i].second);

        for (int j = i + 1; j < point_cnt; ++ j) {// B
            if (flag[j] == true) continue;
            Point B(point_set[j].first, point_set[j].second);

            for (int k = j + 1; k < point_cnt; ++ k) {// C
                if (flag[k] == true) continue;
                Point C(point_set[k].first, point_set[k].second);

                // 选择某个点P, 判断点P是否在凸包的顶点集合中, 如果不再，则在集合中删除这个点
                for (int l = 0; l < point_cnt; ++ l) {// P
                    if (flag[l] == true || l == i || l == j || l == k) continue;
                    Point P(point_set[l].first, point_set[l].second);
                    Triangle tri(A, B, C);
                    // 判断点P是否在triangle中或者在某条边上
                    if (PointInTriangle(tri, P)) {
                        flag[l] = true;
                    }


                } // end for (l) 
            }// end for (k)
        } // end for (j)
    } // end for (i)

    for (int i = 0; i < point_cnt; ++ i) {
        if (!flag[i])
            convex_hull_set.insert(point_set[i]);
    }
}

#include <cmath> // atan2(double x, double y)
#include <stack>

static int FindP0Index(const std::vector<std::pair<double, double>>& points) {
    int min_y = 0;
    for (int i = 0; i < points.size(); ++ i) {
        if (points[min_y].second > points[i].second) {
            min_y = i;
        }
    }
    return min_y;
}

static std::pair<double, double>& NextToTop(std::stack<std::pair<double, double>>& s) {
    auto p = s.top();
    s.pop();
    auto& ret = s.top();
    s.push(p);
    return ret;
}

// 判断是否在左边的角度，通过比较角度
// top_pre pre (1)
// top_pre test    (2)
// (1) > (2) 右移动
// (1) == (2) 不移动
// (1) < (2) 左移动
static bool NotLeftMove(std::pair<double, double>& top_pre, std::pair<double, double>& top, std::pair<double,double>& test) {
            double x0 = top_pre.first, y0 = top_pre.second; // 
            double x1 = top.first, y1 = top.second; //
            double tmp_a = atan2(y1 - y0, x1 - x0);
            tmp_a = tmp_a >=0 ? tmp_a : 2 * PI + tmp_a;

            double x2 = test.first, y2 = test.second; // 
            double tmp_b = atan2(y2 - y0, x2 - x0);
            tmp_b = tmp_b >=0 ? tmp_b : 2 * PI + tmp_b;

            return tmp_a >= tmp_b;
}


// 基于Graham-Scan的凸包求解算法
void ConvexHullSolver::SolverBasedGrahamScan() {
    // 1. 求p_0_idx，y的坐标最小
    int p_0_idx = FindP0Index(point_set);

    convex_hull_set.clear(); // 清空之前的计算结果
    for (auto point : point_set) { // 去重
        convex_hull_set.insert(point);
    }
    // 除去最小坐标的点集
    std::vector<std::pair<double, double>> GS_point_set;
    for (auto iter = convex_hull_set.begin(); iter != convex_hull_set.end(); ++ iter) {
        if (*iter != point_set[p_0_idx])
            GS_point_set.push_back(*iter);
    }

    p0_x = point_set[p_0_idx].first;
    p0_y = point_set[p_0_idx].second;

    // 2. 按照与p0的级角大小排序
    sort(GS_point_set.begin(), GS_point_set.end(), 
            [this](const std::pair<double, double>& a, const std::pair<double, double>& b) {
            // 计算极角
            double x0 = this->p0_x, y0 = this->p0_y; // 
            double x1 = a.first, y1 = a.second; // (0, 1)
            double tmp_a = atan2(y1 - y0, x1 - x0);
            tmp_a = tmp_a >=0 ? tmp_a : 2 * PI + tmp_a;

            double x2 = b.first, y2 = b.second; // (0, 1)
            double tmp_b = atan2(y2 - y0, x2 - x0);
            tmp_b = tmp_b >=0 ? tmp_b : 2 * PI + tmp_b;

            return tmp_a < tmp_b; // 从小到大排序
            } );

    // 3. Push操作
    std::stack<std::pair<double, double>> s;
    s.push(point_set[p_0_idx]); // Push(p0)
    s.push(GS_point_set[1 - 1]); // Push(p1)
    s.push(GS_point_set[2 - 1]); // Push(p2)

    // 4 - 7
    for (int i = 3 - 1; i < GS_point_set.size(); ++ i) {
        // while (Next-to-top(s) Top(s), pi 形成非左移动) s.pop();
        while (NotLeftMove(NextToTop(s), s.top(), GS_point_set[i])) s.pop();
        s.push(GS_point_set[i]);
    }

    // 8
    convex_hull_set.clear();
    while (!s.empty()) {
        convex_hull_set.insert(s.top());
        s.pop();
    }
}

// 基于分治思想的凸包求解算法
void ConvexHullSolver::SolverBasedDAndQ() {

}

// 保存到文件中
void ConvexHullSolver::SaveConvexHullSet(const std::string& store_file_name) {
    std::ofstream ofd(store_file_name, std::fstream::out);
    for (auto point : convex_hull_set) {
        ofd << point.first << " " << point.second << "\n";
    }

    ofd.close();
}

