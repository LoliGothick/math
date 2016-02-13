#pragma once

#include <iostream>
#include "Linear_Algebra.hpp"
using namespace std;

namespace LA{

template <typename T>
void RK_solver(vector<T> &vec, vector<T> (*func)(vector<T> &)){
	int dim = vec.dim;
	vector<T> v(dim);
}

}
