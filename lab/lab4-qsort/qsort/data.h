#ifndef _H_DATA_H_
#define _H_DATA_H_

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <random>
#include <vector>

using std::string;

class Data {
public:
    Data(const string& if_n, const string& of_n) : if_name(if_n), of_name(of_n), delete_file(false)  { }
    Data(const Data& d) = delete;
    ~Data() {
        if (delete_file) remove(if_name.c_str());
    }

    void Generator(const int cnt, const int i = -1) {// 产生数据, 数据的量是cnt个, i表示重复率
        std::ofstream ofd;
        ofd.open(if_name, std::fstream::out);
        std::default_random_engine e;
        std::uniform_int_distribution<int> uniform(e.min(), e.max());
        
        if (i == -1) {
            for (int i = 0; i < cnt; ++ i) {
                ofd << std::to_string(uniform(e)) << "\n";
            }
        } else {
            int rand_cnt = cnt - cnt * 10 * i / 100;
            for (int i = 0; i < rand_cnt; ++ i) {
                ofd << std::to_string(uniform(e)) << "\n";
            }
            int repeat_cnt = cnt - rand_cnt;
            int repeat = uniform(e);
            // std::cout << repeat << std::endl;
            for (int i = 0; i < repeat_cnt; ++i) 
                ofd << repeat << "\n";
        }
        ofd.close();
    }



    // 这里应该传入什么参数进来
    void StoreResult(std::vector<int>& v) const {
        std::ofstream ofd;
        ofd.open(of_name, std::fstream::out);
        for (auto num : v) {
            ofd << num  << "\n";
        }
        
        ofd.close();
    }
    // others
    // 设置是否删除文件，
    //
    //
    string GetDataFileName() const {
        return if_name;
    }

private:
    string if_name; // 保存生成数据的文件
    string of_name; // 保存排序后结果的文件
    bool delete_file; // 是否删除文件
};

class Data;

#endif
