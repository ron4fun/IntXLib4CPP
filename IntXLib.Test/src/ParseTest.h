#define BOOST_TEST_MODULE ParseTest

#include "../IntX.h"
#include "../Utils/Constants.h"
#include <vector>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(ParseTest)

BOOST_AUTO_TEST_CASE(Zero)
{
	IntX int1 = IntX::Parse("0");
	BOOST_CHECK(int1 == 0);
}

BOOST_AUTO_TEST_CASE(WhiteSpace)
{
	IntX int1 = IntX::Parse("  7 ");
	BOOST_CHECK(int1 == 7);
}

BOOST_AUTO_TEST_CASE(Sign)
{
	IntX int1 = IntX::Parse("-7");
	BOOST_CHECK(int1 == -7);
	int1 = IntX::Parse("+7");
	BOOST_CHECK(int1 == 7);
}

BOOST_AUTO_TEST_CASE(Base)
{
	IntX int1 = IntX::Parse("abcdef", 16);
	BOOST_CHECK(int1 == 0xabcdef);

	int1 = IntX::Parse("100", 8);
	BOOST_CHECK(int1 == 64);

	int1 = IntX::Parse("0100");
	BOOST_CHECK(int1 == 64);

	int1 = IntX::Parse("0100000000000");
	BOOST_CHECK(int1 == 0x200000000UL);

	int1 = IntX::Parse("0xabcdef");
	BOOST_CHECK(int1 == 0xabcdef);

	int1 = IntX::Parse("0XABCDEF");
	BOOST_CHECK(int1 == 0xabcdef);

	int1 = IntX::Parse("020000000000");
	BOOST_CHECK(int1 == 0x80000000);
}

BOOST_AUTO_TEST_CASE(Null)
{
	try
	{
		IntX::Parse("");
	} // end try
	catch (const exception&e)
	{
		BOOST_CHECK(typeid(e) == typeid(ArgumentNullException));
		return;
	} // end catch

	BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(InvalidFormat)
{
	try
	{
		IntX::Parse("-123-");
	} // end try
	catch (const exception&e)
	{
		BOOST_CHECK(typeid(e) == typeid(FormatException));
		return;
	} // end catch

	BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(InvalidFormat2)
{
	try
	{
		IntX::Parse("abc");
	} // end try
	catch (const exception&e)
	{
		BOOST_CHECK(typeid(e) == typeid(FormatException));
		return;
	} // end catch

	BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(InvalidFormat3)
{
	try
	{
		IntX::Parse("987", 2);
	} // end try
	catch (const exception&e)
	{
		BOOST_CHECK(typeid(e) == typeid(FormatException));
		return;
	} // end catch

	BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(BigDec)
{
	IntX intX = IntX::Parse("34589238954389567586547689234723587070897800300450823748275895896384753238944985");
	BOOST_CHECK(intX.ToString() == "34589238954389567586547689234723587070897800300450823748275895896384753238944985");
}

BOOST_AUTO_TEST_SUITE_END()

