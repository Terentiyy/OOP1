#include <iostream>

#include "src/Seven.h"

int main() {
    Seven left{30}, right{20}, expected{10};
    Seven result = left.substract(right);

    result.print();
    expected.print();
    std::cout << expected.equals(result) << std::endl;
}