#ifndef _FLPMODEL_H
#define _FLPMODEL_H

#include <vector>
#include <ilconcert/ilomodel.h>
#include <ilcplex/ilocplex.h>

#include "FLPInstance.h"

using namespace std;

// 2-dimension bool variable table
typedef IloArray<IloBoolVarArray> IloBoolVarMatrix;

class FLPModel {
protected:
	char var[100];
    int n_; // number of customers
    int h_; //number of facilities


    // CPLEX model section
    IloEnv env_;   // CPLEX environment (required)
    IloCplex cplex_; // CPLEX object (required)
    IloModel model_; // actual model object
    IloBoolVarArray y_; // y decision variables
    IloBoolVarMatrix x_; // x decision variables

public:
    // construct model out of FLP instance
    FLPModel(const FLPInstance &inst);
    FLPModel() {};

    // solve this model
    int solve();
    IloRange addConstraint(IloExpr leftside, int rightside);
    void addObjective(IloExpr Objective);
    void removeConstraint(IloRange cons);
    IloEnv env() const { return env_; };
    IloBoolVar y(int i) const { return y_[i]; }; // y decision variables
    IloBoolVar x(int i,int j) const { return x_[i][j]; }; // x decision variables

};

#endif
