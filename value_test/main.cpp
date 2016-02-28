#include <iostream>

//lvalue = rvalue;

class X{
public:
};

int f(){
	return 5;
}

void f(X&){
}

void g(X const &){
}

int main(int argc, char* argv[]){

	int i=10;

	i;	//lvalue sahennti
	10;	//rvalue uhennti

	int j = i;	// i uhennti kara sahennti

	X x;

	int();

	x;	//lvalue
	X();//rvalue

	f();//rvalue

	int&& a = 10;

	std::cout << a << std::endl;

	return 0;
}
