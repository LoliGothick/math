#include <iostream>

using namespace std;

template <typename T>
T cd(T n){
    T c = 0;
    for(size_t i=1; 2*i<=n; ++i){
        if(n%i == 0){
           c++;
        }
    }
    c++;
    return c;
}

template <typename T>
void same(T n){
    //if((cd<T>(n) == cd<T>(n+1)) && cd<T>(n) == 8){
    if((cd<T>(n)   == cd<T>(n+1)) && 
	   (cd<T>(n+1) == cd<T>(n+2)) &&
	   (cd<T>(n+2) == cd<T>(n+3)) &&
	   (cd<T>(n+3) == cd<T>(n+4)) &&
	   (cd<T>(n+4) == cd<T>(n+5)) &&
	   (cd<T>(n+5) == cd<T>(n+6)) &&
	   (cd<T>(n+6) == cd<T>(n+7)) &&
	    cd<T>(n) == 8){
        cout << n << endl;
    }
}

int main(){

	cout << "8 rennzoku" << endl;
	for(size_t i=2; i<static_cast<size_t>(18446744709551615); ++i){
		same<size_t>(i);
	}
/*	    
	cout << cd<size_t>(230) << endl;
	cout << cd<size_t>(231) << endl;
	cout << cd<size_t>(232) << endl;
*/				    
	return 0;
}
