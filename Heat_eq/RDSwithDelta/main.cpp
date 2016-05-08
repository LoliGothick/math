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

static constexpr int INTV = 50;
static constexpr int dim  = 2048;
static constexpr int n    = 2;

template <typename T>
constexpr T ratio(const T &a, const T &b){
	return static_cast<T>(a/b);
}

const TYPE dx = ratio<TYPE>(1, dim);
const TYPE dt = ratio<TYPE>(1, 100*dim);
const TYPE k  = ratio<TYPE>(1, 1);
const TYPE v  = ratio<TYPE>(100, 1);
//const TYPE PI = acos(static_cast<TYPE>(-1.0));

template <typename T>
T phi_func(int i,T x){
	if((i-1)*dx < x && x < (i+1)*dx){
		return 1. - abs(x - i*dx)/dx;
	}else if((i == 0) && (1.-dx) < x){
		x = x - 1.;
		return 1. - abs(x)/dx;
	}else{
		return 0;
	}
	cout << "ERROR 32522" << endl;
	return -1;
}

template <typename T>
void init(Eigen::Matrix<T, dim, 1> &u){
	T a = ratio<T>(5, 10);
	T b = ratio<T>(5, 10);
	T c = ratio<T>(1, 10);
	for(auto i=0; i<dim; i++){
		//u(i) = sin(PI*i*dx);
		u(i) = c*ratio<T>(1, 1)*exp(-100.*(dx*i-a)*(dx*i-a)) + c*ratio<T>(1, 1)*exp(-100.*(dx*i-b)*(dx*i-b));
	}
}

template <typename T>
T polynomial(Eigen::Matrix<T, 3, 1> co, T x){
	return co(0)*x*x + co(1)*x + co(2);
}

template <typename T>
void CalcDiff(const Eigen::Matrix<T, dim, 1>& u, T* delta, T* diff){

	for(auto j=0; j<n; ++j){

		int pos = delta[j]/dx;

		if(pos == dim){
			cout << "ERROR 12374892" << endl;
			pos = 0;
		}
		if(pos > dim){
			cout << "ERROR 12374912" << endl;
			pos = 0;
		}

		/* left */
		Eigen::Matrix<T, 3, 3> A;
	
		T x = static_cast<T>(pos * dx);
		for(auto i=0; i<3; ++i){
			A(i, 0) = x*x;
			A(i, 1) = x;
			A(i, 2) = 1.;
			x -= dx;
		}
	
		Eigen::Matrix<T, 3, 1> b;

		b(0) = u(pos);
		pos  = ((pos-1)+dim)%dim;
		b(1) = u((pos+dim)%dim);
		pos  = ((pos-1)+dim)%dim;
		b(2) = u((pos+dim)%dim);
	
		Eigen::Matrix<T, 3, 1> co_left = A.inverse()*b;
	/*
		auto left_f = [&](T x){
			return co_left(0)*x*x + co_left(1)*x + co_left(2);
		};
	*/
		/* right */
	
		Eigen::Matrix<T, 3, 3> B;
	
		pos = delta[j]/dx;
		pos++;
		x = static_cast<T>(pos * dx);
		for(auto i=0; i<3; ++i){
			B(i, 0) = x*x;
			B(i, 1) = x;
			B(i, 2) = 1.;
			x += dx;
		}
	
		pos = pos%dim;
		if(pos == dim){
			pos = 0;
			cout << "wwwwwwwwwwwwwww" << endl;
		}


		Eigen::Matrix<T, 3, 1> r_b;
	
		r_b(0) = u(pos);
		pos    = (pos+1)%dim;
		r_b(1) = u(pos%dim);
		pos    = (pos+1)%dim;
		r_b(2) = u(pos%dim);


		Eigen::Matrix<T, 3, 1> co_right = B.inverse()*r_b;
	/*
		auto right_f = [&](T x){
			return co_right(0)*x*x + co_right(1)*x + co_right(2);
		};
	*/
		//cout << A << endl;
		//cout << B << endl;

		/* symmetric derivative */
		diff[j] = (polynomial(co_right, delta[j] + dx/2.) - polynomial(co_left, delta[j] - dx/2.))/dx;
		//diff[j] = (right_f(delta[j] + dx/2.) - left_f(delta[j] - dx/2.))/dx;
		//diff[0] = (right_f(delta[0] + dx) - left_f(delta[0] - dx))/(2.*dx);
	/*	
		A    = Eigen::Matrix<T, 3, 3>::Zero();
		B    = Eigen::Matrix<T, 3, 3>::Zero();
		b    = Eigen::Matrix<T, 3, 1>::Zero();
		r_b  = Eigen::Matrix<T, 3, 1>::Zero();
		co_left   = Eigen::Matrix<TYPE, 3, 1>::Zero();
		co_right  = Eigen::Matrix<TYPE, 3, 1>::Zero();
	*/
	}
	//cout << __LINE__ << endl;
}

int main(){

	cout << fixed << setprecision(numeric_limits<TYPE>::digits10 + 1);
	
	auto *delta      = new TYPE[n];
	auto *temp_delta = new TYPE[n];
	auto *c          = new TYPE[n];
	auto *diff       = new TYPE[n];

    std::random_device rnd;     // 非決定的な乱数生成器を生成
	std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_real_distribution<> rand100(0., 1.);        // [0, 1] 範囲の一様乱数
	for(int i = 0; i < n; ++i) {
		delta[i] = 0.99;//rand100(mt);
	//	delta[i] = 0.99999;//rand100(mt);
		c[i]     = 1.;//rand100(mt);
	}
	delta[0] = 0.48;//rand100(mt);
	delta[1] = 0.9996;//rand100(mt);
	delta[1] = 0.9999;//rand100(mt);
	
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

	for(auto i=0;  t<10000000; i++){
		t = static_cast<TYPE>(i*dt);

		for(auto j=0; j<dim; ++j){
			for(auto k=0; k<n; ++k){
				phi(j) += c[k] * phi_func(j, delta[k]);
			}
		}

		temp = solver.solve((ratio<TYPE>(2, 1)*M - dt*K - dt*k*M)*u + dt*phi);

		CalcDiff(u, delta, diff);
		
		for(auto j=0; j<n; ++j){
			
			delta[j] -= v * dt * diff[j];
			
			if(delta[j] < 0.){
				delta[j] += 1.;
			}
			if(delta[j] > 1.){
				delta[j] -= 1.;
			}
		}
		
		
		for(auto j=0; j<dim; ++j){
			phi(j) = 0.;
		}

		/* renew */
		u = temp;

		/* draw */
		if(i%INTV == 0){
			
			cout << t << " ";
			for(auto j=0; j<n; ++j){
				cout << delta[j] << " ";
			}
			cout << endl;
			
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

	delete[] delta;
	delete[] temp_delta;
	delete[] diff;
	delete[] c;

	return 0;
}
