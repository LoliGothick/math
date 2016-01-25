#include <iostream>
#include <cmath>
using namespace std;

namespace LA{

template <typename T>
class vector{
public:
	T *vec;
	int dim;

	vector(const int);
	~vector();
	
	T& operator()(const int);
	
	vector<T> operator+(const vector<T>&);
	vector<T> operator-(const vector<T>&);

	void operator=(const vector<T>&);
	void operator+=(const vector<T>&);
	void operator-=(const vector<T>&);

	T operator*(const vector<T>&);
	T norm();

	void show();
};

template <typename T>
vector<T> vector<T>::operator+(const vector<T> &obj){
	/*
	if(dim != obj.dim){
		cout << "error dim doesn't not match."
	}
	*/
	vector<T> ans(dim);
	for(int i=0; i<dim; ++i){
		ans.vec[i] = vec[i] + obj.vec[i];
	}
	return ans;
}

template <typename T>
vector<T> vector<T>::operator-(const vector<T> &obj){
	vector<T> ans(dim);
	for(int i=0; i<dim; ++i){
		ans.vec[i] = vec[i] - obj.vec[i];
	}
	return ans;
}

template <typename T>
void vector<T>::operator=(const vector<T> &obj){
	for(int i=0; i<dim; ++i){
		this->vec[i] = obj.vec[i];
	}
}

template <typename T>
void vector<T>::operator+=(const vector<T> &obj){
	for(int i=0; i<dim; ++i){
		this->vec[i] = this->vec[i] + obj.vec[i];
	}
}

template <typename T>
void vector<T>::operator-=(const vector<T> &obj){
	for(int i=0; i<dim; ++i){
		this->vec[i] = this->vec[i] - obj.vec[i];
	}
}

template <typename T>
vector<T>::vector(const int N){
	vec = new T[N];
	for(int i=0; i<N; ++i){
		vec[i] = 0.;
	}
	dim = N;
}

template <typename T>
vector<T>::~vector(){
	delete[] vec;
}

template <typename T>
T& vector<T>::operator()(const int i){
	return vec[i];
}

template <typename T>
void vector<T>::show(){
	for(int i=0; i<dim; ++i){
		cout << vec[i] << endl;
	}
	cout << endl;
}

template <typename T>
T vector<T>::operator*(const vector<T> &obj){
	T ans = 0;
	for(int i=0; i<dim; ++i){
		ans += this->vec[i] * obj.vec[i];
	}
	return ans;
}

template <typename T>
T vector<T>::norm(){
	T ans=0;
	for(int i=0; i<dim; ++i){
		ans += vec[i] * vec[i];
	}
	return pow(ans, 0.5);
}

/* scalar multiplication */

template <typename T>
vector<T> operator*(const T &k, const vector<T> &obj){
	vector<T> ans(obj.dim);
	for(int i=0; i<obj.dim; ++i){
		ans.vec[i] = k * obj.vec[i];
	}
	return ans;
}

}
