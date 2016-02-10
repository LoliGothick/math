#include <iostream>
#include "Linear_Algebra.hpp"
#include "LU_solver.hpp"

using namespace std;

const int dim = 512;

int main(){

	LA::matrix<double> A(dim);

	for(int i=0; i<dim; ++i){
		for(int j=0; j<dim; ++j){
			//A(i,j) = 1./(i+j+1);
			if(i==j) A(i,j) = 2.;
			if(i==(j-1) || i==(j+1)) A(i,j) = 1.;
		}
	}

	LA::matrix_LU<double> LU(A);

	LA::vector<double> b(dim);
	LA::vector<double> x(dim);

	b = 1.;
	x = 1.;

	b = A * x;
	x = 0.;

	LU.solve_linear_eq(b, x);

	x.show();

	return 0;
}
