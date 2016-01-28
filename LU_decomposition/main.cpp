#include <iostream>
#include "../library/Linear_Algebra.hpp"

using namespace std;

const int dim = 3;

int main(){

	LA::matrix<double> mat(dim);
	LA::matrix<double> L(dim);
	LA::matrix<double> U(dim);

	for(int i=0; i<dim; ++i){
		U(i,i) = 1.;
	}


	return 0;
}
