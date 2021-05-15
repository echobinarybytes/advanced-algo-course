#include "data.h"
#include "timer.h"
#include <string>
#include <iostream>

using std::string;

int main() {
    for (int _i = 1; _i < 11; ++ _i) {
        // generator pointer set
        int i = _i * 1000;
        string if_name = "./data/" + string("points_") + std::to_string(i) +".txt";
        string of_name = "./res/" + string("convex-hull_points_") + std::to_string(i) + ".txt"; 
        Data data(if_name, of_name);

        if(!data.Generator(i)) {
            std::cout << if_name << " exist, use the exist file" << std::endl;
        } 


        // 凸包求解器求解问题
        Timer timer;
        timer.Start();

        timer.End();
    }



}

