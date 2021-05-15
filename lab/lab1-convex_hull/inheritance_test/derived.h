#ifndef _H_DERIVED_H_
#define _H_DERIVED_H_

#include "./base.h"
#include <iostream>

class Derived : public Base {

    Derived() { std::cout << "in derived" << std::endl; }

    void SetVal(int num = 1) { foo = num; }

};
#endif
