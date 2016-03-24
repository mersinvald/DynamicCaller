#include <iostream>
#include "dynamiccaller.hpp"

int plus(int a, int b) {
    return a+b;
}

int minus(int a, int b) {
    return a-b;
}

int multiply(int a, int b) {
    return a*b;
}

int divide(int a, int b) {
    return a/b;
}

#define AddDynFunc(DC, function) \
    DC.PutFunction(function, #function);

int main() {
    DynamicCaller<string, int(int, int)> DC;

    AddDynFunc(DC, plus);
    AddDynFunc(DC, minus);
    AddDynFunc(DC, multiply);
    AddDynFunc(DC, divide);

    std::string func;
    int a, b;

    while(1) {
        std::cin >> func >> a >> b;

        try {
            std::cout << DC.CallFunction(func, a, b) << std::endl;
        } catch(std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
}
