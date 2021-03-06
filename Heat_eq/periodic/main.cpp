/*
 *  soleve Heat Equation
 *	periodic
 *  x in [0,1]
 */

#include <iostream>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include "../../library/Linear_Algebra.hpp"
#include "../../library/LU_solver.hpp"
#include "../../library/ODE_solver.hpp"
#include "../../library/ratio.hpp"

using namespace std;

namespace mp = boost::multiprecision;
using namespace mp;
typedef mp::cpp_dec_float_100 f100;

//#define TYPE double
#define TYPE double

static constexpr int INTV = 1;

const int dim = 100;
const TYPE dx = math::ratio<TYPE>(1, dim - 1);
//const TYPE dt = math::ratio<TYPE>(1, 100000);
const TYPE dt = math::ratio<TYPE>(1,6)*dx*dx;
//const TYPE PI = acos(-1.0);       // NG
const TYPE PI = acos(static_cast<TYPE>(-1.0));   // OK
const TYPE k  = math::ratio<TYPE>(20,1);

template <typename T>
LA::vector<T> func(const LA::vector<T> &u){
	static int flag = 0;
	int dim = u.dim;

	LA::vector<T> b(dim);
	LA::vector<T> x(dim);
	b = u;

	static LA::matrix<TYPE> M(dim);
	static LA::matrix<TYPE> K(dim);
	
	if(flag == 0){
		for(int i=0; i<dim; i++){
			for(int j=0; j<dim; j++){
				if(i == j){
					M(i,j) = math::ratio<T>(2, 3) * dx;
					K(i,j) = math::ratio<T>(2, 1) / dx;
				}
				if(i == (j-1) || i == (j+1)){
					M(i,j) = math::ratio<T>( 1, 6) * dx;
					K(i,j) = math::ratio<T>(-1, 1) / dx;
				}
				if((i == 0 && j == dim-1) || (i == dim-1 && j == 0)){
					M(i,j) = math::ratio<T>(1, 6) * dx;
					K(i,j) = math::ratio<T>(-1, 1) / dx;
				}
			}
		}
		flag++;
	}
	
	static LA::matrix_LU<T> LU(M);

	b = K * b;

	LU.solve_linear_eq(b, x);
	
	//reaction -ku
	x = math::ratio<T>(-1, 1) * x - k * u;

	return x;
}

template <typename T>
void init(LA::vector<T> &u){
	T a = 0.2;
	for(int i=0; i<u.dim; i++){
		//u(i) = sin(PI*i*dx);
		u(i) = exp(-100*(dx*i-a)*(dx*i-a));
	}
}

int main(){

	cout << fixed << setprecision(numeric_limits<TYPE>::digits10 + 1);

	LA::vector<TYPE> u(dim);
	TYPE t = math::ratio<TYPE>(0, 1);
	TYPE x = math::ratio<TYPE>(0, 1);

	init<TYPE>(u);
	
	FILE *gp;
	gp = popen("gnuplot -persist", "w");
	fprintf(gp, "set xr [0:1]\n");
	fprintf(gp, "set yr [0:1]\n");
	//fprintf(gp, "set size square\n");
	fprintf(gp, "set grid\n");
	
	
	for(int i=0; t<0.1; i++){
	//for(int i=0; i<2; i++){
		t = i*dt;	
		
		if(i%INTV == 0){
			x = 0.;
			fprintf(gp, "plot '-' w l lw 3\n");
			for(int j=0; j<dim; j++){
				double a = static_cast<double>(x);
				double b = static_cast<double>(u(j));
				//cout << x << " " << u(j) << endl;
				fprintf(gp, "%f %f \n", a, b);
				//fprintf(gp, "\n");
				x += dx;
			}
			fprintf(gp, "e\n");
			fflush(gp);
		}
		//cout << endl;
		
		LA::RK4(u, func, dt);
		/*	
		TYPE sum = static_cast<TYPE>(0.);
		for(int i=0; i<dim; ++i){
			sum += u(i)*dx;
		}
		cout << sum << endl;
		*/
	//	u.show();
	}

	pclose(gp);
	
	return 0;
}
