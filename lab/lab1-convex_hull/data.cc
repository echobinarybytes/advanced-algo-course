#include "data.h"

bool Data::Generator(const int cnt) {
    // 判断文件是否存在
    std::ifstream ifd(if_name);
    if (ifd) {
        ifd.close();
        return false;
    }

    // 文件不存在
    std::ofstream ofd;
    ofd.open(if_name, std::fstream::out);
    std::default_random_engine e;
    std::uniform_real_distribution<double> uniform(0, 100);

    for (int i = 0; i < cnt; ++ i) {
        // generator (x, y)
        ofd << uniform(e) << " " << uniform(e) << "\n";
    }
    ofd.close();
    return true;
}

void Data::StoreResult() {

}

