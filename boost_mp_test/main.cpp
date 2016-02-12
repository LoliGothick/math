#include <iostream>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include "../library/Linear_Algebra.hpp"

using namespace std;

namespace mp = boost::multiprecision;
using namespace mp;

typedef mp::cpp_dec_float_100 f100;

const int dim = 10;

int main()
{
	f100 x("0.1");
	f100 y;

	LA::vector<f100> b(dim);

	y = x;

	b = y;
	b.show();

	//cout << setprecision(numeric_limits<decltype(x)>::digits10 + 1) << endl;
	//cout << numeric_limits<decltype(x)>::digits10 << endl;

	return 0;
}
