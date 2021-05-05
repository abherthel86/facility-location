#include "FLPInstance.h"
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

    flpm.epsilon(inst);

}
