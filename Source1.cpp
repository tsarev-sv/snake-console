#include <iostream>
using namespace std;

class ATMcontroller {
    bool isAvtorisovan;
};

int main5() {
    int x;
    double y;
    ATMcontroller atm;

    cout << "alignof(int): " << alignof(int) << endl;           // 4
    cout << "alignof(double): " << alignof(double) << endl;     // 8
    cout << "alignof(ATMcontroller): " << alignof(ATMcontroller) << endl; // 8

    cout << "&x: " << &x << ", кратен 4? " << ((long long)&x % 4 == 0) << endl;
    cout << "&y: " << &y << ", кратен 8? " << ((long long)&y % 8 == 0) << endl;
    cout << "&atm: " << &atm << ", кратен 8? " << ((long long)&atm % 8 == 0) << endl;

    return 0;
}