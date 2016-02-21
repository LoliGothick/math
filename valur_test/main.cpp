#include <bits/stdc++.h>

template <typename T>
T max(T&& a, T&& b){
	return (a<b)?b:a;
}

template <typename... Ts, typename T>
T maxa(T&& head, Ts&&... tail){
	return max(std::forward<T>(head), max(std::forward<Ts>(tail)...));
}

int main(int argc, char* argv[]){

	std::cout << maxa(1,2,3) << std::endl; //OK
	//std::cout << maxa(1,2,3,4) << std::endl; // NG

	return 0;
}
