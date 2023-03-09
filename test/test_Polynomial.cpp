#include <gtest.h>
#include <string>
#include <sstream>
#include "polynomial.h"
using namespace std;

TEST(Polynomial, test_addition)
{
	istringstream s_in("x^2+x^1y^1+z^1;x^1+x^2y^2;");
	Polynomial a, b;
	s_in >> a >> b;
	EXPECT_EQ(11, (a + b).calculate(1, 2, 3));
}

TEST(Polynomial, test_multiplication)
{
	istringstream s_in("x^2+x^1y^1+z^1;x^1+x^2y^2;");
	Polynomial a, b;
	s_in >> a >> b;
	EXPECT_EQ(30, (a * b).calculate(1, 2, 3));
}
