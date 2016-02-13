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
LA::vector<T> func(LA::vector<T> &b){
	LA::vector<T> ans = b;
	return ans;
}

int main(){
	f100 a("1.0");

	LA::vector<f100> b(dim);

	b = (f100)a;

	b = func<f100>(b);
	cout << __LINE__ << endl;

	//LA::RK_solver<f100>(b, f);

	return 0;
}
