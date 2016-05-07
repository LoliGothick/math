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

static constexpr int INTV = 2;
static constexpr int dim  = 374;

template <typename T>
constexpr T ratio(const T &a, const T &b){
	return static_cast<T>(a/b);
}

const TYPE dx = ratio<TYPE>(1, dim);
const TYPE dt = ratio<TYPE>(1, 10*dim);
const TYPE k  = ratio<TYPE>(10, 1);
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
		u(i) = ratio<T>(1, 1)*exp(-10*(dx*i-a)*(dx*i-a));
	}
}

template <typename T>
void CalcDiff(const Eigen::Matrix<T, dim, 1>& u, T* delta, T* diff){
	size_t pos = delta[0]/dx;
	//cout << pos << endl;

	/* left */
	static Eigen::Matrix<T, 3, 3> A;
	
	T x = pos * dx;
	for(auto i=0; i<3; ++i){
		A(i, 0) = x*x;
		A(i, 1) = x;
		A(i, 2) = 1.;
		x -= dx;
	}
	
	Eigen::Matrix<T, 3, 1> b;

	b(0) = u(pos); pos--;
	b(1) = u(pos); pos--;
	b(2) = u(pos);
	
	Eigen::Matrix<T, 3, 1> co_left = A.inverse()*b;
	
	auto left_f = [&](T x){
		return co_left(0)*x*x + co_left(1)*x + co_left(2);
	};

	/* right */
	
	static Eigen::Matrix<T, 3, 3> B;
	
	pos = delta[0]/dx;
	pos++;
	x = pos * dx;
	for(auto i=0; i<3; ++i){
		B(i, 0) = x*x;
		B(i, 1) = x;
		B(i, 2) = 1.;
		x += dx;
	}
	
	Eigen::Matrix<T, 3, 1> r_b;
	
	r_b(0, 0) = u(pos); pos++;
	r_b(1, 0) = u(pos); pos++;
	r_b(2, 0) = u(pos);
	
	Eigen::Matrix<T, 3, 1> co_right = B.inverse()*r_b;
	
	auto right_f = [&](T x){
		return co_right(0)*x*x + co_right(1)*x + co_right(2);
	};

	/* symmetric derivative */
	diff[0] = (right_f(delta[0] + dx/2.) - left_f(delta[0] - dx/2.))/dx;
	//diff[0] = (right_f(delta[0] + dx) - left_f(delta[0] - dx))/(2.*dx);
}

int main(){

	cout << fixed << setprecision(numeric_limits<TYPE>::digits10 + 1);
	
	constexpr int n = 1;

	auto *delta      = new TYPE[n];
	auto *temp_delta = new TYPE[n];
	auto *c          = new TYPE[n];
	auto *diff       = new TYPE[n];

    std::random_device rnd;     // 非決定的な乱数生成器を生成
	std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_real_distribution<> rand100(0., 1.);        // [0, 1] 範囲の一様乱数
	for(int i = 0; i < n; ++i) {
		delta[i] = 0.5;//rand100(mt);
		c[i]     = 10.;//rand100(mt);
	}
	
	Eigen::Matrix<TYPE, dim, 1> phi  = Eigen::Matrix<TYPE, dim, 1>::Zero();

	Eigen::Matrix<TYPE, dim, 1> u    = Eigen::Matrix<TYPE, dim, 1>::Zero();
	Eigen::Matrix<TYPE, dim, 1> temp = Eigen::Matrix<TYPE, dim, 1>::Zero();

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

	TYPE t {};

	/* main time loop */

	for(auto i=0;  t<100; i++){
		t = static_cast<TYPE>(i*dt);

		for(auto j=0; j<dim; ++j){
			for(auto k=0; k<n; ++k){
				phi(j) += c[k] * phi_func(j, delta[k]);
			}
		}

		temp = solver.solve((ratio<TYPE>(2, 1)*M - dt*K - dt*k*M)*u + dt*phi);
		
		CalcDiff(u, delta, diff);
		
		//for(auto i=0; i<n; ++i){
			delta[0] -= dt * diff[0];
		//}

		if(i%INTV == 0){
			cout << diff[0] << " " << delta[0] << endl;
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
		/* renew */
		u = temp;
	}

	pclose(gp);

	delete[] delta;
	delete[] temp_delta;
	delete[] diff;
	delete[] c;

	return 0;
}
