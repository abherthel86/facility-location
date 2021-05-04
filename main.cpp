#include "FLPInstance.h"
// #include "FLPSolution.h"
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
    // random number generation initialisation
    // int seed = 1;
    // srandom(seed);
    // cout << "Random seed: " << seed << endl;

    // now we run the main stuff
    FLPInstance inst(input_file);

    FLPModel flpm(inst);
    // FLPSolution s(inst);

    // s.cheapestInsertion(inst);
    // cout << "Solution after construction:" << endl << s << endl;
    // s.two_opt_all_routes(inst);
    // cout << endl << "Solution after 2-opt:" << endl << s << endl;
    //
}
