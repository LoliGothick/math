/*
 *  soleve Heat Equation
 *	periodic boundary condition
 *  x in [0,1] (then u(0) == u(dim))
 *
 *  writen by Takaaki MINOMO
 */

#include <iostream>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <Eigen/Sparse>
#include <Eigen/Dense>

using namespace std;

namespace mp = boost::multiprecision;
using namespace mp;
using f100 = mp::cpp_dec_float_100;

//using TYPE = double;
using TYPE = f100;

static constexpr int INTV = 20;
static constexpr int dim  = 1024;

template <typename T>
constexpr T ratio(const T &a, const T &b){
	return static_cast<T>(a/b);
}

const TYPE dx = ratio<TYPE>(1, dim);
const TYPE dt = ratio<TYPE>(1, 10*dim);
const TYPE k  = ratio<TYPE>(1, 1);
//const TYPE PI = acos(static_cast<TYPE>(-1.0));

template <typename T>
void init(Eigen::Matrix<T, dim, 1> &u){
	T a = ratio<T>(8, 10);
	for(auto i=0; i<dim; i++){
		//u(i) = sin(PI*i*dx);
		u(i) = ratio<T>(1, 1)*exp(-100*(dx*i-a)*(dx*i-a));
	}
}
int main(){

	cout << fixed << setprecision(numeric_limits<TYPE>::digits10 + 1);

	Eigen::Matrix<TYPE, dim, 1> u = Eigen::Matrix<TYPE, dim, 1>::Zero();

	Eigen::SparseMatrix<TYPE> M(dim, dim);
	Eigen::SparseMatrix<TYPE> K(dim, dim);
	
	/*******************************/
	/*      configure matrix       */
	/*******************************/
	
	init(u);

	vector<Eigen::Triplet<TYPE> > M_coef;
	vector<Eigen::Triplet<TYPE> > K_coef;

	for(auto i=0; i<dim; ++i){
		M_coef.push_back(Eigen::Triplet<TYPE>(i, i, static_cast<TYPE>(ratio<TYPE>(2, 3) * dx)));
		K_coef.push_back(Eigen::Triplet<TYPE>(i, i, static_cast<TYPE>(ratio<TYPE>(2, 1) / dx)));
		
		M_coef.push_back(Eigen::Triplet<TYPE>(i, (i+1)%dim, static_cast<TYPE>(ratio<TYPE>( 1, 6) * dx)));
		M_coef.push_back(Eigen::Triplet<TYPE>((i+1)%dim, i, static_cast<TYPE>(ratio<TYPE>( 1, 6) * dx)));
		K_coef.push_back(Eigen::Triplet<TYPE>(i, (i+1)%dim, static_cast<TYPE>(ratio<TYPE>(-1, 1) / dx)));
		K_coef.push_back(Eigen::Triplet<TYPE>((i+1)%dim, i, static_cast<TYPE>(ratio<TYPE>(-1, 1) / dx)));
	}

	M.setFromTriplets(M_coef.begin(), M_coef.end());
	K.setFromTriplets(K_coef.begin(), K_coef.end());

	/*******************************/
	/*        solving method       */
	/*******************************/

	//Eigen::SimplicialLLT<Eigen::SparseMatrix<TYPE> >      solver(M);	
	//Eigen::SimplicialLDLT<Eigen::SparseMatrix<TYPE> >     solver(M);	
	//Eigen::BiCGSTAB<Eigen::SparseMatrix<TYPE> >           solver(M);
	//Eigen::ConjugateGradient<Eigen::SparseMatrix<TYPE> >  solver(M);
	//Eigen::SparseLU<Eigen::SparseMatrix<TYPE> >           solver(M);	
	//Eigen::SimplicialCholesky<Eigen::SparseMatrix<TYPE> > solver(M);	
	
	
	Eigen::SparseMatrix<TYPE> C = ratio<TYPE>(2, 1)*M + dt*K + dt*k*M;
	
	Eigen::SimplicialCholesky<Eigen::SparseMatrix<TYPE> > solver(C);

	solver.analyzePattern(C);
	solver.factorize(C);
	
	FILE *gp;
	gp = popen("gnuplot -persist", "w");
	fprintf(gp, "set xr [0:1]\n");
	fprintf(gp, "set yr [0:1]\n");
	//fprintf(gp, "set size square\n");
	fprintf(gp, "set grid\n");

	TYPE t = ratio<TYPE>(0, 1);
	
	for(auto i=0;  t<10; i++){
		t = static_cast<TYPE>(i*dt);

		u = solver.solve((ratio<TYPE>(2, 1)*M - dt*K - dt*k*M)*u);

		if(i%INTV == 0){
			TYPE x = 0.;
			fprintf(gp, "plot '-' w l lw 1\n");
			for(auto j=0; j<dim; j++){
				double a = static_cast<double>(x);
				double b = static_cast<double>(u(j));
				
				fprintf(gp, "%f %f \n", a, b);
				x += dx;
			}
			fprintf(gp, "%f %f \n", static_cast<double>(ratio<TYPE>(1, 1)), static_cast<double>(u(0)));
			fprintf(gp, "e\n");
			fflush(gp);
		}
	}

	pclose(gp);
	
	return 0;
}
