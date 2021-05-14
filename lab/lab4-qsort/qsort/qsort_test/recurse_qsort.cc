#include <vector>
#include <iostream>
#include <stack>

#if 0
// 选择最低元素作为基础
// 通过将其他元素放到最低元素实现灵活使用
// 从小到大排序
int partition(std::vector<int>& vec, int low, int high) {
    int pivot = vec[low];
    while(low < high) {
        while (low < high && vec[high] > pivot) -- high;
        if (low < high) vec[low] = vec[high];
        while (low < high && vec[low] < pivot) ++ low;
        if (low < high) vec[high] = vec[low];
    }
    vec[low] = pivot;
    return low;
}

#else
// 选择最高元素作为基础
// 通过将其他元素放到最高元素实现灵活使用
// 从小到大排序
int partition(std::vector<int>& vec, int low, int high) {
	int pivot = vec[high];
    int i = low - 1; // i元素前都是比pivot小的元素
    int j = low; // 遍历指针
    while (j < high) {
        if (vec[j] < pivot) {
            ++ i;
            std::swap(vec[i], vec[j]);
        }
        ++ j;
    }
    std::swap(vec[i + 1], vec[high]);
    return i + 1;
}
#endif

static void RandPartitionWith3Divide(std::vector<int>& v, int low, int high, int& left, int& right) {
    int x = v[high]; // pivot

    left = low - 1; // left的左边都比pivot小, 包括left
    right = high + 1; // left的右边都比pivot大, 包括right
    for (int i = low; i <= high - 1; ++ i) { // i是探路指针
        if (x == v[i]) {
            ++ i;
        } else if (v[i] < x) {
            ++ left;
            std::swap(v[i], v[left]);
            ++ i;
        } else if (v[i] > x) {
            -- right;
            std::swap(v[i], v[left]);
        }
    }
}

#if 0
void QuickSort(std::vector<int>& v, int low, int high) {
    if (low < high) {
        int i = partition(v, low, high);
        QuickSort(v, low, i - 1);
        QuickSort(v, i + 1, high);
    }
}

#else 

static void InsertSort(std::vector<int>& v, int low, int high) {
    for (int i = low + 1; i <= high; ++ i) { // 选定某一元素
        if (v[i - 1] > v[i]) {
            for (int j = i; j > low; -- j) { // 前移
                if (v[j - 1] > v[j]) std::swap(v[j - 1], v[j]);
            }
        }
    }

}

void QuickSort(std::vector<int>& v, int low, int high) {
    std::stack<std::pair<int, int>> s;
    s.push(std::make_pair(low, high));
    while (!s.empty()) {
        int l = s.top().first;
        int h = s.top().second;
        s.pop();
        if (l < h) {
            if (h - l <= 10) {
                InsertSort(v, l, h);
                continue;
            }
            int left, right;
            RandPartitionWith3Divide(v, l, h, left, right);
            s.push(std::make_pair(l, left));
            s.push(std::make_pair(right, h));
        }
    }
}
#endif

int main() {
    std::vector<int> v = {2, 8, 6, 3, 3, 3, 3, 1, 9, 0};
     QuickSort(v, 0, v.size() - 1);

    for (auto num : v) {
        std::cout << num  << " ";
    }
}
