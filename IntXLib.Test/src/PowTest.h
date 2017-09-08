#define BOOST_TEST_MODULE PowTest

#include "../IntX.h"
#include <string>
#include <fstream>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(PowTest)

BOOST_AUTO_TEST_CASE(Simple)
{
	IntX intX1 = IntX(-1);
	BOOST_CHECK(IntX::Pow(intX1, 17) == -1);
	BOOST_CHECK(IntX::Pow(intX1, 18) == 1);
}

BOOST_AUTO_TEST_CASE(Zero)
{
	IntX intX2 = IntX(0);
	BOOST_CHECK(IntX::Pow(intX2, 77) == 0);
}

BOOST_AUTO_TEST_CASE(PowZero)
{
	BOOST_CHECK(IntX::Pow(0, 0) == 1);
}

BOOST_AUTO_TEST_CASE(PowOne)
{
	BOOST_CHECK(IntX::Pow(7, 1) == 7);
}

BOOST_AUTO_TEST_CASE(Big)
{
	BOOST_CHECK(IntX::Pow(2, 65).ToString() == string("36893488147419103232"));
}

BOOST_AUTO_TEST_CASE(TwoNOut)
{
	string pow2Str = IntX::Pow(2, 65536).ToString();

	ofstream out_file("2n65536.txt", ios::out);
	out_file << pow2Str << endl;
	out_file.close();

	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()
