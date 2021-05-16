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

/*
 * 线段 p1(x1, y1), p2(x2, y2)
 * 点 p(x3, y3)
 | x1   y1    1 | 
 | x2   y2    1 | = x1 * y2 + x3 * y1 + x2 * y3 - x3 * y2 - x2 * y1 - x1 * y3;
 | x3   y3    1 |
 */
static double Distance(const Point& p1, const Point& p2, const Point& p) {
    double x1 = p1.GetX(), y1 = p1.GetY();
    double x2 = p2.GetX(), y2 = p2.GetY();

    double x3 = p.GetX(), y3 = p.GetY();
    return x1 * y2 + x3 * y1 + x2 * y3 - x3 * y2 - x2 * y1 - x1 * y3;
}

static void Deal(std::vector<std::pair<double, double>>& point_set, std::vector<bool>& left_set, const Point& p1, const Point& p2, std::set<std::pair<double, double>>& ret_set) {
    // 跳出
    if (std::count(left_set.begin(), left_set.end(), true) < 1) return;
    // 1. 在left_set找到距离p1-p2线段最远的
    double max_dis = 0;
    double  max_idx = 0;
    for (int i = 0; i < point_set.size(); ++ i) {
        if (left_set[i] ) {
            double dis = Distance(p1, p2, Point (point_set[i].first, point_set[i].second));
            dis = dis > 0 ? dis : dis * -1; //取绝对值
            if (max_dis < dis) {
                max_dis = dis;
                max_idx = i;
            }

        }
    }
    ret_set.insert(point_set[max_idx]); // 加入凸包集合中
    left_set[max_idx] = false; // 在剩余考虑点中取出该点

    // 2. 删除在三角形内和上的点，由距离最远的几个点构成，在left_set中删除
    Point max_dis_p (point_set[max_idx].first, point_set[max_idx].second);
    Point p1_tmp(p1.GetX(), p1.GetY());
    Point p2_tmp(p2.GetX(), p2.GetY());
    Triangle tri(p1_tmp, p2_tmp, max_dis_p);

    for (int i = 0; i < point_set.size(); ++ i) {
        if (left_set[i] ) {
            Point P(point_set[i].first, point_set[i].second);
            // 判断点P是否在triangle中或者在某条边上
            if (PointInTriangle(tri, P)) {
                left_set[i] = false;
            }

        }
    }


    // 3. 获得L_left, L_right的考虑点
    std::vector<bool> L_left(point_set.size(), false);
    std::vector<bool> L_right(point_set.size(), false); 

    double flag_left = Distance(p1, max_dis_p,  p2); // 判断需要保留的点的正负号
    for (int i = 0; i < point_set.size(); ++ i) {
        if (left_set[i]) {
            Point p(point_set[i].first, point_set[i].second);
            double s = Distance(p1, max_dis_p, p);
            if (flag_left > 0) {
                if (s < 10e-5 &&  s > -10e-5) continue;
                if (s > 0) { // 正向
                    L_left[i] = true;
                } else{ // s == 0
                    L_right[i]= true;

                }

            } else if (flag_left < 0) {
                if (s < 10e-5 &&  s > -10e-5) continue;
                if (s < 0) { // 正向
                    L_left[i] = true;
                } else{ // s == 0
                    L_right[i]= true;

                }

            }

        } 
    }


    // 3. 处理产生的两个子问题
    Deal(point_set, L_left, p1, max_dis_p, ret_set);
    Deal(point_set, L_right, p2, max_dis_p, ret_set);
}

// 基于分治思想的凸包求解算法
void ConvexHullSolver::SolverBasedDAndQ() {
    convex_hull_set.clear(); // 清除之前保存的值
    /*
    1. 以连接最大纵坐标点y_max和最小纵坐标点y_min的线段d={y_max, y_min}划分为左点集L_left和右点集L_right
    2. Deal(L_left); Deal(L_right)

    对于Deal(L_left)
     确定距离d最远的点P
     在三角形内的点(三角形三条边分别是a, b, d; d边是初始线段，a,b是通过加入点P构成的)，删除;
     线段a外的点与a构成L_left的子问题
     线段b外的点与b构成L_left的子问题
    */

    // 1. 找到最大纵坐标和最小纵坐标 
    int y_max_idx = 0, y_min_idx = 0;
    for (int i = 1; i < point_set.size(); ++ i) {
        if (point_set[y_max_idx].second < point_set[i].second) y_max_idx = i;
        if (point_set[y_min_idx].second > point_set[i].second) y_min_idx = i;
    }

    Point y_max(point_set[y_max_idx].first, point_set[y_max_idx].second);
    Point y_min(point_set[y_min_idx].first, point_set[y_min_idx].second);

    // 添加y_max, y_min到结果中
    convex_hull_set.insert(point_set[y_max_idx]);
    convex_hull_set.insert(point_set[y_min_idx]);

    // 2. Deal(L_left); Deal(L_right);
    std::vector<bool> L_left(point_set.size(), false); // 值为true表示在问题中需要考虑这个点
    std::vector<bool> L_right(point_set.size(), false); // 值为true表示在问题中需要考虑这个点

    // 判断点是否在同一侧，通过判断下式是否同号, 即可通过计算距离
    /*
     * 线段 p1(x1, y1), p2(x2, y2)
     * 点 p(x, y)
       | x1   y1    1 | 
       | x2   y2    1 | = x1 * y2 + x3 * y1 + x2 * y3 - x3 * y2 - x2 * y1 - x1 * y3;
       | x3   y3    1 |
     */
    for (int i = 0; i < point_set.size(); ++ i) {
        Point p(point_set[i].first, point_set[i].second);
        double s = Distance(y_max, y_min, p);
        if (s < 10e-5 && s > -10e-5) continue;
        if (s > 0) { // 正向
            L_left[i] = true;
        } else{ // s == 0
            L_right[i]= true;

        }

    }

    Deal(point_set, L_left, y_max, y_min, convex_hull_set);
    Deal(point_set, L_right, y_max, y_min, convex_hull_set);


}

// 保存到文件中
void ConvexHullSolver::SaveConvexHullSet(const std::string& store_file_name) {
    std::ofstream ofd(store_file_name, std::fstream::out);
    for (auto point : convex_hull_set) {
        ofd << point.first << " " << point.second << "\n";
    }

    ofd.close();
}

