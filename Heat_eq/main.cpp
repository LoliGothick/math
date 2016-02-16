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

const int dim = 10;

int main(){

	//cout << fixed << setprecision(numeric_limits<TYPE>::digits10 + 1);

	LA::vector<TYPE> u(dim);

	return 0;
}
