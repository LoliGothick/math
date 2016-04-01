#include <iostream>
#include <cmath>
using namespace std;

int main(){

	double x = 20.543;
	double sum = 0.;

	for(size_t i=0; i<100000000; ++i){
		sum += pow(x+i+0.123, 0.1);
	}

	cout << sum << endl;

	return 0;
}
