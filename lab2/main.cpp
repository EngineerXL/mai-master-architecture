#include <exception>
#include <iostream>

int main() {
    int a = 0, b = 1;
    bool flag = true;
    while (flag && b < 2e9) {
        ++a;
        if (a % 10 == 0) ++b;
    }
    std::cout << a << std::endl;
    std::cout << "Hello world!" << std::endl;
    return 0;
}
