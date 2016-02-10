#pragma once

#include <iostream>
#include "Linear_Algebra.hpp"

using namespace std;

namespace LA{

template <typename T>
class matrix_LU{
public:
	matrix<T> L;
	matrix<T> U;

	int dim;

	//matrix_LU(int);
	//matrix_LU(){};
	matrix_LU(matrix<T> &);
	~matrix_LU();

	void LU_decomposition(matrix<T> &);

	void solve_linear_eq(vector<T> &, vector<T> &);
};

template <typename T>
void matrix_LU<T>::solve_linear_eq(vector<T> &b, vector<T> &x){

	LA::vector<T> y(dim);
	
	for(int i=0; i<b.dim; ++i){
		y(i) = b(i);
		for(int j=0; j<i; ++j){
			y(i) -= L(i,j) * y(j);
		}
		y(i) = y(i) / L(i,i);
	}

	for(int i=(b.dim-1); i>=0; --i){
		x(i) = y(i);
		for(int j=(x.dim-1); j>i; --j){
			x(i) -= U(i,j) * x(j);
		}
	}
}

template <typename T>
void matrix_LU<T>::LU_decomposition(matrix<T> &A){
	
	// LU decomposition

	T sum = 0.;

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
}

/*
template <typename T>
matrix_LU<T>::matrix_LU(int N){
	dim = N;
	L = new matrix<T>(dim);
	U = new matrix<T>(dim);
}
*/
template <typename T>
matrix_LU<T>::matrix_LU(matrix<T> &A){
	dim = A.dim;
	L.setter(dim);
	U.setter(dim);

	/* LU decomposition*/

	T sum = 0.;

	for(int i=0; i<dim; ++i){
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
}

template <typename T>
matrix_LU<T>::~matrix_LU(){
}

}
