#include<iostream>
#include <filesystem>
#include <windows.h>

using namespace std;
namespace fs = std::filesystem;


int  main4() {
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);

	cout << "Текущая папка: " << fs::current_path() << endl;

	return 0;
}
