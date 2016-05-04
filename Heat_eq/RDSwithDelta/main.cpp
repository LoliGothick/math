/*
 *  soleve reaction diffusion system with delta function
 *	periodic boundary condition
 *  x in [0,1] (then u(0) == u(dim))
 *
 *  writen by Takaaki MINOMO
 */

#include <iostream>
#include <random>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <Eigen/Sparse>
#include <Eigen/Dense>

using namespace std;

namespace mp = boost::multiprecision;
using namespace mp;
using f100 = mp::cpp_dec_float_100;

using TYPE = double;
//using TYPE = f100;

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
T phi_func(int i,T x){
	if((i-1)*dx < x && x < (i+1)*dx){
		return 1. - abs(x - i*dx)/dx;
	}else{
		return 0;
	}
	cout << "ERROR 32522" << endl;
	return -1;
}

template <typename T>
void init(Eigen::Matrix<T, dim, 1> &u){
	T a = ratio<T>(8, 10);
	for(auto i=0; i<dim; i++){
		//u(i) = sin(PI*i*dx);
		u(i) = ratio<T>(1, 1)*exp(-100*(dx*i-a)*(dx*i-a));
	}
}
int main(){

	constexpr int n = 10;

	auto *delta = new TYPE[n];
	auto *c     = new TYPE[n];
	delta[0] = static_cast<TYPE>(0.5);
	c[0]     = static_cast<TYPE>(5.5);
	delta[1] = static_cast<TYPE>(0.8);
	c[1]     = static_cast<TYPE>(3.5);


    std::random_device rnd;     // 非決定的な乱数生成器を生成
	std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_real_distribution<> rand100(0., 1.);        // [0, 99] 範囲の一様乱数
	for(int i = 0; i < n; ++i) {
		delta[i] = rand100(mt);
		c[i]     = rand100(mt);
	}
	
	Eigen::Matrix<TYPE, dim, 1> phi = Eigen::Matrix<TYPE, dim, 1>::Zero();

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
	//fprintf(gp, "set yr [0:1]\n");
	//fprintf(gp, "set size square\n");
	fprintf(gp, "set grid\n");

	TYPE t {};
	
	for(auto i=0;  t<2; i++){
		t = static_cast<TYPE>(i*dt);

		for(auto j=0; j<dim; ++j){
			for(auto k=0; k<n; ++k){
				phi(j) += c[k] * phi_func(j, delta[k]);
			}
		}

		u = solver.solve((ratio<TYPE>(2, 1)*M - dt*K - dt*k*M)*u + dt*phi);

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
		for(auto j=0; j<dim; ++j){
			phi(j) = 0.;
		}
	}

	pclose(gp);

	delete[] delta;
	delete[] c;

	return 0;
}
