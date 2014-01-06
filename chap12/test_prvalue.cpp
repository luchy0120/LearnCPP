#include <iostream>

void f(int& i) {
	std::cout << "Lvalue reference" << std::endl;
}

void f(int&& i) {
	std::cout << "Rvalue reference" << std::endl;
}

int main() {
	f(2);

	return 0;
}
