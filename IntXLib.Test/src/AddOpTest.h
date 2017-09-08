#define BOOST_TEST_MODULE AddOpTestCase

#include "../IntX.h"
#include "../Utils/Constants.h"
#include <string>
#include <vector>
#include <fstream>

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(AddOpTestCase)

BOOST_AUTO_TEST_CASE(Add2IntX)
{
	IntX int1 = IntX(3);
	IntX int2 = IntX(5);
	BOOST_CHECK(int1 + int2 == 8);
}

BOOST_AUTO_TEST_CASE(Add2IntXNeg)
{
	IntX int1 = IntX(-3);
	IntX int2 = IntX(-5);
	BOOST_CHECK(int1 + int2 == -8);
}

BOOST_AUTO_TEST_CASE(AddIntIntX)
{
	IntX intX = IntX(3);
	BOOST_CHECK(intX + 5 == 8);
}

BOOST_AUTO_TEST_CASE(AddIntXInt)
{
	IntX intX = IntX(3);
	BOOST_CHECK((IntX)5 + intX == 8);
}
	
BOOST_AUTO_TEST_CASE(Add0IntX)
{
	IntX int1 = IntX(3);
	BOOST_CHECK(int1 + 0 == 3);
	BOOST_CHECK(int1 + IntX() == 3);
}

BOOST_AUTO_TEST_CASE(AddIntX0)
{
	IntX int1 = IntX(-3);
	BOOST_CHECK(int1 + 0 == -3);
	BOOST_CHECK(int1 + IntX() == -3);
	BOOST_CHECK(IntX(0) + IntX(-1) == -1);
}
	
BOOST_AUTO_TEST_CASE(Add2BigIntX)
{
	IntX int1 = IntX(vector<UInt32>({ 1, 2, 3 }), false);
	IntX int2 = IntX(vector<UInt32>({ 3, 4, 5 }), false);
	IntX int3 = IntX(vector<UInt32>({ 4, 6, 8 }), false);
	BOOST_CHECK(int1 + int2 == int3);
}

BOOST_AUTO_TEST_CASE(Add2BigIntXC)
{
	IntX int1 = IntX(vector<UInt32>({ Constants::MaxUInt32Value, Constants::MaxUInt32Value - 1 }), false);
	IntX int2 = IntX(vector<UInt32>({ 1, 1 }), false);
	IntX int3 = IntX(vector<UInt32>({ 0, 0, 1 }), false);
	BOOST_CHECK(int1 + int2 == int3);
}

BOOST_AUTO_TEST_CASE(Add2BigIntXC2)
{
	IntX int1 = IntX(vector<UInt32>({ Constants::MaxUInt32Value - 1, Constants::MaxUInt32Value - 1 }), false);
	IntX int2 = IntX(vector<UInt32>({ 1, 1 }), false);
	IntX int3 = IntX(vector<UInt32>({ Constants::MaxUInt32Value, Constants::MaxUInt32Value }), false);
	BOOST_CHECK(int1 + int2 == int3);
}

BOOST_AUTO_TEST_CASE(Add2BigIntXC3)
{
	IntX int1 = IntX(vector<UInt32>({ Constants::MaxUInt32Value, Constants::MaxUInt32Value }), false);
	IntX int2 = IntX(vector<UInt32>({ 1, 1 }), false);
	IntX int3 = IntX(vector<UInt32>({ 0, 1, 1 }), false);
	BOOST_CHECK(int1 + int2 == int3);
}

BOOST_AUTO_TEST_CASE(Add2BigIntXC4)
{
	IntX int1 = IntX(vector<UInt32>({ Constants::MaxUInt32Value, Constants::MaxUInt32Value, 1, 1 }), false);
	IntX int2 = IntX(vector<UInt32>({ 1, 1 }), false);
	IntX int3 = IntX(vector<UInt32>({ 0, 1, 2, 1 }), false);
	BOOST_CHECK(int1 + int2 == int3);
}

BOOST_AUTO_TEST_CASE(Fibon)
{
	IntX int1 = IntX(1);
	IntX int2 = int1;
	IntX int3 = IntX();
	for (UInt32 i = 0; i < 10000; ++i)
	{
		int3 = int1 + int2;
		int1 = int2;
		int2 = int3;
	} // end for
	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(AddSub)
{
	IntX int1 = IntX(2);
	IntX int2 = IntX(-2);
	BOOST_CHECK(int1 + int2 == 0);
}

BOOST_AUTO_TEST_CASE(FibonOut)
{
	UInt32 numberBase = 16;

	ofstream out_file("fibon.txt", ios::out);

	IntX int1 = IntX(1);

	out_file << int1.ToString(numberBase) << endl;

	IntX int2 = int1;
	out_file << int2.ToString(numberBase) << endl;

	IntX int3 = IntX();
	for (UInt32 i = 0; i < 1000; ++i)
	{
		int3 = int1 + int2;
		out_file << int3.ToString(numberBase) << endl;
		int1 = int2;
		int2 = int3;
	} // end for

	out_file.close();

	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()
