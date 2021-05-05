#ifndef _PARETO_H
#define _PARETO_H

#include <vector>
#include <ilconcert/ilomodel.h>
#include <ilcplex/ilocplex.h>

#include "FLPInstance.h"
#include "FLPModel.h"

using namespace std;

// 2-dimension bool variable table
typedef IloArray<IloBoolVarArray> IloBoolVarMatrix;

class Pareto {
protected:
    FLPModel model1_;
    FLPModel model2_;


public:
    Pareto(FLPModel model1, FLPModel model2, const FLPInstance& instance);

    // solve this model
    vector<int> solve_pareto(const FLPInstance& inst);

};

#endif

