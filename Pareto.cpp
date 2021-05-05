#include "Pareto.h"
#include "FLPModel.h"
#include "FLPInstance.h"
#include <numeric>
#include <iostream>

Pareto::Pareto(FLPModel model1, FLPModel model2, const FLPInstance& instance) {
	model1_ = model1;
	model2_ = model2;
}

vector <int> Pareto::solve_pareto(const FLPInstance& inst) {
	vector<int> solution;
	int opt = -1;
	int epsilon = 631; // max costs of opening a facility
	int omega = 1; //min reduction in costs for not opening a facility
	IloExpr expr(model1_.env());
	IloRangeArray cons_array1(model1_.env());
	IloRangeArray cons_array2(model2_.env());
	//f2 < epsilon
	for (unsigned int j = 0; j < inst.h(); j++) {
		expr += inst.c(j) * model1_.y(j);
	}
	IloRange cons1 = model1_.addConstraint(expr, epsilon);
	IloRange cons2 = model2_.addConstraint(expr, epsilon);
	cons_array1.add(cons1);
	cons_array2.add(cons2);


	IloExpr objFunction1(model1_.env());
	//minimize total distance f1
	for (unsigned int i = 0; i < inst.n(); i++) {
		for (unsigned int j = 0; j < inst.h(); j++) {
			objFunction1 += inst.d(i, j) * model1_.x(i,j);
		}

	}
	model1_.addObjective(objFunction1);

	IloExpr objFunction2(model2_.env());
		//minimize cost of opening
	for (unsigned int j = 0; j < inst.h(); j++){
		objFunction2 += inst.c(j) * model2_.y(j);
	}

	model2_.addObjective(objFunction2);

	int count = 0;
	while (epsilon > 62) {
		int f1 = model1_.solve();
		cout << "f1 : " << f1 << endl;
		solution.push_back(f1);
		IloRange cons3 = model2_.addConstraint(objFunction1, f1);
		cons_array2.add(cons3);
		count += 1;
		int f2 = model2_.solve();
		cout << "f2 : " << f2 << endl;
		solution.push_back(f2);
		model2_.removeConstraint(cons_array2[count]);
		epsilon = f2 - omega;
		cons_array2[0].setUB(epsilon);
		cons_array1[0].setUB(epsilon);

	}
	return solution;
}

