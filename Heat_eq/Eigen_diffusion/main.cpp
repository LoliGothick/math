/*
 *  soleve Heat Equation
 *	periodic boundary condition
 *  x in [0,1]
 *
 *  Takaaki MINOMO
 */

#include <iostream>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <Eigen/Sparse>
#include <Eigen/Dense>
#include "../../library/ratio.hpp"

using namespace std;

namespace mp = boost::multiprecision;
using namespace mp;
typedef mp::cpp_dec_float_100 f100;

#define TYPE double
//#define TYPE f100

static constexpr int INTV = 5;

const int dim = 224;
const TYPE dx = math::ratio<TYPE>(1, dim);
//const TYPE dt = math::ratio<TYPE>(1, 1000);
const TYPE dt = math::ratio<TYPE>(1,6)*dx*dx;
//const TYPE PI = acos(static_cast<TYPE>(-1.0));
const TYPE k  = math::ratio<TYPE>(0,dx);


template <typename T>
T phi_func(int i,T x){
	if((i-1)*dx < x && x < (i+1)*dx){
		return math::ratio(1, 1) - abs(x - i*dx)/dx;
	}else{
		return math::ratio(0, 1);
	}
	cout << "ERROR 32522" << endl;
	return -1;
}

template<typename T>
using Vector = Eigen::Matrix<T, dim, 1>;

template<typename T>
void init(Vector<T> &u){
//void init(Eigen::Matrix<T, dim, 1> &u){
	T a = math::ratio<T>(8, 10);
	for(auto i=0; i<dim; i++){
		//u(i) = sin(PI*i*dx);
		u(i) = math::ratio<T>(10, 10)*exp(-100*(dx*i-a)*(dx*i-a));
	}
}

template <typename T>
//Eigen::Matrix<T, dim, 1> func(Eigen::SimplicialCholesky<Eigen::SparseMatrix<T> > &solver, Eigen::SparseMatrix<T> &K, Eigen::Matrix<T, dim, 1> &u){
Eigen::Matrix<T, dim, 1> func(auto &solver, Eigen::SparseMatrix<T> &K, Eigen::Matrix<T, dim, 1> &u){
	return math::ratio<T>(-1, 1) * solver.solve(K * u) - k * u;
}

int main(){

	cout << fixed << setprecision(numeric_limits<TYPE>::digits10 + 1);

	Eigen::Matrix<TYPE, dim, 1> u = Eigen::Matrix<TYPE, dim, 1>::Zero();

	//cout << b << endl;

	Eigen::SparseMatrix<TYPE> M(dim, dim);
	Eigen::SparseMatrix<TYPE> K(dim, dim);
	
	/*******************************/
	/*      configure matrix       */
	/*******************************/
	
	init(u);

	vector<Eigen::Triplet<TYPE> > M_coef;
	vector<Eigen::Triplet<TYPE> > K_coef;

	for(auto i=0; i<dim; ++i){
		M_coef.push_back(Eigen::Triplet<TYPE>(i, i, static_cast<TYPE>(math::ratio<TYPE>(2, 3) * dx)));
		K_coef.push_back(Eigen::Triplet<TYPE>(i, i, static_cast<TYPE>(math::ratio<TYPE>(2, 1) / dx)));
		//M_coef.push_back(Eigen::Triplet<TYPE>(i, i, static_cast<TYPE>(math::ratio<TYPE>(2, 3))));
		//K_coef.push_back(Eigen::Triplet<TYPE>(i, i, static_cast<TYPE>(math::ratio<TYPE>(2, 1))));
		
		M_coef.push_back(Eigen::Triplet<TYPE>(i, (i+1)%dim, static_cast<TYPE>(math::ratio<TYPE>( 1, 6) * dx)));
		M_coef.push_back(Eigen::Triplet<TYPE>((i+1)%dim, i, static_cast<TYPE>(math::ratio<TYPE>( 1, 6) * dx)));
		K_coef.push_back(Eigen::Triplet<TYPE>(i, (i+1)%dim, static_cast<TYPE>(math::ratio<TYPE>(-1, 1) / dx)));
		K_coef.push_back(Eigen::Triplet<TYPE>((i+1)%dim, i, static_cast<TYPE>(math::ratio<TYPE>(-1, 1) / dx)));
		//M_coef.push_back(Eigen::Triplet<TYPE>(i, (i+1)%dim, static_cast<TYPE>(math::ratio<TYPE>( 1, 6))));
		//M_coef.push_back(Eigen::Triplet<TYPE>((i+1)%dim, i, static_cast<TYPE>(math::ratio<TYPE>( 1, 6))));
		//K_coef.push_back(Eigen::Triplet<TYPE>(i, (i+1)%dim, static_cast<TYPE>(math::ratio<TYPE>(-1, 1))));
		//K_coef.push_back(Eigen::Triplet<TYPE>((i+1)%dim, i, static_cast<TYPE>(math::ratio<TYPE>(-1, 1))));
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
	Eigen::SimplicialCholesky<Eigen::SparseMatrix<TYPE> > solver(M);	

	solver.analyzePattern(M);
	solver.factorize(M);

	FILE *gp;
	gp = popen("gnuplot -persist", "w");
	fprintf(gp, "set xr [0:1]\n");
	fprintf(gp, "set yr [0:1]\n");
	//fprintf(gp, "set size square\n");
	fprintf(gp, "set grid\n");

	TYPE t = math::ratio<TYPE>(0, 1);
	
	for(auto i=0; ; i++){
	//for(auto i=0; i<100000.; i++){
		t = i*dt;

		u = u + dt * func<TYPE>(solver, K, u);
		
		if(i%INTV == 0){
		//if(false){
			TYPE x = 0.;
			fprintf(gp, "plot '-' w l lw 1\n");
			for(auto j=0; j<dim; j++){
				double a = static_cast<double>(x);
				double b = static_cast<double>(u(j));
				
				fprintf(gp, "%f %f \n", a, b);
				x += dx;
			}
			fprintf(gp, "%f %f \n", static_cast<double>(math::ratio<TYPE>(1, 1)), static_cast<double>(u(0)));
			fprintf(gp, "e\n");
			fflush(gp);
		}
		
	}

	pclose(gp);
	
	return 0;
}
