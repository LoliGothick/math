#pragma once

#include <iostream>
#include "Linear_Algebra.hpp"
using namespace std;

namespace LA{

template <typename T>
void RK_solver(vector<T> &vec, vector<T> (*func)(const vector<T> &), const T &dt){

	size_t dim = vec.dim;

	vector<T> k1(dim);
	vector<T> k2(dim);
	vector<T> k3(dim);
	vector<T> k4(dim);

	k1 = func(vec);
	k2 = func(vec + 0.5*dt*k1);
	k3 = func(vec + 0.5*dt*k2);
	k4 = func(vec +     dt*k3);

	vec = vec + (1.0/6.0) *dt * (k1 + k2 + k3 + k4);
}

}
