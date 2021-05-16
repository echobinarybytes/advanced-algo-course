#ifndef _H_SET_H_
#define _H_SET_H_

#include <set>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>

class Set {
public:
    Set(int size = 0, std::string file_name = "") : _size(size), _file_name(file_name) {  }  
    Set(Set& s) {
        _set = s.GetSet(); 
        _size = s.Size();
    }
    ~Set() {  }
    Set& operator=(Set& s) { 
        _set = s.GetSet(); 
        _size = s.Size();
        return *this;
    }
public:
    void Generator() {
        // 如果文件存在，不产生直接读取文件
        if (_file_name != "") {
            std::ifstream ifd(_file_name, std::fstream::in);
            if (ifd) {
                ReadFromFile(ifd);
                ifd.close();
                return;
            }
        }

        // 产生数据
        std::default_random_engine e;
        std::uniform_int_distribution<int> uniform(e.min(), e.max());

        for (int i = 0; i < _size; ++ i) {
            _set.insert(uniform(e));

        }

    }

    // 将集合保存到文件
    void Store(std::string& name) {
        _file_name = name;
        std::ofstream ofd(name, std::fstream::out);
        for (auto iter = _set.begin(); iter != _set.end(); ++ iter) {
            ofd << *iter << "\n";
        }
    }

public:
    std::set<int>& GetSet() { return _set; }
    unsigned Size() { return _size; }
    std::string GetFileName() { return _file_name; }

private:
    void ReadFromFile(std::ifstream& ifd)  {// 从文件中读取数据，而不是进行生产数据
        int tmp;
        while (ifd >> tmp) {
            _set.insert(tmp);

        }
        _size = _set.size();
    }
        
    std::set<int> _set;
    unsigned _size;
    std::string _file_name;
};

class Sets {
public:
    // 当需要从文件中读取数据时，在这里制定文件的名字
    Sets(int size = 0,  std::string file_name = "") : _size(size) , _file_name(file_name){  }
    Sets(Sets& s) = delete;
    ~Sets();
    Sets& operator=(Sets& s) {
        _sets = s.GetSets();
        _size = s.GetSize();
        _file_name = s.GetFileName();
        return *this;
    }
public:
    // 产生子集合族
    void Generator(Set& s);

    // 将集合保存到文件
    void Store(std::string& name) {
        _file_name = name;
        std::ofstream ofd(name, std::fstream::out);
        for (auto setp : _sets) {
            bool first = true;
            for (auto num : *setp) {
                if (first) first = false;
                else ofd << " ";
                ofd << num ;
            }
            ofd << "\n";
        }
    }

public:
    std::vector<std::set<int>*>& GetSets() { return _sets; }
    unsigned GetSize()  { return _size; }
    std::string GetFileName() { return _file_name; }

private:
    void ReadFromFile(std::ifstream& ifd); // 从文件中读取数据，而不是进行生产数据
    std::vector<std::set<int>* > _sets;
    unsigned _size;
    std::string _file_name;
};

#endif
