#include <iostream>
#include <memory>
#include <cstdlib>
using namespace std;

namespace LA{

template <typename T>
class vector{
public:
	//shared_ptr<T> *v;
	T *v;
	int dim;

	vector(const int);
	~vector();
	T& operator()(const int);
	void show();
};

template <typename T>
vector<T>::vector(const int N){
	v = new T[N];
	for(int i=0; i<N; ++i){
		v[i] = 0.;
	}
	dim = N;
}

template <typename T>
vector<T>::~vector(){
	delete[] v;
}

template <typename T>
T& vector<T>::operator()(const int i){
	return v[i];
}

template <typename T>
void vector<T>::show(){
	for(int i=0; i<dim; ++i){
		cout << v[i] << endl;
	}
}

}
