#include <iostream>

int f(int x) {
    return 5*x + 5;
}

int main() {

    std::cout << f(50);
    std::cout << "Cmake should be able to run this file" << std::endl;
    return 0;

}