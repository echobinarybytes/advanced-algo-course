#include "sort.h"
#include "data.h"
#include "timer.h"
#include <string>
#include <iostream>


int main(int argc, char** argv) {
#if 0 // 产生数据集
    for (int i = 0; i < 11; ++ i) {
        std::string data_name = "data_" + std::to_string(i) + ".txt";  
        std::string res_name = "res_" + std::to_string(i) + ".txt"; 
        Data data(data_name, res_name);
        // 产生数据
        if (argc == 1) {
            data.Generator(100000, i);
        } else if (argc == 2) {
            data.Generator(std::atoi(argv[1]));
        } else {
            std::cout << "Input error" << std::endl;
        }
    }
#else 
    for (int i = 0; i < 11; ++ i) {
        std::string data_name = "data_" + std::to_string(i) + ".txt";  
        std::string res_name = "res_" + std::to_string(i) + ".txt"; 
        Data data(data_name, res_name);

        Sort sortor;

        // 传入数据
        sortor.ReadData(data);

        Timer t;
        std::cout << std::string(10, '-') << "User Sort BEGIN - "  << i << std::string(10, '-') << std::endl;
        t.Start();
        // 对数据进行排序
        sortor.QSort();
        t.End();
        t.Show();
         std::cout << std::string(10, '-') << "END" << std::string(10, '-') << std::endl;

         std::cout << std::string(10, '-') << "Lib Sort BEGIN - "  << i << std::string(10, '-') << std::endl;
        sortor.StoreSortedData(data);

#if 0 // 使用库函数
        t.Start();
        // 对数据进行排序
        sortor.LibSort();
        t.End();
        t.Show();
        std::cout << std::string(10, '-') << "END" << std::string(10, '-') << std::endl;
        // 返回数据
        sortor.StoreSortedData(data);
#endif
    }
#endif
}
