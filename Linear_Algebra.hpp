#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

namespace LA{

/********************************************/
/*            vector class                  */
/********************************************/
template <typename T>
class vector{
public:
	//unique_ptr<T> vec;
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
	//cout << "memory free" << endl;
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

/********************************************/
/*            matrix class                  */
/********************************************/

template <typename T>
class matrix{
public:
	T *mat;
	int dim;
	
	matrix(int);
	~matrix();

	T& operator()(int &, int &);

	void operator=(const matrix<T> &);
	matrix<T> operator+(matrix<T> &);
	matrix<T> operator-(matrix<T> &);

	void show();
};

template <typename T>
T& matrix<T>::operator()(int &m, int &n){
	return mat[n*dim + m];
}

template <typename T>
void matrix<T>::operator=(const matrix<T> &obj){
	int dim = this->dim;
	for(int i=0; i<dim; ++i){
		for(int j=0; j<dim; ++j){
			this->mat[j*dim + i] = obj.mat[j*dim + i];
		}
	}
}

template <typename T>
matrix<T> matrix<T>::operator+(matrix<T> &obj){
	matrix<T> ans(this->dim);
	for(int i=0; i<dim; ++i){
		for(int j=0; j<dim; ++j){
			ans(i,j) = this->operator()(i, j) + obj.operator()(i,j);
		}
	}
	return ans;
}

template <typename T>
matrix<T> matrix<T>::operator-(matrix<T> &obj){
	matrix<T> ans(this->dim);
	for(int i=0; i<dim; ++i){
		for(int j=0; j<dim; ++j){
			ans(i,j) = this->operator()(i, j) - obj.operator()(i,j);
		}
	}
	return ans;
}

template <typename T>
void matrix<T>::show(){
	cout << fixed << setprecision(4);
	for(int i=0; i<dim; ++i){
		for(int j=0; j<dim; ++j){
			cout << this->operator()(i, j) << " ";
		}
		cout << endl;
	}
}

template <typename T>
matrix<T>::matrix(int n){
	mat = new T[n * n];
	dim = n;

	for(int i=0; i<(dim*dim); ++i){
		mat[i] = 0;
	}
}

template <typename T>
matrix<T>::~matrix(){
	delete[] mat;
}

/*     multiplication       */

template <typename T>
matrix<T> operator*(matrix<T> &mat1, matrix<T> &mat2){
	matrix<T> ans(mat1.dim);
	int dim = mat1.dim;
	for(int i=0; i<dim; ++i){
		for(int j=0; j<dim; ++j){
			for(int k=0; k<dim; ++k){
				ans(i,j) += mat1(i,k) * mat2(k,j);
			}
		}
	}
	return ans;
}

template <typename T>
matrix<T> operator*(const T &k, matrix<T> &mat){
	matrix<T> ans(mat.dim);
	int dim = mat.dim;
	for(int i=0; i<dim; ++i){
		for(int j=0; j<dim; ++j){
			ans.operator()(i,j) = k * mat.operator()(i,j);
		}
	}
	return ans;
}

template <typename T>
vector<T> operator*(matrix<T> &mat, vector<T> &vec){
	vector<T> ans(vec.dim);
	int dim = vec.dim;
	for(int i=0; i<dim; ++i){
		for(int j=0; j<dim; ++j){
			ans(i) += mat(i,j) * vec(j);
		}
	}
	return ans;
}

}
