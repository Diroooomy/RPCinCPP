// #include "add.h"
// #pragma once

#include <iostream>
#include "age_sum.h"
#include "Calculator.h"
using namespace std;

int main() {
    int c = add(2,3);
    cout << c <<endl;
    cout << sub(1,2) <<endl;
    user user1 = {"as", 20};
    user user2 = {"wwe", 22};
    cout << age_sum(user1, user2) << endl;
}