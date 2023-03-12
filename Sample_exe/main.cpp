#include <iostream>
#include <fstream>
#include "polynomial.h"
using namespace std;

int main() {
	ifstream fin(R"(..\..\Sample_exe\example.txt)"); // lab4-Polynomial\Sample_exe\example.txt
	Polynomial p1, p2;
	fin >> p1 >> p2;
	cout << "p1: " << p1 << endl << "p2: " << p2 << endl;
	Polynomial t;
	t = p1 * p2;
	cout << "p1 * p2: ";
	cout << t << "\n";
	int x, y, z;
	fin >> x >> y >> z;
	cout << "x = " << x << ", y = " << y << ", z = " << z << endl;
	cout << t.calculate(x, y, z) << endl;
	return 0;
}
