#define BOOST_TEST_MODULE ToStringTest

#include "../IntX.h"
#include "../Utils/Constants.h"
#include <string>
#include <sstream>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(ToStringTest)

BOOST_AUTO_TEST_CASE(VerySimple)
{
	IntX intX = IntX(11);
	BOOST_CHECK(intX.ToString() == "11");
}

BOOST_AUTO_TEST_CASE(Simple)
{
	IntX intX = IntX(12345670);
	BOOST_CHECK(intX.ToString() == "12345670");
}

BOOST_AUTO_TEST_CASE(Zero)
{
	IntX intX = IntX();
	BOOST_CHECK(intX.ToString() == "0");
}

BOOST_AUTO_TEST_CASE(Neg)
{
	IntX intX = IntX(Constants::MinIntValue);

	ostringstream tempStrStream;
	tempStrStream << Constants::MinIntValue;

	BOOST_CHECK(intX.ToString() == tempStrStream.str());
}

BOOST_AUTO_TEST_CASE(Big)
{
	IntX intX = IntX(Constants::MaxIntValue);
	intX = intX + intX + intX;

	UInt64 longX = Constants::MaxIntValue;
	longX = longX + longX + longX;

	ostringstream longStrStream;
	longStrStream << longX;

	string temp1 = intX.ToString();
	string temp2 = longStrStream.str();

	BOOST_CHECK(intX.ToString() == longStrStream.str());
}

BOOST_AUTO_TEST_CASE(Binary)
{
	IntX intX = IntX(19);
	BOOST_CHECK(intX.ToString(2) == "10011");
}

BOOST_AUTO_TEST_CASE(Octal)
{
	IntX intX = IntX(100);
	BOOST_CHECK(intX.ToString(8) == "144");
}

BOOST_AUTO_TEST_CASE(Octal2)
{
	IntX intX = IntX(901);
	BOOST_CHECK(intX.ToString(8) == "1605");
}

BOOST_AUTO_TEST_CASE(Octal3)
{
	IntX intX = 0x80000000;
	BOOST_CHECK(intX.ToString(8) == "20000000000");

	intX = 0x100000000;
	BOOST_CHECK(intX.ToString(8) == "40000000000");
}

BOOST_AUTO_TEST_CASE(Hex)
{
	IntX intX = IntX(0xABCDEF);
	BOOST_CHECK(intX.ToString(16) == "ABCDEF");
}

BOOST_AUTO_TEST_CASE(HexLower)
{
	IntX intX = IntX(0xFF00FF00FF00FF);
	BOOST_CHECK(intX.ToString(16, false) == "ff00ff00ff00ff");
}

BOOST_AUTO_TEST_CASE(OtherBase)
{
	IntX intX = IntX(-144);
	BOOST_CHECK(intX.ToString(140) == "-{1}{4}");
}

BOOST_AUTO_TEST_SUITE_END()
