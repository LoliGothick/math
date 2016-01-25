#include <iostream>
#include "Linear_Algebra.hpp"
using namespace std;

const double dim = 10;

int main(){

	LA::vector<double> a(dim);
	LA::vector<double> b(dim);
	LA::vector<double> c(dim);

	for(int i=0; i<dim; ++i){
		a(i) = i+0.2;
		b(i) = 3.;
	}

	a.show();
	b.show();
	

	c = a + b;

	c.show();

	c = 3.*c;

	c.show();

	return 0;
}
