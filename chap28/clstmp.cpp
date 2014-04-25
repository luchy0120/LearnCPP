#include <iostream>

using namespace std;

// here is a normal class
// struct A {
//    double f(double& d) {
//        return d + 1;
//    }
// };

// template<typename T>
// struct A {
//     T f(T& t) {
//         return t + 1;
//     }
// };

double f(double d) {
    return d + 1;
}

template<typename T>
T f(T t) {
    return t + 2;
}

int main() {
    cout << f(1.0) << endl;
    cout << f(1) << endl;
    return 0;
}
