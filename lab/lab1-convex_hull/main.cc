#include "data.h"
#include "timer.h"
#include <string>
#include <iostream>
#include "convex_hull_solver.h"

using std::string;

int main() {
#if 1 // for real
    for (int _i = 1; _i < 20; ++ _i) {
#else // for test
    for (int _i = 1; _i < 2; ++ _i) {
#endif
        // generator pointer set
        int i = _i * 1000;
        string if_name = "./data/" + string("points_") + std::to_string(i) +".txt";
        string of_name = "./res/" + string("convex-hull_points_") + std::to_string(i) + ".txt"; 
        Data data(if_name, of_name);

        if(!data.Generator(i)) {
            std::cout << if_name << " exist, use the exist file" << std::endl;
        } 


        ConvexHullSolver solver(data);
        // 凸包求解器求解问题
        Timer timer;
        timer.Start();
        solver.SolverBasedEnum();
        timer.End();
        std::cout << "Enum based : " << i <<  std::endl;
        timer.Show();

        solver.SaveConvexHullSet(solver.GetStoreFileName() + "_by_enum");

        // 凸包求解器求解问题
        timer.Start();
        solver.SolverBasedGrahamScan();
        timer.End();
        std::cout << "GrahamScan based : " << i <<  std::endl;
        timer.Show();

        solver.SaveConvexHullSet(solver.GetStoreFileName() + "_by_grahamscan");

        // divide and conqure
        timer.Start();
        solver.SolverBasedDAndQ();
        timer.End();
        std::cout << "DAndQ based : " << i <<  std::endl;
        timer.Show();

        solver.SaveConvexHullSet(solver.GetStoreFileName() + "_by_dandq");
    }



}

