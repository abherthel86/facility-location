#include "FLPModel.h"
#include <numeric>
#include <iostream>

// construct model out of Facility location problem instance
FLPModel::FLPModel(const FLPInstance &inst) {
    n_ = inst.n();
    h_ = inst.h();
	int opt = 0;
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
		//objective functions
		//minimize cost of opening
		IloExpr expr1(env_);
		for (unsigned int j = 0; j < h_; j++){
        	expr1 += inst.c(j) * y_[j];
		}

		objF_1 = IloMinimize(env_, expr1);

		// minimize total distance
		IloExpr expr2(env_);
		for (unsigned int i = 0; i < n_; i++){
			for (unsigned int j = 0; j < h_; j++){
				expr2 += inst.d(i,j) * x_[i][j];
			}
		}
		objF_2 = IloMinimize(env_, expr2);

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
	cplex_.setOut(env_.getNullStream());
}

int FLPModel::solve() {
	int opt = -1;
	try {
		auto start = cplex_.getTime();
		cplex_.solve();
		auto elapsed = cplex_.getTime() - start;
		solving_time.push_back(elapsed);
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

void FLPModel::epsilon(const FLPInstance &inst) {
	vector< pair<int, int> > opt_sol;
	pair<int, int> opt_pair;

	double epsilon = IloInfinity;
	int omega = 1;

	int z1, z2;
	int f1, f2;

	int count = 0;
	
	IloExpr expr_1 = objF_1.getExpr();
	IloExpr expr_2 = objF_2.getExpr();

	IloRange cons_f1;
	IloRange cons_f2;

	sprintf (var_2, "ConstraintF2");
	cons_f2 = (expr_2 <= epsilon);
	cons_f2.setName(var_2);
	model_.add(cons_f2);


	while (true) {

		model_.add(objF_1);

		z1 = solve();
		if (cplex_.getStatus() != IloAlgorithm::Optimal){
			break;
		}

		//changing the OF
		model_.remove(objF_1);

		model_.add(objF_2);

		//bounding f1
		sprintf (var_1, "ConstraintF1");
		IloRange cons_f1 = (expr_1 <= z1);
		cons_f1.setName(var_1);
		model_.add(cons_f1);

		z2 = solve();
		if (cplex_.getStatus() != IloAlgorithm::Optimal){
			break;
		}

		//collect the objective values
		opt_pair.first = z1;
		opt_pair.second = z2;

		opt_sol.push_back(opt_pair);

		model_.remove(cons_f1);

		cplex_.exportModel("flp1.lp");

		epsilon = z2 - omega;

		//changing the OF
		model_.remove(objF_2);

		//update upper bound constraint on f2
		cons_f2.setUB(epsilon);
	}

	// print the pareto frontier
	cout << "f1: cost" << "\tf2: dist" << endl;
	for (int i = 0; i < opt_sol.size(); ++i)
	{
		cout << opt_sol[i].first << "\t\t" << opt_sol[i].second << endl;
	}

}