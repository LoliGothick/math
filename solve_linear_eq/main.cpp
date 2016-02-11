#include <iostream>
#include "../library/Linear_Algebra.hpp"
#include "../library/LU_solver.hpp"
#include <boost/multiprecision/cpp_dec_float.hpp>

namespace mp = boost::multiprecision;

using namespace std;

const int dim = 200;

#define TYPE double //mp::cpp_dec_float_100

int main(){

	LA::matrix<TYPE> A(dim);

	for(int i=0; i<dim; ++i){
		for(int j=0; j<dim; ++j){
			//A(i,j) = 1./(i+j+1);
			if(i==j) A(i,j) = 2.0;
			if(i==(j-1) || i==(j+1)) A(i,j) = 1.0;
		}
	}

	LA::matrix_LU<TYPE> LU(A);

	LA::vector<TYPE> b(dim);
	LA::vector<TYPE> x(dim);

	b = 1.0;
	x = 1.0;

	b = A * x;
	x = 0.0;

	LU.solve_linear_eq(b, x);

	x.show();

	return 0;
}
