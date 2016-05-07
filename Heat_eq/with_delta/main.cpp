/*
 *  soleve Heat Equation
 *	periodic with delta function
 *  x in [0,1]
 */

#include <iostream>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "fixed_is_floating_point.hpp"
#include "../../library/Linear_Algebra.hpp"
#include "../../library/LU_solver.hpp"
#include "../../library/ODE_solver.hpp"
#include "../../library/ratio.hpp"

using namespace std;

namespace mp = boost::multiprecision;
using namespace mp;
using f100 =  mp::cpp_dec_float_100;

using TYPE = double;
//#define TYPE f100

static constexpr int INTV = 4;

static constexpr int dim = 2048;
static constexpr TYPE dx = math::ratio<TYPE>(1, dim-1);
//const TYPE dt = math::ratio<TYPE>(1, 100000);
static constexpr TYPE dt = math::ratio<TYPE>(1,6)*dx*dx;
//const TYPE PI = acos(-1.0);       // NG
//const TYPE PI = acos(static_cast<TYPE>(-1.0));   // OK
static constexpr TYPE k  = math::ratio<TYPE>(1,1);

template <typename T>
T delta_func(int i,T x){
	static_assert(detail::fixed_is_floating_point_v<T>,"T is not floating point!");

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
	static_assert(detail::fixed_is_floating_point_v<T>,"T is not floating point!");
	
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
		//M(0, 0) = 0.5*math::ratio<T>(2, 3) * dx;
		//K(0, 0) = math::ratio<T>(2, 1) / dx;
		//M(dim-1, dim-1) = 0.5*math::ratio<T>(2, 3) * dx;
		//K(dim-1, dim-1) = math::ratio<T>(2, 1) / dx;
		flag++;
	}
	
	static LA::matrix_LU<T> LU(M);

	b = K * b;

	LU.solve_linear_eq(b, x);

	//static T delta_x1 = math::ratio<T>(4, 10);
	//static T delta_x2 = math::ratio<T>(5, 10);
	static T delta_x3 = math::ratio<T>(5, 10);

	if(delta_x3 >= 1){
		delta_x3 = delta_x3 - 1.;
	}else if(delta_x3 < 0){
		delta_x3 = delta_x3 + 1.;
	}
	//delta_x3 += 0.0001;

	int left  = (static_cast<int>(delta_x3/dx));
	int right = (left+1)%dim;

	//cout << dim << " " << left << " " << right << endl; 

/*
	cout << dx*((left+dim)%dim)   << " " << u.vec[(left+dim)%dim]   << endl;
	cout << dx*((left+dim-1)%dim) << " " << u.vec[(left+dim-1)%dim] << endl;
	cout << dx*((left+dim-2)%dim) << " " << u.vec[(left+dim-2)%dim] << endl;
*/
	/* make matrix */

	Eigen::Matrix<T, 3, 3> A;
	
	T xx = dx*((left+dim)%dim);

	A(0, 0) = xx*xx;
	A(0, 1) = xx;
	A(0, 2) = 1.;
	
	//T yy = dx*((left+dim-1)%dim);
	T yy = xx - dx;

	A(1, 0) = yy*yy;
	A(1, 1) = yy;
	A(1, 2) = 1.;
	
	//T zz = dx*((left+dim-2)%dim);
	T zz = yy - dx;

	A(2, 0) = zz*zz;
	A(2, 1) = zz;
	A(2, 2) = 1.;

//	cout << A << endl;

	/* make vector */
	
	Eigen::Matrix<T, 3, 1> bb;

	bb(0, 0) = u.vec[(left+dim)%dim];
	bb(1, 0) = u.vec[(left+dim-1)%dim];
	bb(2, 0) = u.vec[(left+dim-2)%dim];

//	cout << bb << endl;

//	cout << A.inverse()*bb << endl;
	Eigen::Matrix<T, 3, 1> co = A.inverse()*bb;

//	cout << co << endl;
	
	T aaa = co(0, 0);
	T bbb = co(1, 0);
	T ccc = co(2, 0);

	auto left_f = [&](T x){
		return aaa*x*x + bbb*x + ccc;
	};
/*	
	cout << dx*((right+dim)%dim)   << " " << u.vec[(right+dim)%dim]   << endl;
	cout << dx*((right+dim+1)%dim) << " " << u.vec[(right+dim+1)%dim] << endl;
	cout << dx*((right+dim+2)%dim) << " " << u.vec[(right+dim+2)%dim] << endl;
*/
	/* make matrix */

	Eigen::Matrix<T, 3, 3> B;
	
	T r_xx = dx*((right+dim)%dim);

	B(0, 0) = r_xx*r_xx;
	B(0, 1) = r_xx;
	B(0, 2) = 1.;
	
	//T r_yy = dx*((right+dim+1)%dim);
	T r_yy = r_xx + dx;

	B(1, 0) = r_yy*r_yy;
	B(1, 1) = r_yy;
	B(1, 2) = 1.;
	
	//T r_zz = dx*((right+dim+2)%dim);
	T r_zz = r_yy + dx;

	B(2, 0) = r_zz*r_zz;
	B(2, 1) = r_zz;
	B(2, 2) = 1.;

//	cout << B << endl;

	/* make vector */
	
	Eigen::Matrix<T, 3, 1> r_bb;

	r_bb(0, 0) = u.vec[(right+dim)%dim];
	r_bb(1, 0) = u.vec[(right+dim+1)%dim];
	r_bb(2, 0) = u.vec[(right+dim+2)%dim];

//	cout << r_bb << endl;

//	cout << B.inverse()*r_bb << endl;
	Eigen::Matrix<T, 3, 1> r_co = B.inverse()*r_bb;

//	cout << r_co << endl;
	
	T r_aaa = r_co(0, 0);
	T r_bbb = r_co(1, 0);
	T r_ccc = r_co(2, 0);
	//cout << co << endl;
	//cout << r_co << endl;
	
	auto right_f = [&r_aaa,&r_bbb,&r_ccc](T x){
		return r_aaa*x*x + r_bbb*x + r_ccc;
	};

	//cout << left_f(delta_x3) << endl;
	//cout << right_f(delta_x3) << endl;

	T diff = (right_f(delta_x3+dx/2.) - left_f(delta_x3-dx/2.))/(dx);

	//cout << diff << endl;

	T coef = math::ratio<T>(10,1);

	delta_x3 = delta_x3 - coef * diff * dt;
	
	if(delta_x3 >= 1){
		delta_x3 = delta_x3 - 1.;
	}else if(delta_x3 < 0){
		delta_x3 = delta_x3 + 1.;
	}

//	cout << "delta_x3 :" << delta_x3 << "\n";

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
	static_assert(detail::fixed_is_floating_point_v<T>,"T is not floating point!");

	T a = 0.4;
	for(auto i=0; i<u.dim; i++){
		//u(i) = sin(PI*i*dx);
		u(i) = 0.2*exp(-100*(dx*i-a)*(dx*i-a));
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
	
	
	for(auto i=0; t<10.8; i++){
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
