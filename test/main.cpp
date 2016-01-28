#include <iostream>
#include "Linear_Algebra.hpp"
using namespace std;

const double dim = 20;

int main(){

	LA::matrix<double> m(dim);
	LA::matrix<double> n(dim);
	LA::matrix<double> ans(dim);

	for(int i=0; i<dim; ++i){
		for(int j=0; j<dim; ++j){
			m(i,j) = 1.1;//1./(i + j + 1);
			n(i,j) = 0.9;
		}
	}

	(m + n).show();

	ans = (m * n);

	ans.show();


	ans = (m - m);
	ans.show();
	
	LA::vector<double> a(dim);
	LA::vector<double> b(dim);
	LA::vector<double> c(dim);

	for(int i=0; i<dim; ++i){
		a(i) = i+0.2;
		b(i) = 3.;
	}

	a.show();
	b.show();

	cout << b.norm() << endl;
	cout << a*b << endl;

	c = a + b;

	c.show();

	c = 3.*c;

	c.show();

	c = m * a;

	c.show();

	return 0;
}
