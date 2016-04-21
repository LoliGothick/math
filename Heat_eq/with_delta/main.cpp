/*
 *  soleve Heat Equation
 *	periodic with delta function
 *  x in [0,1]
 */

#include <iostream>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "../../library/Linear_Algebra.hpp"
#include "../../library/LU_solver.hpp"
#include "../../library/ODE_solver.hpp"
#include "../../library/ratio.hpp"

using namespace std;

namespace mp = boost::multiprecision;
using namespace mp;
typedef mp::cpp_dec_float_100 f100;

#define TYPE double
//#define TYPE f100

static constexpr int INTV = 3;

const int dim = 157;
const TYPE dx = math::ratio<TYPE>(1, dim - 1);
//const TYPE dt = math::ratio<TYPE>(1, 100000);
const TYPE dt = math::ratio<TYPE>(1,6)*dx*dx;
//const TYPE PI = acos(-1.0);       // NG
const TYPE PI = acos(static_cast<TYPE>(-1.0));   // OK
const TYPE k  = math::ratio<TYPE>(10,1);

template <typename T>
T delta_func(int i,T x){
	if((i-1)*dx < x && x < (i+1)*dx){
		return 1. - abs(x - i*dx)/dx;
	}else{
		return 0;
	}
	cout << "ERROR 32522" << endl;
	return -1;
}

template <typename T>
LA::vector<T> func(LA::vector<T> const &u){
	static int flag = 0;
	int dim = u.dim;

	LA::vector<T> b(dim);
	LA::vector<T> x(dim);
	LA::vector<T> y(dim);
	LA::vector<T> delta1(dim);
	LA::vector<T> delta2(dim);
	LA::vector<T> delta3(dim);
	LA::vector<T> delta(dim);
	b = u;

	static LA::matrix<TYPE> M(dim);
	static LA::matrix<TYPE> K(dim);
	
	if(flag == 0){
		for(auto i=0; i<dim; i++){
			for(auto j=0; j<dim; j++){
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

	//static T delta_x1 = math::ratio<T>(4, 10);
	//static T delta_x2 = math::ratio<T>(5, 10);
	static T delta_x3 = math::ratio<T>(6, 10);

	if(delta_x3 > 1){
		delta_x3 = delta_x3 - 1;
	}else if(delta_x3 < 0){
		delta_x3 = delta_x3 + 1;
	}
	//delta_x3 += 0.0001;

	int left = static_cast<int>(delta_x3/dx);

	cout << u.vec[left] << endl;
	//cout << u(3) << endl;

//	T diff = (u.vec[left+2] - u.vec[left-1])/(4*dx);


	//cout << delta_x3 << endl;

	for(auto i=0; i<dim; ++i){
		delta1(i) = 0;//math::ratio(1, 1) * delta_func<T>(i, delta_x1);
		delta2(i) = 0;//math::ratio(1, 1) * delta_func<T>(i, delta_x2);
		delta3(i) = math::ratio(1, 1) * delta_func<T>(i, delta_x3);
	}

	delta = delta1 + delta2 + delta3;
	
	LU.solve_linear_eq(delta, y);
/*
	delta_x3 += - dx * diff;

	if(delta_x3 > 1.){
		delta_x3 -= 1.;
	}else if(delta_x3<0){
		delta_x3 += 1.;
	}
*/
	//reaction -ku and delta function
	x = math::ratio<T>(-1, 1) * x - k * u + y;

	return x;
}

template <typename T>
void init(LA::vector<T> &u){
	T a = 0.8;
	for(auto i=0; i<u.dim; i++){
		//u(i) = sin(PI*i*dx);
		u(i) = exp(-100*(dx*i-a)*(dx*i-a));
		//u(i) = 0.;
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
	//fprintf(gp, "set yr [0:1]\n");
	//fprintf(gp, "set size square\n");
	fprintf(gp, "set grid\n");
	
	
	for(auto i=0; t<0.001; i++){
	//for(int i=0; i<2; i++){
		t = i*dt;	
		
		if(i%INTV == 0){
			x = 0.;
			fprintf(gp, "plot '-' w l lw 1\n");
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
