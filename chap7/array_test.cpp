# include <iostream>

// accept multi-dimension array
void f(int array[][3], int dim1) {

}

void g(int array[]) {
	std::cout << sizeof(array) << std::endl;
}

int main() {
	int array[4][3];
	f(array, 4);

	int a[5];
	std::cout << sizeof(a) << std::endl;
	g(a);

	return 0;
}
