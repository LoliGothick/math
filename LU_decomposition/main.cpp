#include <iostream>
#include "../library/Linear_Algebra.hpp"

using namespace std;

const int dim = 12;

int main(){

	LA::matrix<double> A(dim);
	LA::matrix<double> L(dim);
	LA::matrix<double> U(dim);

	for(int i=0; i<dim; ++i){
		for(int j=0; j<dim; ++j){
			//A(i,j) = 1./(i+j+1);
			if(i==j) A(i,j) = 2.;
			if((i==(j-1) || (i==(j+1)))) A(i,j) = 1.;
		}
	}

	A.show();

	/* LU decomposition*/

	double sum = 0.;

	for(int i=0; i<dim; i++){
		U(i,i) = 1.;
	}

	for(int j=0; j<dim; j++){
		for(int i=j; i<dim; i++){
			sum = 0.;
			for(int k=0; k<j; k++) {
				sum = sum + L(i,k) * U(k,j);	
			}
			L(i,j) = A(i,j) - sum;
		}

		for(int i=j; i<dim; i++){
			sum = 0.;
			for(int k=0; k<j; k++) {
				sum = sum + L(j,k) * U(k,i);
			}
			if(L(j,j) == 0.) {
				cout << "det(L) close to 0!\n Can't divide by 0..." << endl;
				exit(EXIT_FAILURE);
			}
			U(j,i) = (A(j,i) - sum) / L(j,j);
		}
	}

	L.show();
	U.show();

	(L*U).show();


	LA::vector<double> x(dim);
	LA::vector<double> y(dim);
	LA::vector<double> b(dim);

	for(int i=0; i<x.dim; ++i){
		x(i) = 1.;
	}

	b = A * x;

	b.show();
	
	for(int i=0; i<x.dim; ++i){
		x(i) = 0.;
	}

	for(int i=0; i<b.dim; ++i){
		y(i) = b(i);
		for(int j=0; j<i; ++j){
			y(i) -= L(i,j) * y(j);
		}
		y(i) = y(i) / L(i,i);
	}

	for(int i=b.dim; i>=0; --i){
		x(i) = y(i);
		for(int j=x.dim; j>i; --j){
			x(i) -= U(i,j) * x(j);
		}
	}

	x.show();
	
	return 0;
}
