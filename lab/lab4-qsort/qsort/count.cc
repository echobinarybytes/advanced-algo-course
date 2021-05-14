#include <iostream>
#include <fstream>
#include <string>

int Count(int num, std::string name) {
    std::ifstream ifd(name, std::ifstream::in);

    int tmp;
    int cnt = 0;
    while (ifd >> tmp) {
        if (tmp == num) ++ cnt; 
    }

    ifd.close();
    return cnt;
}

int main() {
#if 0
    std::string name("data_1.txt"); 
    Count(1991083534, name);
#endif
    std::cout << Count(1991083534, std::string ("data_1.txt"))
    << std::endl;
}
