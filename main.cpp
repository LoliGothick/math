#include <iostream>
#include "Linear_Algebra.hpp"
using namespace std;

const double dim = 10;

int main(){

	LA::vector<double> a(dim);
	LA::vector<double> b(dim);
	LA::vector<double> c(dim);

	//a.show();
	
	a(4) = 5.5;
	b(4) = 0.5;

	c = a + b;

	c.show();

	return 0;
}
