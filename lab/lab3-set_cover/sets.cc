#include "sets.h"
#include <assert.h>

void Sets::Generator(Set& s) {
    // 如果文件存在，不产生直接读取文件
    if (_file_name != "") {
        std::ifstream ifd(_file_name, std::fstream::in);
        if (ifd) {
            ReadFromFile(ifd);
            ifd.close();
            return;
        }
    }

    // s.Size(); 采用vector的话，删除操作用时过多，TODO:需要采用更好的结构
    std::vector<int> set_vec;
    std::vector<int> notselected_idx_pool(s.Size(), 0); // 存储的是set_vec中没有被选择的序号, 序号是set_vec的索引
    std::vector<int> selected_idx_pool; // 存储的是set_vec中已经被随机选到的序号
    auto iter = s.GetSet().begin();
    for (int i = 0; i < notselected_idx_pool.size(); ++ i, ++ iter) {
        set_vec.push_back(*iter);
        notselected_idx_pool[i] = i;
    }


    std::default_random_engine e;
    std::uniform_int_distribution<int> uniform(0, e.max());
    // 按照规则产生子集集族
    // 保证生成一组可行解集合
    // 假定s.size()的取值为100, 1000, 5000
    std::set<int>* set_p = new std::set<int>;

    //生成 _sets[0]的元素
    for (int i = 0; i < 20; ++ i) {
        // GeneratorSet(idx)
        unsigned idx = uniform(e) % notselected_idx_pool.size(); // 选择notselected_idx_pool的索引
        set_p->insert(set_vec[notselected_idx_pool[idx]]); // 选择 set_vec的索引
        selected_idx_pool.push_back(notselected_idx_pool[idx]); // 保存选择了的set_vec的索引
        notselected_idx_pool.erase(notselected_idx_pool.begin() + idx); // 删除notselected idx pool中的元素
    }
    _sets.push_back(set_p);

    // 生成_sets[1], _sets[2], ..., _sets[j] 
    while (notselected_idx_pool.size() >= 20) {
        int n = uniform(e) % 20 + 1; // 产生1-20的随机数字
        int x = uniform(e) % n + 1; // 产生1-n的随机数
        set_p = new std::set<int>;

        // 从notselected_idx_pool中选择元素
        for (int i = 0; i < x; ++ i) {
            unsigned idx = uniform(e) % notselected_idx_pool.size(); // 选择notselected_idx_pool的索引
            set_p->insert(set_vec[notselected_idx_pool[idx]]); // 选择 set_vec的索引
            selected_idx_pool.push_back(notselected_idx_pool[idx]); // 保存选择了的set_vec的索引
            notselected_idx_pool.erase(notselected_idx_pool.begin() + idx); // 删除notselected idx pool中的元素
        }

        // 从selected_idx_pool中选择元素
        for (int i = 0; i < n - x; ++ i) {
            unsigned idx = uniform(e) % selected_idx_pool.size(); // 选择selected_idx_pool的索引
            set_p->insert(set_vec[selected_idx_pool[idx]]); // 选择 set_vec的索引
        }
        _sets.push_back(set_p);

    }

    if (notselected_idx_pool.size() != 0) {
        set_p = new std::set<int>;
        for (int i = 0; i < notselected_idx_pool.size(); ++ i) {
            set_p->insert(set_vec[notselected_idx_pool[i]]); // 选择 set_vec的索引
            selected_idx_pool.push_back(notselected_idx_pool[i]); // 保存选择了的set_vec的索引
        }
        notselected_idx_pool.clear(); 
        _sets.push_back(set_p);

    }
    assert(notselected_idx_pool.size() == 0);

    int left_set_to_generate = _size - _sets.size();
    for (int i = 0; i < left_set_to_generate; ++ i) {
        set_p = new std::set<int>;
        int cnt = uniform(e) % 30 + 1; // 生成的集合的大小为 1-30
        for (int j = 0; j < cnt; ++ j) {
            int idx = uniform(e) % set_vec.size();
            set_p->insert(set_vec[idx]); // 选择 set_vec的索引
        }
        _sets.push_back(set_p);

    }
}

#include <sstream>
void Sets::ReadFromFile(std::ifstream& ifd) {
    std::string line;
    std::set<int>* set_p = nullptr;
    while (getline(ifd, line)) { // 读取每行, 一行即是一个集合
        set_p = new std::set<int>;
        std::stringstream ss(line);
        int tmp;
        while (ss >> tmp) { // 解析每行
            set_p->insert(tmp);
        }
        _sets.push_back(set_p);

    }
    _size = _sets.size();
}


// 回收动态分配的内存空间
Sets::~Sets() {
    // TODO
}
