#include "FLPModel.h"
#include <numeric>
#include <iostream>

// construct model out of Facility location problem instance
FLPModel::FLPModel(const FLPInstance &inst) {
    n_ = inst.n();
    h_ = inst.h();

    try {
		model_ = IloModel(env_);
		cplex_ = IloCplex(model_);

		//creating variables
		y_ = IloBoolVarArray(env_, h_);
		for (unsigned int i = 0; i < h_; i++){
			sprintf(var, "y(%d)", i);
			y_[i].setName(var);
			model_.add(y_[i]);
		}

		x_ = IloBoolVarMatrix(env_, n_);
		for (unsigned int i=0; i < n_; i++) {
			x_[i] = IloBoolVarArray(env_, h_);
			for(unsigned int j = 0; j < h_; ++j){

				sprintf(var, "x(%d,%d)", i, j);
				x_[i][j].setName(var);
				model_.add(x_[i][j]);
			}			
		}
		// objective function

		IloExpr objFunction(env_);
		//minimize cost of opening
		for (unsigned int j = 0; j < h_; j++){
        	objFunction += inst.c(j) * y_[j];
		}
		
		//minimize total distance
		// for (unsigned int i = 0; i < n_; i++){
		// 	for (unsigned int j = 0; j < h_; j++){
		// 		objFunction += inst.d(i,j) * x_[i][j];
		// 	}
		// }

		model_.add(IloMinimize(env_, objFunction));

		// constraint: all customers must be served
		for (unsigned int i = 0; i < n_; i++) {
			IloExpr expr(env_);
			for (unsigned int j = 0; j < h_; j++) {
				expr += x_[i][j];
			}
			sprintf (var, "Constraint1_%d", i);
			IloRange cons = (expr == 1);
			cons.setName(var);
			model_.add(cons);
		}

		// constraint: the capacity of a facility cannot be surpassed
		for (unsigned int j = 0; j < h_; j++) {
			IloExpr expr1(env_);
			IloExpr expr2(env_);
			for (unsigned int i = 0; i < n_; i++) {
				expr1 += inst.q(i)*x_[i][j];
			}
			expr2 = inst.Q(j)*y_[j];

			sprintf (var, "Constraint2_%d", j);
			IloRange cons = (expr1 - expr2 <= 0);
			cons.setName(var);
			model_.add(cons);
		}
    } catch (IloException &e) {
		cerr << "Error creating model: " << e.getMessage() << endl;
		e.end();
    }
	cplex_.exportModel("flp1.lp");
}

int FLPModel::solve() {
	int opt = -1;
	cout << "In solve" << endl;
	getchar();
	try {
		auto start = cplex_.getTime();
		cplex_.solve();
		auto elapsed = cplex_.getTime() - start;
		if (cplex_.getStatus() == IloAlgorithm::Optimal) {
			opt = cplex_.getObjValue();
			cout << "Solution found in \t" << elapsed << " seconds" << endl;
			for (unsigned int j = 0; j < h_; j++) {
				if (cplex_.getValue(y_[j]) > 0.5) {
					cout << "\tFacility " << j << ":";
					for (unsigned int i = 1; i < n_; i++) {
						if (cplex_.getValue(x_[i][j]) > 0.5) {
							cout << " " << i;
						}
					}
					cout << endl;
				}
			}
		}
		else {
			cout << "No feasible solution found" << endl;
		}
	}
	catch (IloException& e) {
		cerr << "Error solving model: " << e.getMessage() << endl;
		e.end();
	}
	return opt;
}

