/*
 *  Heat Equation solover
 *
 *  x in [0,1]
 */


#include <iostream>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include "../library/Linear_Algebra.hpp"
#include "../library/ODE_solver.hpp"
#include "../library/ratio.hpp"

using namespace std;

namespace mp = boost::multiprecision;
using namespace mp;
typedef mp::cpp_dec_float_100 f100;

#define TYPE f100

const int dim = 10;
const TYPE dx = math::ratio<TYPE>(1, (dim - 1));
const TYPE dt = math::ratio<TYPE>(1, 1000);
const TYPE PI = acos(-1.0);

template <typename T>
LA::vector<T> func(const LA::vector<T> &u){
	static int flag = 0;
	int dim = u.dim;

	LA::vector<T> ans(dim);
	
	static LA::matrix<TYPE> M(dim);
	static LA::matrix<TYPE> K(dim);
	
	if(flag == 0){
		for(int i=0; i<dim; i++){
			for(int j=0; j<dim; j++){
				if(i == j){
					M(i,j) = math::ratio<T>(2, 3) * dx;
					K(i,j) = math::ratio<T>(1, 6) * dx;
				}
				if(i == (j-1) || i == (j+1)){
					M(i,j) = math::ratio<T>( 2, 1) / dx;
					K(i,j) = math::ratio<T>(-1, 1) / dx;
				}
			}
		}
		flag++;
	}

	//iroiro keisann suru

	ans = u;
	return ans;
}

template <typename T>
void init(LA::vector<T> &u){
	for(int i=0; i<u.dim; i++){
		u(i) = sin(PI*i*dx);
	}
}

int main(){

	//cout << fixed << setprecision(numeric_limits<TYPE>::digits10 + 1);

	LA::vector<TYPE> u(dim);

	init<TYPE>(u);
	u.show();

	cout << PI << endl;
	cout << sin(PI) << endl;

	LA::RK4(u, func, dt);

	return 0;
}
