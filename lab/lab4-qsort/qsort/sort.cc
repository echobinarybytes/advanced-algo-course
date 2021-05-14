#include "sort.h"
#include <random>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <stack>

static int RandPartition(std::vector<int>& v, int low, int high) {
#if 1
    static std::default_random_engine e;
    std::uniform_int_distribution<int> u(low, high);
#if 1    // 三数取中
    int min = u(e), mid = u(e), max = u(e);
    if (v[min] > v[mid]) std::swap(min, mid);
    if (v[mid] > v[max]) std::swap(mid, max);
    if (v[min] > v[mid]) std::swap(min, mid); 

    int i = mid;

#else 
    int i = u(e);
#endif

#else
    // 0 ~ high - low 
    int i = rand() % (high - low + 1) + low;
#endif
    std::swap(v[high], v[i]);

    int x = v[high];
    i = low - 1;
    for (int j = low; j <= high - 1; ++ j) {
        if (v[j] <= x) {
            ++ i;
            std::swap(v[i], v[j]);
        }
    }
    std::swap(v[i + 1], v[high]);
    return i + 1;
}

static void RandPartitionWith3Divide(std::vector<int>& v, int low, int high, int& left, int& right) {
#if 1
    static std::default_random_engine e;
    std::uniform_int_distribution<int> u(low, high);
#if 1    // 三数取中
    int min = u(e), mid = u(e), max = u(e);
    if (v[min] > v[mid]) std::swap(min, mid);
    if (v[mid] > v[max]) std::swap(mid, max);
    if (v[min] > v[mid]) std::swap(min, mid); 

    int _i = mid;

#else 
    int i = u(e);
#endif

#else
    // 0 ~ high - low 
    int i = rand() % (high - low + 1) + low;
#endif
    std::swap(v[high], v[_i]);

    int x = v[high]; // pivot

    left = low - 1; // left的左边都比pivot小, 包括left
    right = high + 1; // left的右边都比pivot大, 包括right
    int i = low; // i是探路指针
    while (i < right) { 
        if (x == v[i]) {
            ++ i;
        } else if (v[i] < x) {
            ++ left;
            std::swap(v[i], v[left]);
            ++ i;
        } else if (v[i] > x) {
            -- right;
            std::swap(v[i], v[right]);
        }
    }
}
static int Partition(std::vector<int>& v, int low, int high) {
    // 双指针，i, j
    // i 始终指向小的树，j指向下一个目标数
    int i = low - 1, j = low;
    int x = v[high]; // pivot
    // 什么时候停止
    for (; j < high; ++ i) {
        if (v[j] <= x) {
            ++ i;
            std::swap(v[i], v[j]);
        }
    }
    std::swap(v[i + 1], v[high]);
    return i + 1;

}

static void InsertSort(std::vector<int>& v, int low, int high) {
    for (int i = low + 1; i <= high; ++ i) { // 选定某一元素
        if (v[i - 1] > v[i]) {
            for (int j = i; j > low; -- j) { // 前移
                if (v[j - 1] > v[j]) std::swap(v[j - 1], v[j]);
            }
        }
    }

}
static void QuickSort(std::vector<int>& v, int low, int high) {
#if 0 // 选择使用递归
    if (low < high) {
#if 1
        int q = RandPartition(v, low, high);
#else
        // not random
        int q = Partition(v, low, high);
#endif
        QuickSort(v, low, q - 1);
        QuickSort(v, q + 1, high);
    }
#else // 选择使用栈保存
    std::stack<std::pair<int, int>> s;
    s.push(std::make_pair(low, high));
    while (!s.empty()) {
        int l = s.top().first;
        int h = s.top().second;
        s.pop();
        if (l < h) {
#if 1 // 添加插入排序
            if (h - l <= 10) {
                InsertSort(v, l, h);
                continue;
            } 
#endif

#if 0 // 使用普通划分
            int q = RandPartition(v, l, h);
            s.push(std::make_pair(l, q - 1));
            s.push(std::make_pair(q + 1, h));
#else  // 使用三划分
            int left, right;
            RandPartitionWith3Divide(v, l, h, left, right);
            s.push(std::make_pair(l, left));
            s.push(std::make_pair(right, h));
#endif
        }
    }

#endif
}

// v_data is a vector
void Sort::QSort() {
    srand(time(nullptr));
    QuickSort(v_data, 0, v_data.size() - 1);
}

