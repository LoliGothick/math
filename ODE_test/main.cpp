#include <iostream>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include "../library/Linear_Algebra.hpp"
#include "../library/LU_solver.hpp"
//#include "../library/ODE_solver.hpp"

using namespace std;

namespace mp = boost::multiprecision;
using namespace mp;
typedef mp::cpp_dec_float_100 f100;

const int dim = 50;

template <typename T>
LA::vector<T> func(const LA::vector<T> &b){
	LA::vector<T> ans(b.dim);
	ans = -1.0 * b;
	return ans;
}

int main(){

	LA::vector<double> b(dim);

	b = func<double>(b);

	//LA::RK_solver<f100>(b, f);

	return 0;
}
