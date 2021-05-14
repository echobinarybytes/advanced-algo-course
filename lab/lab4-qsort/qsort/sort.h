#ifndef _H_SORT_H_
#define _H_SORT_H_

#include "data.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
class Sort{
public:
    Sort()  {}
    Sort(const Sort& s) = delete;
    ~Sort() {}

    // 读入数据
    void ReadData(const Data& data) {
        std::ifstream ifd(data.GetDataFileName(), std::fstream::in);
        int tmp;
        while(ifd >> tmp) v_data.push_back(tmp);
    }

    // 对数据进行排序
    void QSort();

    void LibSort() { std::sort(v_data.begin(), v_data.end()); }

    // 返回排好序的数据
    void StoreSortedData(const Data& data) {
        data.StoreResult(v_data);
    }

private:
    std::vector<int> v_data;
};
#endif
