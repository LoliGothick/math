#include <iostream>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include "../library/Linear_Algebra.hpp"
#include "../library/ODE_solver.hpp"
#include "../library/ratio.hpp"

using namespace std;

namespace mp = boost::multiprecision;
using namespace mp;
typedef mp::cpp_dec_float_100 f100;

#define TYPE f100

const int dim = 1;

template <typename T>
LA::vector<T> func(const LA::vector<T> &b){
	LA::vector<T> ans(b.dim);
	ans = math::ratio<T>(-1.0, 1.0) * b;
	return ans;
}

int main(){

	const TYPE dt = math::ratio<TYPE>(1.0, 1000);
	TYPE t = math::ratio<TYPE>(0.0, 1.0);

	LA::vector<TYPE> x(dim);

	x = math::ratio<TYPE>(1.0, 1.0);

	for(size_t i=0; t<100; ++i){
		t = i*dt;
		//cout << t << " ";
		//x.show();
		LA::RK_solver<TYPE>(x, func, dt);
	}

	return 0;
}
