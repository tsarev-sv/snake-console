#include <iostream>
#include <windows.h>
#include<chrono>

using namespace std;
using namespace std::chrono;

//2.3 задание
int viborka(int arr[], int size, int c) {
    for (int i = 0; i < size; i++) {
        if (arr[i] > c) {
            return i;
        }
    }
    return 0;
}

int viborka1(int arr[], int size, int c, int index) {

    if (index < size) {
        if (arr[index] > c) {
            return index;
        }
    }
    if (index >= size) {
        return 0;
    }
    return viborka1(arr, size, c, index + 1);
}

int main6() {

    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    int arr[] = { 2,-7,18,-21,3,8,-9,2,6,5,32 };

    int c;

    cout << "Введите число: ";

    cin >> c;

    int index = 0;

    int size = sizeof(arr) / sizeof(arr[0]);

    auto t3 = high_resolution_clock::now();
    int r2 = viborka1(arr, size, c, index);
    auto t4 = high_resolution_clock::now();

    cout << "Результат = " << r2 << ", время: " << duration_cast<nanoseconds>(t4 - t3).count() << " нс\n";

    auto t1 = high_resolution_clock::now();
    int r1 = viborka(arr, size, c);
    auto t2 = high_resolution_clock::now();

    cout << "Результат = " << r1 << ", время: " << duration_cast<nanoseconds>(t2 - t1).count() << " нс\n";



    return 0;
}