#include "FLPInstance.h"
#include "Pareto.h"
#include "FLPModel.h"

#include <iostream>
#include <vector>
#include <cstdlib>


using namespace std;

int main(int argc, char **argv) {
    // input file: default or first command line argument
    string input_file = "Instances/flp1.txt";
    if (argc > 1) {
        input_file = argv[1];
    }

    // now we run the main stuff
    FLPInstance inst(input_file);

    FLPModel flpm(inst);
    FLPModel flpm2(inst);
    Pareto s(flpm, flpm2, inst);
    vector<int> solution = s.solve_pareto(inst);
    for (int i = 0; i < solution.size() - 1;i++) {
        cout << "Objective 1: " << solution[i] << ", Objective 2 " << solution[i + 1] << endl;
        i++;
    }
    getchar();


}
