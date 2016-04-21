/*
 *  test Eigen program
 *
 */

#include <iostream>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <Eigen/Dense>
#include <Eigen/Sparse>

using namespace std;

namespace mp = boost::multiprecision;
using namespace mp;
typedef mp::cpp_dec_float_100 f100;

#define TYPE double
//#define TYPE f100

int main(){

	cout << fixed << setprecision(numeric_limits<TYPE>::digits10 + 1);

	Eigen::Vector3f a(1, 2, 3);
	Eigen::Vector3f b(2, 3, 4);

	cout << a.cross(b) << endl;

	Eigen::SparseMatrix<TYPE> A(5,5);

	return 0;
}
