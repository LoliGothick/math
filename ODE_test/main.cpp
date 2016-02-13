#include <iostream>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include "../library/Linear_Algebra.hpp"
#include "../library/ODE_solver.hpp"

using namespace std;

namespace mp = boost::multiprecision;
using namespace mp;
typedef mp::cpp_dec_float_100 f100;

const int dim = 1;

template <typename T>
LA::vector<T> func(const LA::vector<T> &b){
	LA::vector<T> ans(b.dim);
	ans = - 1.0 * b;
	return ans;
}

int main(){

	const double dt = 0.001;

	LA::vector<double> b(dim);

	b = 1.0;

	for(size_t i=0; ; ++i){
		LA::RK_solver<double>(b, func, dt);
		b.show();
	}

	return 0;
}
