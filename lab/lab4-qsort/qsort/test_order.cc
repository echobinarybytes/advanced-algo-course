#include <iostream>
#include <fstream>
#include <string>

// 测试输入的文件是否有序
bool TestOrder(std::ifstream& ifd) {
    int pre;
    ifd >> pre;
    int now;
    while (ifd >> now) {
        if (pre > now) {
            std::cout << "pre is " << pre << "; now is " << now << std::endl;
            return false;
        } 
        else {
            pre = now;
        }
    }
    return true;
}

int main() {
    for (int i = 0; i < 11; ++ i) {
        std::string name = "res_" + std::to_string(i) + ".txt";
        std::ifstream ifd(name, std::fstream::in);

        std::cout << TestOrder(ifd) << std::endl;
        ifd.close();

    }
}
