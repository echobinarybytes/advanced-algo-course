#ifndef _H_POINT_TRIANGLE_VEC_H_
#define _H_POINT_TRIANGLE_VEC_H_
#include <algorithm>
#include <iostream>

// doubleODO: 操纵不了模板编程
class Point {
public:
    // Point();
    Point(double x, double y) : _x(x), _y(y) {}
    Point(Point& p) {
        _x = p.GetX();
        _y = p.GetY();
    }

    Point& operator=(const Point& p) {
        _x = p.GetX();
        _y = p.GetY();
        return *this;
    }
    ~Point() {  }

public:
    // return x value
    double GetX() const { return _x; }
    // return y value
    double GetY() const { return _y; }

private:
    double _x;
    double _y;
};

class Vec;
class Triangle {
public:
    // 设置点的(x, y)坐标
    Triangle(double x1, double y1,
             double x2, double y2,
             double x3, double y3) : _A(x1, y1), _B(x2, y2), _C(x3, y3) {  } 
    Triangle(Point& A, Point& B, Point& C) : _A(A), _B(B), _C(C) {  }
    Triangle(Triangle& t) = delete;
    ~Triangle() {  }

public:
    //获取A, B, C
    Point& GetPointA() { return _A; }
    Point& GetPointB() { return _B; }
    Point& GetPointC() { return _C; }
private:
    Point _A;
    Point _B;
    Point _C;
};

class Vec {
public:
    Vec(Point O, Point P) : vec(P.GetX() - O.GetX(), P.GetY() - O.GetY()) {  }
    Vec(Vec& v) = delete;
    ~Vec() {  }

public:
    Point& GetVec() { return vec; }
    // 重载*，表示叉乘 vec*op
    double operator*(Vec& op) {
        return vec.GetX()*op.GetVec().GetY() - vec.GetY()*op.GetVec().GetX();
    } 

private:
    Point vec;
};

// 使用
bool PointInTriangle(Triangle& tri, Point& p) {

    // 获得向量AB, AP
    // 错误 no matching constructor for initialization of 'Point<int>'
    Vec AB(tri.GetPointA(), tri.GetPointB());
    Vec AP(tri.GetPointA(), p);
    // 计算叉乘
    double res1 = AB * AP;

    // 获得向量BC, BP
    Vec BC(tri.GetPointB(), tri.GetPointC());
    Vec BP(tri.GetPointB(), p);
    // 计算叉乘
    double res2 = BC * BP;

    // 获得向量CA, CP
    Vec CA(tri.GetPointC(), tri.GetPointA());
    Vec CP(tri.GetPointC(), p);
    // 计算叉乘
    double res3 = CA * CP;


    if ( (res1 < 0 && res2 < 0 && res3 < 0) || 
         (res1 > 0 && res2 > 0 && res3 > 0)) return true;
    if (res1 == 0 || res2 == 0 || res3 == 0) return true;

    return false;
}


#if 0 // test
int main() {
    Point A(0, 0);
    Point B(4, 0);
    Point M(1, 2);
    Point N(3, 4);
    Point O(3, -4);

    Triangle t(A, M, O);
    Point p(N);
    if (PointInTriangle(t, p)) {
        std::cout << "point in triangle" << std::endl;
    } else {
        std::cout << "point not in triangle" << std::endl;
    }

    Triangle t1(M, O, B);
    Point p1(A);
    if (PointInTriangle(t1, p1)) {
        std::cout << "point in triangle" << std::endl;
    } else {
        std::cout << "point not in triangle" << std::endl;
    }

    Triangle t2(A, N, O);
    Point p2(A);
    if (PointInTriangle(t2, p2)) {
        std::cout << "point in triangle" << std::endl;
    } else {
        std::cout << "point not in triangle" << std::endl;
    }


}
#endif

#endif
