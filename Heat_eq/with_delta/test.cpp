#include <iostream>

using namespace std;

int main(){

	constexpr int N = 10;
	int a[N];

	for(auto i=0; i<N; ++i){
		a[i] = i;
	}

	cout << a[(-11+N)%N] << endl;

	return 0;
}
