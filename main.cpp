#include <iostream>
#include "Linear_Algebra.hpp"
using namespace std;


int main(){

	LA::vector<double> myvector(20);

	myvector(4) = 5.5;

	myvector.show();

	return 0;
}
