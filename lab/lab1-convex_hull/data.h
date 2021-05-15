#ifndef _H_DATA_H_
#define _H_DATA_H_
// #include "../lab4-qsort/qsort/data.h"
#include <iostream>
#include <fstream>
#include <random>
#include <string>
using std::string;

// TODO : 继承../lab4-qsort/qsort/data.h中的Data类
class Data{
public:
     Data(const string& if_n, const string& of_n) : if_name(if_n), of_name(of_n), delete_file(false)  { }
     Data(const Data& d) = delete;
     ~Data() {
         if (delete_file) remove(if_name.c_str());
     }

     // 产生数据
    bool Generator(const int cnt);
    // 存储数据
    void StoreResult(void);

public:
    string GetInFileName(void) const { return if_name; }
    string GetOutFileName(void) const { return of_name; }

private:
     string if_name; // 保存生成数据的文件
     string of_name; // 保存排序后结果的文件
     bool delete_file; // 是否删除文件
};



#endif
