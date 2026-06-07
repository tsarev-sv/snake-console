#include <iostream>
#include <windows.h>
#include<chrono>
#include<cmath>

using namespace std;
using namespace std::chrono;

//2.2 задание
double f(double sum, int b) {
    if (b >= 1) {
        for (int k = 0; k <= b; k++) {
            sum += pow(-1, k) / pow(2 * k + 1, 3);
        }
    }
    return sum;
}

double f1(int b, double l, int k = 0) {
    if (b <= 0) {
        return 0;
    }
    if (k > b) {
        return l;
    }
    l += pow(-1, k) / pow(2 * k + 1, 3);
    return f1(b, l, k + 1);

}

int main3() {

    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    const double pi = 3.14159;
    double a = pow(pi, 3) / 32;

    double sum = 0;
    double l = 0;

    int b;
    cout << "Введите верхний предел суммы: ";
    cin >> b;


    auto t3 = high_resolution_clock::now();
    double r2 = f1(b, l);
    auto t4 = high_resolution_clock::now();

    auto t1 = high_resolution_clock::now();
    double r1 = f(sum, b);
    auto t2 = high_resolution_clock::now();

    cout << "Значение = " << r2 << ", время: " << duration_cast<nanoseconds>(t4 - t3).count() << " нс\n";
    cout << "Значение = " << r1 << ", время: " << duration_cast<nanoseconds>(t2 - t1).count() << " нс\n";


    cout << "(pi^3)/32 = " << a;

    return 0;
}
