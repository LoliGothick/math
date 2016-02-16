/*
 *  Heat Equation solover
 *
 *  x in [0,1]
 */


#include <iostream>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include "../library/Linear_Algebra.hpp"
#include "../library/LU_solver.hpp"
#include "../library/ODE_solver.hpp"
#include "../library/ratio.hpp"

using namespace std;

namespace mp = boost::multiprecision;
using namespace mp;
typedef mp::cpp_dec_float_100 f100;

#define TYPE f100

const int INTV = 100;

const int dim = 100;
const TYPE dx = math::ratio<TYPE>(1, dim + 1);
const TYPE dt = math::ratio<TYPE>(1, 100000);
//const TYPE PI = acos(-1.0);       // NG
const TYPE PI = acos((TYPE)-1.0);   // OK

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
			}
		}
		flag++;
	}

	LA::matrix_LU<T> LU(M);

	b = K * b;

	LU.solve_linear_eq(b, x);

	x = math::ratio<T>(-1, 1) * x;

	return x;
}

template <typename T>
void init(LA::vector<T> &u){
	for(int i=0; i<u.dim; i++){
		u(i) = sin(PI*(i+1)*dx);
	}
}

int main(){

	//cout << fixed << setprecision(numeric_limits<TYPE>::digits10 + 1);

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
	//for(int i=0; i<10; i++){
		t = i*dt;
		
		
		if(i%INTV == 0){
			fprintf(gp, "plot '-' w l\n");
			fprintf(gp, "0 0\n");
			x = dx;
			for(int j=0; j<dim; j++){
				double a = static_cast<double>(x);
				double b = static_cast<double>(u(j));
				//cout << x << " " << u(j) << endl;
				fprintf(gp, "%f %f \n", a, b);
				//fprintf(gp, "\n");
				x += dx;
			}
			fprintf(gp, "1 0\n");
			fprintf(gp, "e\n");
			fflush(gp);
		}
		//cout << endl;
		
		
		LA::RK4(u, func, dt);
	//	u.show();
	}

	pclose(gp);
	
	return 0;
}
