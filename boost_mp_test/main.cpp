#include <iostream>
#include <boost/multiprecision/cpp_dec_float.hpp>

namespace mp = boost::multiprecision;

int main()
{
	mp::cpp_dec_float_100 x = 2.0f;
	mp::cpp_dec_float_100 y;

	mp::cpp_dec_float_100 result = mp::sqrt(x);

	y = result;

	std::cout << y << std::endl;

	std::cout << std::setprecision(std::numeric_limits<decltype(x)>::digits10 + 1)
			<< result;

	return 0;
}
