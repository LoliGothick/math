#pragma once

#include <iostream>
#include "Linear_Algebra.hpp"
#include "ratio.hpp"
using namespace std;

namespace LA{

template <typename T>
void RK4(vector<T> &vec, vector<T> (*func)(vector<T> const &), const T &dt){

	auto dim = vec.dim;

	static T r1_2 = math::ratio<T>(1, 2);
	static T r1_6 = math::ratio<T>(1, 6);
	static T r2_1 = math::ratio<T>(2, 1);

	vector<T> k1(dim);
	vector<T> k2(dim);
	vector<T> k3(dim);
	vector<T> k4(dim);

	k1 = func(vec                   );
	k2 = func(vec + r1_2 * (dt * k1));
	k3 = func(vec + r1_2 * (dt * k2));
	k4 = func(vec +          dt * k3);

	vec = vec + r1_6 * (dt * (k1 + r2_1 * k2 + r2_1 * k3 + k4));
}

/*
template <typename T>
void RK4(vector<T> &vec, vector<T> (*func)(vector<T> &), const T &dt){

	size_t dim = vec.dim;

	static T r1_2 = math::ratio<T>(1, 2);
	static T r1_6 = math::ratio<T>(1, 6);
	static T r2_1 = math::ratio<T>(2, 1);

	vector<T> k1(dim);
	vector<T> k2(dim);
	vector<T> k3(dim);
	vector<T> k4(dim);

	k1 = func(vec                   );
	k2 = func(vec + r1_2 * (dt * k1));
	k3 = func(vec + r1_2 * (dt * k2));
	k4 = func(vec +          dt * k3);

	vec = vec + r1_6 * (dt * (k1 + r2_1 * k2 + r2_1 * k3 + k4));
}
*/
}
