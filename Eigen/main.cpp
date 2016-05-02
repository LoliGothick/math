/*
 *  test Eigen program
 *
 */

#define BOOST_NO_CXX11_HDR_ARRAY
#include <iostream>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <Eigen/Sparse>
#include <Eigen/Dense>

using namespace std;

namespace mp = boost::multiprecision;
using namespace mp;
using f100 = mp::cpp_dec_float_100;

#define TYPE double
//#define TYPE f100

const int N = 10000;

int main(){

	cout << fixed << setprecision(numeric_limits<TYPE>::digits10 + 1);

// vector test

/*
	Eigen::Vector3f a(1, 2, 3);
	Eigen::Vector3f b(2, 3, 4);

	cout << a.cross(b) << endl;
*/

// matrix test

/*
	Eigen::Matrix<TYPE, N, N> A = Eigen::Matrix<TYPE, N, N>::Random(N, N);
	Eigen::Matrix<TYPE, N, 1> b = Eigen::Matrix<TYPE, N, 1>::Random(N, 1);

	cout << A << endl;
	cout << "" << endl;

	Eigen::Matrix<TYPE, N, 1> x = A.fullPivLu().solve(b);

	cout << (A*x-b).norm() << endl;
*/	

	Eigen::SparseMatrix<TYPE> A(N, N);
	Eigen::Matrix<TYPE, N, 1> b = Eigen::Matrix<TYPE, N, 1>::Random(N, 1);
	Eigen::Matrix<TYPE, N, 1> x = Eigen::Matrix<TYPE, N, 1>::Random(N, 1);
	
	vector<Eigen::Triplet<TYPE> > d;

	for(auto i=0; i<N; ++i){
		d.push_back(Eigen::Triplet<TYPE>(i, i, 2.0));
	}
	
	for(auto i=0; i<N-1; ++i){
		d.push_back(Eigen::Triplet<TYPE>(i, i+1, -1.0));
		d.push_back(Eigen::Triplet<TYPE>(i+1, i, -1.0));
	}

	A.setFromTriplets(d.begin(), d.end());

	Eigen::SimplicialCholesky<Eigen::SparseMatrix<TYPE> > solver(A);	
	//Eigen::SimplicialLLT<Eigen::SparseMatrix<TYPE> > solver(A);	
	//Eigen::SimplicialLDLT<Eigen::SparseMatrix<TYPE> > solver(A);	
	//Eigen::BiCGSTAB<Eigen::SparseMatrix<TYPE> > solver(A);
	//Eigen::ConjugateGradient<Eigen::SparseMatrix<TYPE> > solver(A);

	x = solver.solve(b);
	cout << (A*x-b).norm() << endl;

	return 0;
}
