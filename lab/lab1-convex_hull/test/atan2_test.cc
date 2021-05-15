#include <cmath>
#include <iostream>

using std::cout;
using std::endl;

static const double PI = 3.141592653;
void Atan2Test() {
    double x0 = 0, y0 = 0; // (0, 0)
    double x1 = 0, y1 = 1; // (0, 1)
    std::cout << atan2(y1 - y0, x1 - x0)*180/PI << std::endl; // 90
    std::cout << atan2(y0 - y1, x0 - x1)*180/PI << std::endl; // -90 // 与顺序有关

    double x2 = 1, y2 = 0; // (1, 0)
    std::cout << atan2(y2 - y0, x2 - x0)*180/PI << std::endl; // 0

}

int main() {
    Atan2Test();
}
