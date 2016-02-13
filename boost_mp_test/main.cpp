#include <iostream>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include "../library/Linear_Algebra.hpp"
#include "../library/LU_solver.hpp"

using namespace std;

namespace mp = boost::multiprecision;
using namespace mp;

typedef mp::cpp_dec_float_100 f100;

const int dim = 50;

int main()
{
	f100 a("1.0");

	LA::vector<f100> b(dim);
	LA::vector<f100> x(dim);

	x = a;

	LA::matrix<f100> A(dim);

	for(int i=0; i<dim; ++i){
		for(int j=0; j<dim; ++j){
			A(i,j) = (f100)1./(i+j+1);
		}
	}
	
	b = A * x;

	x = (f100)0.;

	LA::matrix_LU<f100> LU(A);

	LU.solve_linear_eq(b, x);

	x.show();

	//cout << setprecision(numeric_limits<decltype(x)>::digits10 ) << endl;
	cout << numeric_limits<decltype(a)>::digits10 << endl;

	return 0;
}
