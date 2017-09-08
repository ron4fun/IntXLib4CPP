#include "stdafx.h"

#include <cstdlib>
#include <typeinfo>
#include <cmath>

#include "OpHelpers/OpHelper.h"
#include "OpHelpers/DigitOpHelper.h"
#include "OpHelpers/DigitHelper.h"

#include "Parsers/ParseManager.h"
#include "Multipliers/MultiplyManager.h"
#include "Dividers/DivideManager.h"
#include "StringConverters/StringConvertManager.h"

#include "IntX.h"

using namespace std;

// Initialize static variable
IntXGlobalSettings IntX::globalSettings = IntXGlobalSettings();

//==================================================================
//  Constructors
//==================================================================
//
IntX::IntX(const int value)
{
	if (value == 0)
	{
		// Very specific fast processing for zero values
		InitFromZero();
	} // end if
	else
	{
		// Prepare internal fields
		length = 1;
		digits = vector<UInt32>(length);

		// Fill the only big integer digit
		DigitHelper::ToUInt32WithSign(value, digits[0], negative);
	} // end else
} // end constructor

IntX::IntX(const UInt32 value)
{
	if (value == 0)
	{
		// Very specific fast processing for zero values
		InitFromZero();
	} // end if
	else
	{
		// Prepare internal fields
		length = 1;
		digits = vector<UInt32>(length);
		digits[0] = value;
	} // end else
} // end constructor

IntX::IntX(const long long value)
{
	if (value == 0)
	{
		// Very specific fast processing for zero values
		InitFromZero();
	} // end if
	else
	{
		// Fill the only big integer digit
		UInt64 newValue;
		DigitHelper::ToUInt64WithSign(value, newValue, negative);
		InitFromUlong(newValue);
	} // end else
} // end constructor

IntX::IntX(const unsigned long value)
{
	if (value == 0)
	{
		// Very specific fast processing for zero values
		InitFromZero();
	} // end if
	else
	{
		InitFromUlong((UInt64)value);
	} // end else
} // end constructor

IntX::IntX(const UInt64 value)
{
	if (value == 0)
	{
		// Very specific fast processing for zero values
		InitFromZero();
	} // end if
	else
	{
		InitFromUlong(value);
	} // end else
} // end constructor

IntX::IntX(const float value)
{
	// Exceptions
	if (value == FP_INFINITE)
		throw OverflowException(Strings::Overflow_TIntXInfinity);
	if (value == FP_NAN)
		throw OverflowException(Strings::Overflow_NotANumber);

	OpHelper::SetDigitsFromDouble(value, digits, *this);
} // end .cctor float

IntX::IntX(const double value)
{
	// Exceptions
	if (value == FP_INFINITE)
		throw OverflowException(Strings::Overflow_TIntXInfinity);
	if (value == FP_NAN)
		throw OverflowException(Strings::Overflow_NotANumber);

	OpHelper::SetDigitsFromDouble(value, digits, *this);
} // end .cctor double

IntX::IntX(const char *value)
{
	IntX intX = Parse(value);
	InitFromIntX(intX);
} // end constructor 

IntX::IntX(const char *value, const UInt32 numberBase)
{
	IntX intX = Parse(value, numberBase);
	InitFromIntX(intX);
} // end constructor

IntX::IntX(const string &value)
{
	IntX intX = Parse(value);
	InitFromIntX(intX);
} // end constructor 

IntX::IntX(const string &value, const UInt32 numberBase)
{
	IntX intX = Parse(value, numberBase);
	InitFromIntX(intX);
} // end constructor

IntX::IntX(const IntX &value)
{
	InitFromIntX(value);
} // end .cctor

IntX::IntX(const UInt32 length, const bool negative)
{
	this->length = length;
	this->digits = vector<UInt32>(length);
	this->negative = negative;
} // end constructor

IntX::IntX(const UInt32 *digits, const bool negative, const UInt32 length)
{
	// Exceptions
	if (digits == nullptr)
	{
		throw ArgumentNullException("values");
	} // end if

	InitFromDigits(digits, negative, length);
} // end constructor

IntX::IntX(const vector<UInt32> &digits, const bool negative, const UInt32 length)
{
	InitFromDigits(digits, negative, length);
} // end constructor


//==================================================================
//  Comparable implementation
//==================================================================


/// <summary>
/// Compares current object with another big integer.
/// </summary>
/// <param name="n">Big integer to compare with.</param>
/// <returns>1 if object is bigger than <paramref name="n" />, -1 if object is smaller than <paramref name="n" />, 0 if they are equal.</returns>
int IntX::CompareTo(const IntX &n) const
{
	return OpHelper::Cmp(*this, n, true);
} // end function CompareTo

/// <summary>
/// Compares current object with another integer.
/// </summary>
/// <param name="n">Integer to compare with.</param>
/// <returns>1 if object is bigger than <paramref name="n" />, -1 if object is smaller than <paramref name="n" />, 0 if they are equal.</returns>
int IntX::CompareTo(const int n) const
{
	return OpHelper::Cmp(*this, n);
} // end function CompareTo

/// <summary>
/// Compares current object with another unsigned integer.
/// </summary>
/// <param name="n">Unsigned integer to compare with.</param>
/// <returns>1 if object is bigger than <paramref name="n" />, -1 if object is smaller than <paramref name="n" />, 0 if they are equal.</returns>
int IntX::CompareTo(const UInt32 n) const
{
	return OpHelper::Cmp(*this, n);
} // end function CompareTo

/// <summary>
/// Compares current object with another long integer.
/// </summary>
/// <param name="n">Long integer to compare with.</param>
/// <returns>1 if object is bigger than <paramref name="n" />, -1 if object is smaller than <paramref name="n" />, 0 if they are equal.</returns>
int IntX::CompareTo(const long long n) const
{
	return OpHelper::Cmp(*this, n, true);
} // end function CompareTo

/// <summary>
/// Compares current object with another unsigned long integer.
/// </summary>
/// <param name="n">Unsigned long integer to compare with.</param>
/// <returns>1 if object is bigger than <paramref name="n" />, -1 if object is smaller than <paramref name="n" />, 0 if they are equal.</returns>
int IntX::CompareTo(const UInt64 n) const
{
	return OpHelper::Cmp(*this, n, true);
} // end function CompareTo

int IntX::CompareTo(const string &n) const
{
	return OpHelper::Cmp(*this, (IntX)n, true);
} // end function CompareTo


//==================================================================
//  Parsing methods
//==================================================================


/// <summary>
/// Parses provided string representation of <see cref="IntX" /> object in decimal base.
/// If number starts from "0" then it's treated as octal; if number starts fropm "0x"
/// then it's treated as hexadecimal.
/// </summary>
/// <param name="value">Number as string.</param>
/// <returns>Parsed object.</returns>
IntX IntX::Parse(const string &value)
{
	return ParseManager::GetCurrentParser()->Parse(value, 10U, Constants::BaseCharToDigits, true);
} // end function Parse

/// <summary>
/// Parses provided string representation of <see cref="IntX" /> object.
/// </summary>
/// <param name="value">Number as string.</param>
/// <param name="numberBase">Number base.</param>
/// <returns>Parsed object.</returns>
IntX IntX::Parse(const string &value, const UInt32 numberBase)
{
	return ParseManager::GetCurrentParser()->Parse(value, numberBase, Constants::BaseCharToDigits, false);
} // end function Parse

/// <summary>
/// Parses provided string representation of <see cref="IntX" /> object using custom alphabet.
/// </summary>
/// <param name="value">Number as string.</param>
/// <param name="numberBase">Number base.</param>
/// <param name="alphabet">Alphabet which contains chars used to represent big integer, char position is coresponding digit value.</param>
/// <returns>Parsed object.</returns>
IntX IntX::Parse(const string &value, const UInt32 numberBase, const string &alphabet)
{
	return ParseManager::GetCurrentParser()->Parse(value, numberBase, StrRepHelper::CharDictionaryFromAlphabet(alphabet, numberBase), false);
} // end function Parse

/// <summary>
/// Parses provided string representation of <see cref="IntX" /> object in decimal base.
/// If number starts from "0" then it's treated as octal; if number starts fropm "0x"
/// then it's treated as hexadecimal.
/// </summary>
/// <param name="value">Number as string.</param>
/// <param name="mode">Parse mode.</param>
/// <returns>Parsed object.</returns>
IntX IntX::Parse(const string &value, ParseMode mode)
{
	return ParseManager::GetParser(mode)->Parse(value, 10U, Constants::BaseCharToDigits, true);
} // end function Parse

/// <summary>
/// Parses provided string representation of <see cref="IntX" /> object.
/// </summary>
/// <param name="value">Number as string.</param>
/// <param name="numberBase">Number base.</param>
/// <param name="mode">Parse mode.</param>
/// <returns>Parsed object.</returns>
IntX IntX::Parse(const string &value, const UInt32 numberBase, ParseMode mode)
{
	return ParseManager::GetParser(mode)->Parse(value, numberBase, Constants::BaseCharToDigits, false);
} // end function Parse

/// <summary>
/// Parses provided string representation of <see cref="IntX" /> object using custom alphabet.
/// </summary>
/// <param name="value">Number as string.</param>
/// <param name="numberBase">Number base.</param>
/// <param name="alphabet">Alphabet which contains chars used to represent big integer, char position is coresponding digit value.</param>
/// <param name="mode">Parse mode.</param>
/// <returns>Parsed object.</returns>
IntX IntX::Parse(const string &value, const UInt32 numberBase, const string &alphabet, ParseMode mode)
{
	return ParseManager::GetParser(mode)->Parse(value, numberBase, StrRepHelper::CharDictionaryFromAlphabet(alphabet, numberBase), false);
} // end function Parse


//==================================================================
//  ToString overrides
//==================================================================


/// <summary>
/// Returns decimal string representation of this <see cref="IntX" /> object.
/// </summary>
/// <returns>Decimal number in string.</returns>
string IntX::ToString() const
{
	return ToString(10U, true);
} // end function ToString

/// <summary>
/// Returns string representation of this <see cref="IntX" /> object in given base.
/// </summary>
/// <param name="numberBase">Base of system in which to do output.</param>
/// <returns>Object string representation.</returns>
string IntX::ToString(const UInt32 numberBase) const
{
	return ToString(numberBase, true);
} // end function ToString

/// <summary>
/// Returns string representation of this <see cref="IntX" /> object in given base.
/// </summary>
/// <param name="numberBase">Base of system in which to do output.</param>
/// <param name="upperCase">Use uppercase for bases from 11 to 16 (which use letters A-F).</param>
/// <returns>Object string representation.</returns>
string IntX::ToString(const UInt32 numberBase, const bool upperCase) const
{
	return StringConvertManager::GetStringConverter(settings.getToStringMode())
		->ToString(*this, numberBase, upperCase ? Constants::BaseUpperChars : Constants::BaseLowerChars);
} // end function ToString

/// <summary>
/// Returns string representation of this <see cref="IntX" /> object in given base using custom alphabet.
/// </summary>
/// <param name="numberBase">Base of system in which to do output.</param>
/// <param name="alphabet">Alphabet which contains chars used to represent big integer, char position is coresponding digit value.</param>
/// <returns>Object string representation.</returns>
string IntX::ToString(const UInt32 numberBase, const string &alphabet) const
{
	StrRepHelper::AssertAlphabet(alphabet, numberBase);
	return StringConvertManager::GetStringConverter(settings.getToStringMode())->ToString(*this, numberBase, alphabet);
} // end function ToString


//==================================================================
//  Other public methods
//==================================================================


/// <summary>
/// Frees extra space not used by digits.
/// </summary>
void IntX::Normalize()
{
	if (this->digits.size() > length)
	{
		vector<UInt32> newDigits(length);
		memcpy(&newDigits[0], &digits[0], sizeof(UInt32) * length);
		this->digits = vector<UInt32>(newDigits);
		// alternative
		// digits.resize(length);
	} // end if

	if (length == 0)
	{
		negative = false;
	} // end if
} // end function Normalize

/// <summary>
/// Retrieves this <see cref="IntX" /> internal state as digits array and sign.
/// Can be used for serialization and other purposes.
/// Note: please use constructor instead to clone <see cref="IntX" /> object.
/// </summary>
/// <param name="digits">Digits array.</param>
/// <param name="negative">Is negative integer.</param>
void IntX::GetInternalState(vector<UInt32> &digitsTo, bool &negativeTo) const
{
	digitsTo = vector<UInt32>(this->digits);
	negativeTo = this->negative;
} // end function GetInternalState

/// <summary>
/// Return if <see cref="IntX" /> object is negative.
/// </summary>
bool IntX::IsNegative() const
{
	return this->negative;
} // end function IsNegative

/// <summary>
/// Return if <see cref="IntX" /> object is zero.
/// </summary>
bool IntX::IsZero() const
{
	return this->length == 0 || this->digits.empty();
} // end function IsNegative

//==================================================================
//  Other public static methods
//==================================================================

/// <summary>
/// Returns a Non-Negative Random <see cref="TIntX" /> object using Pcg Random.
/// </summary>
/// <returns>Random TIntX value.</returns>
IntX IntX::Random()
{
	return OpHelper::Random();
} // end function Random

/// <summary>
/// Returns a Non-Negative Random <see cref="TIntX" /> object using Pcg Random within the specified Range. (Max not Included)
/// </summary>
/// <param name="Min">Minimum value.</param>
/// <param name="Max">Maximum value (Max not Included)</param>
/// <returns>Random TIntX value.</returns>
IntX IntX::RandomRange(const UInt32 Min, const UInt32 Max)
{
	return OpHelper::RandomRange(Min, Max);
} // end function RandomRange

/// <summary>
/// Returns a non negative big integer.
/// </summary>
/// <param name="value">value to get its absolute value.</param>
/// <returns>Absolute number.</returns>
IntX IntX::AbsoluteValue(const IntX &value)
{
	return OpHelper::AbsoluteValue(value);
} // end function AbsoluteValue

/// <summary>
/// Returns a specified big integer raised to the power of 2.
/// </summary>
/// <param name="value">Number to get its square.</param>
/// <returns>Squared number.</returns>
IntX IntX::Square(const IntX &value)
{
	return OpHelper::Square(value);
} // end function Square

/// <summary>
/// Calculates Integer SquareRoot of <see cref="TIntX" /> object
/// </summary>
/// <param name="value">value to get Integer squareroot of.</param>
/// <returns>Integer SquareRoot.</returns>
/// <seealso href="http://www.dahuatu.com/RkWdPBx6W8.html">[IntegerSquareRoot Implementation]</seealso>
IntX IntX::IntegerSquareRoot(const IntX &value)
{
	if (value.negative)
		throw ArgumentException(Strings::NegativeSquareRoot + string(" value"));

	return OpHelper::IntegerSquareRoot(value);
} //end function IntegerSquareRoot

/// <summary>
/// Returns a specified big integer holding the factorial of value.
/// </summary>
/// <param name="value">Number to get its factorial.</param>
/// <returns>factorialed number.</returns>
/// <exception cref="EArgumentException"><paramref name="value" /> is a negative value.</exception>
IntX IntX::Factorial(const IntX &value)
{
	return OpHelper::Factorial(value);
} // end function Factorial

/// <summary>
/// (Optimized GCD).
/// Returns a specified big integer holding the GCD (Greatest common Divisor) of
/// two big integers using Binary GCD (Stein's algorithm).
/// </summary>
/// <param name="int1">First big integer.</param>
/// <param name="int2">Second big integer.</param>
/// <returns>GCD number.</returns>
/// <seealso href="http://lemire.me/blog/archives/2013/12/26/fastest-way-to-compute-the-greatest-common-divisor/">[GCD Implementation]</seealso>
/// <seealso href="https://hbfs.wordpress.com/2013/12/10/the-speed-of-gcd/">[GCD Implementation Optimizations]</seealso>
IntX IntX::GCD(const IntX &int1, const IntX &int2)
{
	return OpHelper::GCD(int1, int2);
} // end function GCD

/// <summary>
/// (LCM).
/// Returns a specified big integer holding the LCM (Least Common Multiple) of
/// two big integers.
/// </summary>
/// <param name="int1">First big integer.</param>
/// <param name="int2">Second big integer.</param>
/// <returns>LCM number.</returns>
/// <exception cref="EArgumentNilException"><paramref name="int1" /> is a null reference.</exception>
/// <exception cref="EArgumentNilException"><paramref name="int2" /> is a null reference.</exception>
IntX IntX::LCM(const IntX &int1, const IntX &int2)
{
	return OpHelper::LCM(int1, int2);
} // end function LCM

/// <summary>
/// Calculate Modular Inverse for two <see cref="TIntX" /> objects using Euclids Extended Algorithm.
/// returns Zero if no Modular Inverse Exists for the Inputs
/// </summary>
/// <param name="int1">First big integer.</param>
/// <param name="int2">Second big integer.</param>
/// <returns>Modular Inverse.</returns>
/// <seealso href="https://en.wikipedia.org/wiki/Modular_multiplicative_inverse">[Modular Inverse Explanation]</seealso>
/// <seealso href="http://www.di-mgt.com.au/euclidean.html">[Modular Inverse Implementation]</seealso>
IntX IntX::InvMod(const IntX &int1, const IntX &int2)
{
	if (int1.negative || int2.negative)
		throw ArgumentException(Strings::InvModNegativeNotAllowed);

	return OpHelper::InvMod(int1, int2);
} // end function InvMod

/// <summary>
/// Calculates Calculates Modular Exponentiation of <see cref="TIntX" /> object.
/// </summary>
/// <param name="value">value to compute ModPow of.</param>
/// <param name="exponent">exponent to use.</param>
/// <param name="modulus">modulus to use.</param>
/// <returns>Computed value.</returns>
/// <seealso href="https://en.wikipedia.org/wiki/Modular_exponentiation">[Modular Exponentiation Explanation]</seealso>
IntX IntX::ModPow(const IntX &value, const IntX &exponent, const IntX &modulus)
{
	if (modulus <= 0)
		throw ArgumentException(Strings::ModPowModulusCantbeZeroorNegative);

	if (exponent.negative)
		throw ArgumentException(Strings::ModPowExponentCantbeNegative);

	return OpHelper::ModPow(value, exponent, modulus);
} // end function ModPow

/// <summary>
/// Calculates Bézoutsidentity for two <see cref="TIntX" /> objects using Euclids Extended Algorithm
/// </summary>
/// <param name="int1">first value.</param>
/// <param name="int2">second value.</param>
/// <param name="bezOne">first bezout value.</param>
/// <param name="bezTwo">second bezout value.</param>
/// <returns>GCD (Greatest Common Divisor) value.</returns>
/// <seealso href="https://en.wikipedia.org/wiki/Bézout's_identity">[Bézout's identity Explanation]</seealso>
/// <seealso href="https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm#Pseudocode">[Bézout's identity Pseudocode using Extended Euclidean algorithm]</seealso>
IntX IntX::Bezoutsidentity(const IntX &int1, const IntX &int2, IntX &bezOne, IntX &bezTwo)
{
	return OpHelper::Bezoutsidentity(int1, int2, bezOne, bezTwo);
} // end function Bezoutsidentity

/// <summary>
/// Checks if a <see cref="TIntX" /> object is Probably Prime using Miller–Rabin primality test.
/// </summary>
/// <param name="value">big integer to check primality.</param>
/// <param name="Accuracy">Accuracy parameter `k´ of the Miller-Rabin algorithm. Default is 5. The execution time is proportional to the value of the accuracy parameter.</param>
/// <returns>Boolean value.</returns>
/// <seealso href="https://en.wikipedia.org/wiki/Miller–Rabin_primality_test">[Miller–Rabin primality test Explanation]</seealso>
/// <seealso href="https://github.com/cslarsen/miller-rabin">[Miller–Rabin primality test Implementation in C]</seealso>
bool IntX::IsProbablyPrime(const IntX &value, const int Accuracy)
{
	return OpHelper::IsProbablyPrime(value, Accuracy);
} // end function IsProbablyPrime

/// <summary>
/// The Max Between Two TIntX values.
/// </summary>
/// <param name="left">left value.</param>
/// <param name="right">right value.</param>
/// <returns>The Maximum TIntX value.</returns>
IntX IntX::Max(const IntX &left, const IntX &right)
{
	return OpHelper::Max(left, right);
} // end function Max

/// <summary>
/// The Min Between Two TIntX values.
/// </summary>
/// <param name="left">left value.</param>
/// <param name="right">right value.</param>
/// <returns>The Minimum TIntX value.</returns>
IntX IntX::Min(const IntX &left, const IntX &right)
{
	return OpHelper::Min(left, right);
} // end function Min

/// <summary>
/// The base-10 logarithm of the value.
/// </summary>
/// <param name="value">The value.</param>
/// <returns>The base-10 logarithm of the value.</returns>
/// <remarks> Source : Microsoft .NET Reference on GitHub </remarks>
double IntX::Log10(const IntX &value)
{
	return OpHelper::Log10(value);
} // end function Log10

/// <summary>
/// Calculates the natural logarithm of the value.
/// </summary>
/// <param name="value">The value.</param>
/// <returns>The natural logarithm.</returns>
/// <remarks> Source : Microsoft .NET Reference on GitHub </remarks>
double IntX::Ln(const IntX &value)
{
	return OpHelper::Ln(value);
} // end function Ln

/// <summary>
/// Calculates Logarithm of a number <see cref="TIntX" /> object for a specified base.
/// the largest power the base can be raised to that does not exceed the number.
/// </summary>
/// <param name="base">base.</param>
/// <param name="value">number to get log of.</param>
/// <returns>Log value.</returns>
/// <remarks> Source : Microsoft .NET Reference on GitHub </remarks>
double IntX::LogN(const double base, const IntX &value)
{
	return OpHelper::LogN(base, value);
} // end function LogN

/// <summary>
/// Calculates Integer Logarithm of a number <see cref="TIntX" /> object for a specified base.
/// the largest power the base can be raised to that does not exceed the number.
/// </summary>
/// <param name="base">base.</param>
/// <param name="number">number to get Integer log of.</param>
/// <returns>Integer Log.</returns>
/// <seealso href="http://gist.github.com/dharmatech/409723">[IntegerLogN Implementation]</seealso>
IntX IntX::IntegerLogN(const IntX &base, const IntX &number)
{
	if (base == 0 || number == 0)
		throw ArgumentException(Strings::LogCantComputeZero);

	if (base.negative || number.negative)
		throw ArgumentException(Strings::LogNegativeNotAllowed);

	return OpHelper::IntegerLogN(base, number);
} // end IntegerLogN


//==================================================================
//  IEquatable/Equals/GetHashCode implementation/overrides
//==================================================================


/// <summary>
/// Returns equality of this <see cref="IntX" /> with another big integer.
/// </summary>
/// <param name="n">Big integer to compare with.</param>
/// <returns>True if equals.</returns>
bool IntX::Equals(const IntX &n) const
{
	return *this == n;
} // end function Equals

/// <summary>
/// Returns hash code for this <see cref="IntX" /> object.
/// </summary>
/// <returns>Object hash code.</returns>
int IntX::GetHashCode() const
{
	switch (this->length)
	{
	case 0:
		return 0;
	case 1:
		return (int)(this->digits[0] ^ this->length ^ (this->negative ? 1 : 0));
	default:
		return (int)(this->digits[0] ^ this->digits[this->length - 1] ^ this->length ^ (this->negative ? 1 : 0));
	} // end switch
} // end function GetHashCode


//==================================================================
//  Math static methods
//==================================================================


/// <summary>
/// Multiplies one <see cref="IntX" /> object on another.
/// </summary>
/// <param name="int1">First big integer.</param>
/// <param name="int2">Second big integer.</param>
/// <param name="mode">Multiply mode set explicitly.</param>
/// <returns>Multiply result.</returns>
IntX IntX::Multiply(const IntX &int1, const IntX &int2, MultiplyMode mode)
{
	return MultiplyManager::GetMultiplier(mode)->Multiply(int1, int2);
} // end function Multiply

/// <summary>
/// Divides one <see cref="IntX" /> object by another.
/// </summary>
/// <param name="int1">First big integer.</param>
/// <param name="int2">Second big integer.</param>
/// <param name="mode">Divide mode.</param>
/// <returns>Division result.</returns>
IntX IntX::Divide(const IntX &int1, const IntX &int2, DivideMode mode)
{
	IntX modRes;
	return DivideManager().GetDivider(mode)->DivMod(int1, int2, modRes, DivModResultFlags::dmrfDiv);
} // end function Divide

/// <summary>
/// Divides one <see cref="IntX" /> object by another and returns division modulo.
/// </summary>
/// <param name="int1">First big integer.</param>
/// <param name="int2">Second big integer.</param>
/// <param name="mode">Divide mode.</param>
/// <returns>Modulo result.</returns>
IntX IntX::Modulo(const IntX &int1, const IntX &int2, DivideMode mode)
{
	IntX modRes;
	DivideManager().GetDivider(mode)->DivMod(int1, int2, modRes, DivModResultFlags::dmrfMod);
	return modRes;
} // end function Modulo

/// <summary>
/// Divides one <see cref="IntX" /> object on another.
/// Returns both divident and remainder
/// </summary>
/// <param name="int1">First big integer.</param>
/// <param name="int2">Second big integer.</param>
/// <param name="modRes">Remainder big integer.</param>
/// <returns>Division result.</returns>
IntX IntX::DivideModulo(const IntX &int1, const IntX &int2, IntX &modRes)
{
	return DivideManager().GetCurrentDivider()->DivMod(int1, int2, modRes, (DivModResultFlags)((int)DivModResultFlags::dmrfDiv | (int)DivModResultFlags::dmrfMod));
} // end function DivideModulo

/// <summary>
/// Divides one <see cref="IntX" /> object on another.
/// Returns both divident and remainder
/// </summary>
/// <param name="int1">First big integer.</param>
/// <param name="int2">Second big integer.</param>
/// <param name="modRes">Remainder big integer.</param>
/// <param name="mode">Divide mode.</param>
/// <returns>Division result.</returns>
IntX IntX::DivideModulo(const IntX &int1, const IntX &int2, IntX &modRes, DivideMode mode)
{
	return DivideManager().GetDivider(mode)->DivMod(int1, int2, modRes, (DivModResultFlags)((int)DivModResultFlags::dmrfDiv | (int)DivModResultFlags::dmrfMod));
} // end function DivideModulo

/// <summary>
/// Returns a specified big integer raised to the specified power.
/// </summary>
/// <param name="value">Number to raise.</param>
/// <param name="power">Power.</param>
/// <returns>Number in given power.</returns>
IntX IntX::Pow(const IntX &value, const UInt32 power)
{
	return OpHelper::Pow(value, power, globalSettings.getMultiplyMode());
} // end function Pow

/// <summary>
/// Returns a specified big integer raised to the specified power.
/// </summary>
/// <param name="value">Number to raise.</param>
/// <param name="power">Power.</param>
/// <param name="multiplyMode">Multiply mode set explicitly.</param>
/// <returns>Number in given power.</returns>
IntX IntX::Pow(const IntX &value, const UInt32 power, MultiplyMode mode)
{
	return OpHelper::Pow(value, power, mode);
} // end function Pow


//==================================================================
//  Operators
//==================================================================

//==================================================================
//  Assignment operator=
//==================================================================

// assignment operator
const IntX &IntX::operator =(const IntX &value)
{
	if(*this != value)
		InitFromIntX(value);
	return *this;
} // end function operator=


//==================================================================
//  operator==
//==================================================================


/// <summary>
/// Compares two <see cref="IntX" /> objects and returns true if their internal state is equal.
/// </summary>
/// <param name="int1">First big integer.</param>
/// <param name="int2">Second big integer.</param>
/// <returns>True if equals.</returns>
bool IntX::operator==(const IntX &int2) const
{
	return OpHelper::Cmp(*this, int2, false) == 0;
} // end function operator==


//==================================================================
//  operator!=
//==================================================================


/// <summary>
/// Compares two <see cref="IntX" /> objects and returns true if their internal state is not equal.
/// </summary>
/// <param name="int1">First big integer.</param>
/// <param name="int2">Second big integer.</param>
/// <returns>True if not equals.</returns>
bool IntX::operator!=(const IntX &int2) const
{
	return !(*this == int2);
} // end function operator!=


//==================================================================
//  operator>
//==================================================================


/// <summary>
/// Compares two <see cref="IntX" /> objects and returns true if first is greater.
/// </summary>
/// <param name="int1">First big integer.</param>
/// <param name="int2">Second big integer.</param>
/// <returns>True if first is greater.</returns>
bool IntX::operator>(const IntX &int2) const
{
	return OpHelper::Cmp(*this, int2, true) > 0;
} // end function operator>


//==================================================================
//   operator>=
//==================================================================


/// <summary>
/// Compares two <see cref="IntX" /> objects and returns true if first is greater or equal.
/// </summary>
/// <param name="int1">First big integer.</param>
/// <param name="int2">Second big integer.</param>
/// <returns>True if first is greater or equal.</returns>
bool IntX::operator>=(const IntX &int2) const
{
	return !(*this < int2);
} // end function operator>=

//==================================================================
//  operator<
//==================================================================


/// <summary>
/// Compares two <see cref="IntX" /> objects and returns true if first is lighter.
/// </summary>
/// <param name="int1">First big integer.</param>
/// <param name="int2">Second big integer.</param>
/// <returns>True if first is lighter.</returns>
bool IntX::operator<(const IntX &int2) const
{
	return OpHelper::Cmp(*this, int2, true) < 0;
} // end function operator<


//==================================================================
//  operator<=
//==================================================================


/// <summary>
/// Compares two <see cref="IntX" /> objects and returns true if first is lighter or equal.
/// </summary>
/// <param name="int1">First big integer.</param>
/// <param name="int2">Second big integer.</param>
/// <returns>True if first is lighter or equal.</returns>
bool IntX::operator<=(const IntX &int2) const
{
	return !(int2 < *this);
} // end function operator<=


//==================================================================
//  operator+ and operator-
//==================================================================

IntX IntX::operator+(const IntX &int2) const
{
	return OpHelper::AddSub(*this, int2, false);
} // end function operator+

IntX IntX::operator+(const int int2) const
{
	return OpHelper::AddSub(*this, int2, false);
} // end operator +

IntX IntX::operator+(const UInt32 int2) const
{
	return OpHelper::AddSub(*this, int2, false);
} // end operator +

IntX IntX::operator+(const unsigned long int2) const
{
	return OpHelper::AddSub(*this, int2, false);
} // end operator +

IntX IntX::operator+(const long long int2) const
{
	return OpHelper::AddSub(*this, int2, false);
} // end operator +

IntX IntX::operator+(const UInt64 int2) const
{
	return OpHelper::AddSub(*this, int2, false);
} // end operator +

IntX IntX::operator+(const double int2) const
{
	return OpHelper::AddSub(*this, int2, false);
} // end operator +

IntX IntX::operator+(const string &int2) const
{
	return OpHelper::AddSub(*this, int2, false);
} // end operator +
//
//
//
IntX IntX::operator+=(const IntX &int2)
{
	*this = OpHelper::AddSub(*this, int2, false);
	return *this;
} // end function operator+=

IntX IntX::operator+=(const int int2)
{
	*this = OpHelper::AddSub(*this, int2, false);
	return *this;
} // end operator +=

IntX IntX::operator+=(const UInt32 int2)
{
	*this = OpHelper::AddSub(*this, int2, false);
	return *this;
} // end operator +=

IntX IntX::operator+=(const unsigned long int2)
{
	*this = OpHelper::AddSub(*this, int2, false);
	return *this;
} // end operator +=

IntX IntX::operator+=(const long long int2)
{
	*this = OpHelper::AddSub(*this, int2, false);
	return *this;
} // end operator +=

IntX IntX::operator+=(const UInt64 int2)
{
	*this = OpHelper::AddSub(*this, int2, false);
	return *this;
} // end operator +=

IntX IntX::operator+=(const double int2)
{
	*this = OpHelper::AddSub(*this, int2, false);
	return *this;
} // end operator +=

IntX IntX::operator+=(const string &int2)
{
	*this = OpHelper::AddSub(*this, int2, false);
	return *this;
} // end operator +=
//
//
//
IntX IntX::operator-(const IntX &int2) const
{
	return OpHelper::AddSub(*this, int2, true);
} // end function operator-

IntX IntX::operator-(const int int2) const
{
	return OpHelper::AddSub(*this, int2, true);
} // end operator -

IntX IntX::operator-(const UInt32 int2) const
{
	return OpHelper::AddSub(*this, int2, true);
} // end operator -

IntX IntX::operator-(const unsigned long int2) const
{
	return OpHelper::AddSub(*this, int2, true);
} // end operator -

IntX IntX::operator-(const long long int2) const
{
	return OpHelper::AddSub(*this, int2, true);
} // end operator -

IntX IntX::operator-(const UInt64 int2) const
{
	return OpHelper::AddSub(*this, int2, true);
} // end operator -

IntX IntX::operator-(const double int2) const
{
	return OpHelper::AddSub(*this, int2, true);
} // end operator -

IntX IntX::operator-(const string &int2) const
{
	return OpHelper::AddSub(*this, int2, true);
} // end operator -
//
//
//
IntX IntX::operator-=(const IntX &int2)
{
	*this = OpHelper::AddSub(*this, int2, true);
	return *this;
} // end function operator-=

IntX IntX::operator-=(const int int2)
{
	*this = OpHelper::AddSub(*this, int2, true);
	return *this;
} // end operator -=

IntX IntX::operator-=(const UInt32 int2)
{
	*this = OpHelper::AddSub(*this, int2, true);
	return *this;
} // end operator -=

IntX IntX::operator-=(const unsigned long int2)
{
	*this = OpHelper::AddSub(*this, int2, true);
	return *this;
} // end operator -=

IntX IntX::operator-=(const long long int2)
{
	*this = OpHelper::AddSub(*this, int2, true);
	return *this;
} // end operator -=

IntX IntX::operator-=(const UInt64 int2)
{
	*this = OpHelper::AddSub(*this, int2, true);
	return *this;
} // end operator -=

IntX IntX::operator-=(const double int2)
{
	*this = OpHelper::AddSub(*this, int2, true);
	return *this;
} // end operator -=

IntX IntX::operator-=(const string &int2)
{
	*this = OpHelper::AddSub(*this, int2, true);
	return *this;
} // end operator -=

//==================================================================
//  operator* / *=
//==================================================================

IntX IntX::operator*(const IntX &int2) const
{
	return MultiplyManager::GetCurrentMultiplier()->Multiply(*this, int2);
} // end function operator*

IntX IntX::operator*(const int int2) const
{
	return MultiplyManager::GetCurrentMultiplier()->Multiply(*this, int2);
} // end operator *

IntX IntX::operator*(const UInt32 int2) const
{
	return MultiplyManager::GetCurrentMultiplier()->Multiply(*this, int2);
} // end operator *

IntX IntX::operator*(const unsigned long int2) const
{
	return MultiplyManager::GetCurrentMultiplier()->Multiply(*this, int2);
} // end operator *

IntX IntX::operator*(const long long int2) const
{
	return MultiplyManager::GetCurrentMultiplier()->Multiply(*this, int2);
} // end operator *

IntX IntX::operator*(const UInt64 int2) const
{
	return MultiplyManager::GetCurrentMultiplier()->Multiply(*this, int2);
} // end operator *

IntX IntX::operator*(const double int2) const
{
	return MultiplyManager::GetCurrentMultiplier()->Multiply(*this, int2);
} // end operator *

IntX IntX::operator*(const string &int2) const
{
	return MultiplyManager::GetCurrentMultiplier()->Multiply(*this, int2);
} // end operator *
//
//
//
IntX IntX::operator*=(const IntX &int2)
{
	*this = MultiplyManager::GetCurrentMultiplier()->Multiply(*this, int2);
	return *this;
} // end function operator*=

IntX IntX::operator*=(const int int2)
{
	*this = MultiplyManager::GetCurrentMultiplier()->Multiply(*this, int2);
	return *this;
} // end operator *=

IntX IntX::operator*=(const UInt32 int2)
{
	*this = MultiplyManager::GetCurrentMultiplier()->Multiply(*this, int2);
	return *this;
} // end operator *=

IntX IntX::operator*=(const unsigned long int2)
{
	*this = MultiplyManager::GetCurrentMultiplier()->Multiply(*this, int2);
	return *this;
} // end operator *=

IntX IntX::operator*=(const long long int2)
{
	*this = MultiplyManager::GetCurrentMultiplier()->Multiply(*this, int2);
	return *this;
} // end operator *=

IntX IntX::operator*=(const UInt64 int2)
{
	*this = MultiplyManager::GetCurrentMultiplier()->Multiply(*this, int2);
	return *this;
} // end operator *=

IntX IntX::operator*=(const double int2)
{
	*this = MultiplyManager::GetCurrentMultiplier()->Multiply(*this, int2);
	return *this;
} // end operator *=

IntX IntX::operator*=(const string &int2)
{
	*this = MultiplyManager::GetCurrentMultiplier()->Multiply(*this, int2);
	return *this;
} // end operator *=

//==================================================================
//  operator/ and operator%
//==================================================================




IntX operator/=(IntX &int1, const IntX &int2)
{
	IntX modRes;
	int1 = DivideManager::GetCurrentDivider()->DivMod(int1, int2, modRes, DivModResultFlags::dmrfDiv);
	return int1;
} // end function operator/=

IntX IntX::operator/(const IntX &int2) const
{
	IntX modRes;
	return DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfDiv);
} // end function operator/

IntX IntX::operator/(const int int2) const
{
	IntX modRes;
	return DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfDiv);
} // end operator /

IntX IntX::operator/(const UInt32 int2) const
{
	IntX modRes;
	return DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfDiv);
} // end operator /

IntX IntX::operator/(const unsigned long int2) const
{
	IntX modRes;
	return DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfDiv);
} // end operator /

IntX IntX::operator/(const long long int2) const
{
	IntX modRes;
	return DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfDiv);
} // end operator /

IntX IntX::operator/(const UInt64 int2) const
{
	IntX modRes;
	return DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfDiv);
} // end operator /

IntX IntX::operator/(const double int2) const
{
	IntX modRes;
	return DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfDiv);
} // end operator /

IntX IntX::operator/(const string &int2) const
{
	IntX modRes;
	return DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfDiv);
} // end operator /
//
//
//
IntX IntX::operator/=(const IntX &int2)
{
	IntX modRes;
	*this = DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfDiv);
	return *this;
} // end function operator/=

IntX IntX::operator/=(const int int2)
{
	IntX modRes;
	*this = DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfDiv);
	return *this;
} // end operator /=

IntX IntX::operator/=(const UInt32 int2)
{
	IntX modRes;
	*this = DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfDiv);
	return *this;
} // end operator /=

IntX IntX::operator/=(const unsigned long int2)
{
	IntX modRes;
	*this = DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfDiv);
	return *this;
} // end operator /=

IntX IntX::operator/=(const long long int2)
{
	IntX modRes;
	*this = DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfDiv);
	return *this;
} // end operator /=

IntX IntX::operator/=(const UInt64 int2)
{
	IntX modRes;
	*this = DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfDiv);
	return *this;
} // end operator /=

IntX IntX::operator/=(const double int2)
{
	IntX modRes;
	*this = DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfDiv);
	return *this;
} // end operator /=

IntX IntX::operator/=(const string &int2)
{
	IntX modRes;
	*this = DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfDiv);
	return *this;
} // end operator /=
//
//
//
IntX IntX::operator%(const IntX &int2) const
{
	IntX modRes;
	DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfMod);
	return modRes;
} // end function operator%

IntX IntX::operator%(const int int2) const
{
	IntX modRes;
	DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfMod);
	return modRes;
} // end operator %

IntX IntX::operator%(const UInt32 int2) const
{
	IntX modRes;
	DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfMod);
	return modRes;
} // end operator %

IntX IntX::operator%(const unsigned long int2) const
{
	IntX modRes;
	DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfMod);
	return modRes;
} // end operator %

IntX IntX::operator%(const long long int2) const
{
	IntX modRes;
	DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfMod);
	return modRes;
} // end operator %

IntX IntX::operator%(const UInt64 int2) const
{
	IntX modRes;
	DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfMod);
	return modRes;
} // end operator %

IntX IntX::operator%(const double int2) const
{
	IntX modRes;
	DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfMod);
	return modRes;
} // end operator %

IntX IntX::operator%(const string &int2) const
{
	IntX modRes;
	DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfMod);
	return modRes;
} // end operator %
//
//
//
IntX IntX::operator%=(const IntX &int2)
{
	IntX modRes;
	DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfMod);
	*this = modRes;
	return *this;
} // end function operator%=

IntX IntX::operator%=(const int int2)
{
	IntX modRes;
	DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfMod);
	*this = modRes;
	return *this;
} // end operator %=

IntX IntX::operator%=(const UInt32 int2)
{
	IntX modRes;
	DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfMod);
	*this = modRes;
	return *this;
} // end operator %=

IntX IntX::operator%=(const unsigned long int2)
{
	IntX modRes;
	DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfMod);
	*this = modRes;
	return *this;
} // end operator %=

IntX IntX::operator%=(const long long int2)
{
	IntX modRes;
	DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfMod);
	*this = modRes;
	return *this;
} // end operator %=

IntX IntX::operator%=(const UInt64 int2)
{
	IntX modRes;
	DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfMod);
	*this = modRes;
	return *this;
} // end operator %=

IntX IntX::operator%=(const double int2)
{
	IntX modRes;
	DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfMod);
	*this = modRes;
	return *this;
} // end operator %=

IntX IntX::operator%=(const string &int2)
{
	IntX modRes;
	DivideManager::GetCurrentDivider()->DivMod(*this, int2, modRes, DivModResultFlags::dmrfMod);
	*this = modRes;
	return *this;
} // end operator %=

//==================================================================
//  operator<< / <<= and operator>> / >>=
//==================================================================
//

/// <summary>
/// Shifts <see cref="IntX" /> object on selected bits count to the left.
/// </summary>
/// <param name="intX">Big integer.</param>
/// <param name="shift">Bits count.</param>
/// <returns>Shifting result.</returns>
IntX operator<<(const IntX &intX, const UInt32 shift)
{
	return OpHelper::Sh(intX, shift, true);
} // end function operator<<

IntX operator<<(const IntX &intX, const int shift)
{
	return OpHelper::Sh(intX, shift, true);
} // end function operator<<

IntX operator<<(const IntX &intX, const long long shift)
{
	return OpHelper::Sh(intX, shift, true);
} // end function operator<<

IntX operator<<=(IntX &int1, const UInt32 shift)
{
	int1 = int1 << shift;
	return int1;
} // end operator <<=

IntX operator<<=(IntX &int1, const int shift)
{
	int1 = int1 << shift;
	return int1;
} // end operator <<=

IntX operator<<=(IntX &int1, const long long shift)
{
	int1 = int1 << shift;
	return int1;
} // end operator <<=


/// <summary>
/// Shifts <see cref="IntX" /> object on selected bits count to the right.
/// </summary>
/// <param name="intX">Big integer.</param>
/// <param name="shift">Bits count.</param>
/// <returns>Shifting result.</returns>
IntX operator>>(const IntX &intX, const UInt32 shift)
{
	return OpHelper::Sh(intX, shift, false);
} // end function operator>>

IntX operator>>(const IntX &intX, const int shift)
{
	return OpHelper::Sh(intX, shift, false);
} // end function operator>>

IntX operator>>(const IntX &intX, const long long shift)
{
	return OpHelper::Sh(intX, shift, false);
} // end function operator>>

IntX operator>>=(IntX &int1, const UInt32 shift)
{
	int1 = int1 >> shift;
	return int1;
} // end operator >>=

IntX operator>>=(IntX &int1, const int shift)
{
	int1 = int1 >> shift;
	return int1;
} // end operator >>=

IntX operator>>=(IntX &int1, const long long shift)
{
	int1 = int1 >> shift;
	return int1;
} // end operator >>=

//==================================================================
//  +, -, ++, -- unary operators
//==================================================================


/// <summary>
/// Returns the same <see cref="IntX" /> value.
/// </summary>
/// <param name="value">Initial value.</param>
/// <returns>The same value, but new object.</returns>
/// <exception cref="ArgumentNullException"><paramref name="value" /> is a null reference.</exception>
IntX IntX::operator+()
{
	return *this;
} // end function operator+

/// <summary>
/// Returns the same <see cref="IntX" /> value, but with other sign.
/// </summary>
/// <param name="value">Initial value.</param>
/// <returns>The same value, but with other sign.</returns>
/// <exception cref="ArgumentNullException"><paramref name="value" /> is a null reference.</exception>
IntX IntX::operator-()
{
	if ((*this).length != 0)
	{
		(*this).negative = !(*this).negative;
	} // end if
	return *this;
} // end function operator-

/// <summary>
/// Returns increased <see cref="IntX" /> value.
/// </summary>
/// <param name="value">Initial value.</param>
/// <returns>Increased value.</returns>
/// <exception cref="ArgumentNullException"><paramref name="value" /> is a null reference.</exception>
const IntX IntX::operator++()
{
	*this += 1;
	return *this;
} // end function operator++

//  postfix
const IntX IntX::operator++(int)
{
	IntX temp = *this;
	*this += 1;
	return temp;
} // end function operator++

/// <summary>
/// Returns decreased <see cref="IntX" /> value.
/// </summary>
/// <param name="value">Initial value.</param>
/// <returns>Decreased value.</returns>
/// <exception cref="ArgumentNullException"><paramref name="value" /> is a null reference.</exception>
const IntX IntX::operator--()
{
	*this -= 1;
	return *this;
} // end function operator--

// postfix
const IntX IntX::operator--(int)
{
	IntX temp = *this;
	*this -= 1;
	return temp;
} // end function operator--

//==================================================================
//  Bitwise operations
//==================================================================


/// <summary>
/// Performs bitwise OR for two big integers.
/// </summary>
/// <param name="int1">First big integer.</param>
/// <param name="int2">Second big integer.</param>
/// <returns>Resulting big integer.</returns>
IntX IntX::operator|( const IntX &int2) const
{
	return OpHelper::BitwiseOr(*this, int2);
} // end function operator|

IntX IntX::operator|(const int int2) const
{
	return OpHelper::BitwiseOr(*this, int2);
} // end operator |

IntX IntX::operator|(const UInt32 int2) const
{
	return OpHelper::BitwiseOr(*this, int2);
} // end operator |

IntX IntX::operator|(const unsigned long int2) const
{
	return OpHelper::BitwiseOr(*this, int2);
} // end operator |

IntX IntX::operator|(const long long int2) const
{
	return OpHelper::BitwiseOr(*this, int2);
} // end operator |

IntX IntX::operator|(const UInt64 int2) const
{
	return OpHelper::BitwiseOr(*this, int2);
} // end operator |

IntX IntX::operator|(const double int2) const
{
	return OpHelper::BitwiseOr(*this, int2);
} // end operator |

IntX IntX::operator|(const string &int2) const
{
	return OpHelper::BitwiseOr(*this, int2);
} // end operator |
//
//
//
IntX IntX::operator|=(const IntX &int2)
{
	*this = OpHelper::BitwiseOr(*this, int2);
	return *this;
} // end function operator|=

IntX IntX::operator|=(const int int2)
{
	*this = OpHelper::BitwiseOr(*this, int2);
	return *this;
} // end operator |=

IntX IntX::operator|=(const UInt32 int2)
{
	*this = OpHelper::BitwiseOr(*this, int2);
	return *this;
} // end operator |=

IntX IntX::operator|=(const unsigned long int2)
{
	*this = OpHelper::BitwiseOr(*this, int2);
	return *this;
} // end operator |=

IntX IntX::operator|=(const long long int2)
{
	*this = OpHelper::BitwiseOr(*this, int2);
	return *this;
} // end operator |=

IntX IntX::operator|=(const UInt64 int2)
{
	*this = OpHelper::BitwiseOr(*this, int2);
	return *this;
} // end operator |=

IntX IntX::operator|=(const double int2)
{
	*this = OpHelper::BitwiseOr(*this, int2);
	return *this;
} // end operator |=

IntX IntX::operator|=(const string &int2)
{
	*this = OpHelper::BitwiseOr(*this, int2);
	return *this;
} // end operator |=
//
//
//
IntX IntX::operator&(const IntX &int2) const
{
	return OpHelper::BitwiseAnd(*this, int2);
} // end function operator&

IntX IntX::operator&(const int int2) const
{
	return OpHelper::BitwiseAnd(*this, int2);
} // end operator &

IntX IntX::operator&(const UInt32 int2) const
{
	return OpHelper::BitwiseAnd(*this, int2);
} // end operator &

IntX IntX::operator&(const unsigned long int2) const
{
	return OpHelper::BitwiseAnd(*this, int2);
} // end operator &

IntX IntX::operator&(const long long int2) const
{
	return OpHelper::BitwiseAnd(*this, int2);
} // end operator &

IntX IntX::operator&(const UInt64 int2) const
{
	return OpHelper::BitwiseAnd(*this, int2);
} // end operator &

IntX IntX::operator&(const double int2) const
{
	return OpHelper::BitwiseAnd(*this, int2);
} // end operator &

IntX IntX::operator&(const string &int2) const
{
	return OpHelper::BitwiseAnd(*this, int2);
} // end operator &
  //
  //
  //
IntX IntX::operator&=(const IntX &int2)
{
	*this = OpHelper::BitwiseAnd(*this, int2);
	return *this;
} // end function operator&=

IntX IntX::operator&=(const int int2)
{
	*this = OpHelper::BitwiseAnd(*this, int2);
	return *this;
} // end operator &=

IntX IntX::operator&=(const UInt32 int2)
{
	*this = OpHelper::BitwiseAnd(*this, int2);
	return *this;
} // end operator &=

IntX IntX::operator&=(const unsigned long int2)
{
	*this = OpHelper::BitwiseAnd(*this, int2);
	return *this;
} // end operator &=

IntX IntX::operator&=(const long long int2)
{
	*this = OpHelper::BitwiseAnd(*this, int2);
	return *this;
} // end operator &=

IntX IntX::operator&=(const UInt64 int2)
{
	*this = OpHelper::BitwiseAnd(*this, int2);
	return *this;
} // end operator &=

IntX IntX::operator&=(const double int2)
{
	*this = OpHelper::BitwiseAnd(*this, int2);
	return *this;
} // end operator &=

IntX IntX::operator&=(const string &int2)
{
	*this = OpHelper::BitwiseAnd(*this, int2);
	return *this;
} // end operator &=
//
//
//
IntX IntX::operator^(const IntX &int2) const
{
	return OpHelper::ExclusiveOr(*this, int2);
} // end function operator^

IntX IntX::operator^(const int int2) const
{
	return OpHelper::ExclusiveOr(*this, int2);
} // end operator ^

IntX IntX::operator^(const UInt32 int2) const
{
	return OpHelper::ExclusiveOr(*this, int2);
} // end operator ^

IntX IntX::operator^(const unsigned long int2) const
{
	return OpHelper::ExclusiveOr(*this, int2);
} // end operator ^

IntX IntX::operator^(const long long int2) const
{
	return OpHelper::ExclusiveOr(*this, int2);
} // end operator ^

IntX IntX::operator^(const UInt64 int2) const
{
	return OpHelper::ExclusiveOr(*this, int2);
} // end operator ^

IntX IntX::operator^(const double int2) const
{
	return OpHelper::ExclusiveOr(*this, int2);
} // end operator ^

IntX IntX::operator^(const string &int2) const
{
	return OpHelper::ExclusiveOr(*this, int2);
} // end operator ^
  //
  //
  //
IntX IntX::operator^=(const IntX &int2)
{
	*this = OpHelper::ExclusiveOr(*this, int2);
	return *this;
} // end function operator^=

IntX IntX::operator^=(const int int2)
{
	*this = OpHelper::ExclusiveOr(*this, int2);
	return *this;
} // end operator ^=

IntX IntX::operator^=(const UInt32 int2)
{
	*this = OpHelper::ExclusiveOr(*this, int2);
	return *this;
} // end operator ^=

IntX IntX::operator^=(const unsigned long int2)
{
	*this = OpHelper::ExclusiveOr(*this, int2);
	return *this;
} // end operator ^=

IntX IntX::operator^=(const long long int2)
{
	*this = OpHelper::ExclusiveOr(*this, int2);
	return *this;
} // end operator ^=

IntX IntX::operator^=(const UInt64 int2)
{
	*this = OpHelper::ExclusiveOr(*this, int2);
	return *this;
} // end operator ^=

IntX IntX::operator^=(const double int2)
{
	*this = OpHelper::ExclusiveOr(*this, int2);
	return *this;
} // end operator ^=

IntX IntX::operator^=(const string &int2)
{
	*this = OpHelper::ExclusiveOr(*this, int2);
	return *this;
} // end operator ^=

//
//
//
IntX IntX::operator~()
{
	return OpHelper::OnesComplement(*this);
} // end function operator~


//==================================================================
//  From IntX (Explicit)
//==================================================================


/// <summary>
/// Explicitly converts <see cref="IntX" /> to <see cref="int" />.
/// </summary>
/// <param name="value">Value to convert.</param>
/// <returns>Conversion result.</returns>
IntX::operator int()  const
{
	int res = (int)(UInt32)(*this);
	return this->negative ? -res : res;
} // end function operator int()

/// <summary>
/// Explicitly converts <see cref="IntX" /> to <see cref="uint" />.
/// </summary>
/// <param name="value">Value to convert.</param>
/// <returns>Conversion result.</returns>
IntX::operator UInt32() const
{
	if (this->length == 0) return 0;
	return this->digits[0];
} // end function operator unsigned int()

IntX::operator long long() const
{
	long long res = (long long)(UInt64)(*this);
	return this->negative ? -res : res;
} // end function operator long()

IntX::operator unsigned long() const
{
	unsigned long res = (unsigned long)(UInt64)(*this);
	return res;
} // end function operator unsigned long()

IntX::operator UInt64() const
{
	UInt64 res = (UInt32)(*this);
	if (this->length > 1)
	{
		res |= (UInt64)(this->digits[1]) << Constants::DigitBitCount;
	} // end if
	return res;
} // end function unsigned long()

IntX::operator unsigned short() const
{
	return (unsigned short)(UInt32)(*this);
} // end function operator unsigned short

IntX::operator string() const
{
	return ToString();
} // end function operator unsigned short

IntX::operator float() const
{
	return (float)(double)(*this);
} // end operator float

IntX::operator double() const
{
	const double infinityLength = (int)(1024 / 32);
	const double DoublePositiveInfinity = FP_INFINITE;
	const double DoubleNegativeInfinity = -FP_INFINITE;
	
	UInt64 man, h, m, l;
	int exp, sign, lLength, z;
	vector<UInt32> bits;

	bits = this->digits;
	lLength = this->length;

	if (this->IsZero()) return 0;

	if (this->IsNegative()) sign = -1;
	else sign = 1;
	
	if (lLength > infinityLength)
	{
		if (sign == 1) return DoublePositiveInfinity;
		return DoubleNegativeInfinity;
	} // end if

	h = bits[lLength - 1];

	if (lLength > 1) m = bits[lLength - 2];
	else m = 0;

	if (lLength > 2) l = bits[lLength - 3];
	else l = 0;

	// measure the exact bit count
	z = OpHelper::CbitHighZero(UInt32(h));

	exp = ((lLength - 2) * 32) - z;

	// extract most significant bits
	man = (h <<(32 + z)) | (m << z) | (l >>(32 - z));

	return OpHelper::GetDoubleFromParts(sign, exp, man);
} // end operator double


//==================================================================
//  Init utilitary methods
//==================================================================


/// <summary>
/// Initializes class instance from zero.
/// For internal use.
/// </summary>
void IntX::InitFromZero()
{
	this->length = 0;
	digits = vector<UInt32>(0);
} // end function InitFromZero

/// <summary>
/// Initializes class instance from <see cref="UInt64" /> value.
/// Doesn't initialize sign.
/// For internal use.
/// </summary>
/// <param name="value">Unsigned long value.</param>
void IntX::InitFromUlong(const UInt64 value)
{
	// Divide ulong into 2 uint values
	UInt32 low = (UInt32)value;
	UInt32 high = (UInt32)(value >> Constants::DigitBitCount);

	// Prepare internal fields
	if (high == 0)
	{
		this->length = 1;
		digits = vector<UInt32>(this->length);
		digits[0] = low;
	} // end if
	else
	{
		this->length = 2;
		digits = vector<UInt32>(this->length);
		digits[0] = low;
		digits[1] = high;
	} // end else

} // end function InitFromUlong

/// <summary>
/// Initializes class instance from another <see cref="IntX" /> value.
/// For internal use.
/// </summary>
/// <param name="value">Big integer value.</param>
void IntX::InitFromIntX(const IntX &value)
{
	this->length = value.length;
	this->digits = vector<UInt32>(value.digits);
	this->negative = value.negative;
} // end function InitFromIntX

void IntX::InitFromDigits(const vector<UInt32> &vdigits, const bool vNegative, const UInt32 vlength)
{
	length = vlength;
	digits = vector<UInt32>(vdigits);

	if (length != 0)
	{
		negative = vNegative;
	} // end if
} // end function InitFromDigits

/// <summary>
/// Initializes class instance from digits array.
/// For internal use.
/// </summary>
/// <param name="digits">Big integer digits.</param>
/// <param name="negative">Big integer sign.</param>
/// <param name="length">Big integer length.</param>
void IntX::InitFromDigits(const UInt32 *vdigits, const bool vNegative, const UInt32 vlength)
{
	length = vlength;
	digits = vector<UInt32>(vdigits, vdigits + sizeof(vdigits) / sizeof(vdigits[0]));

	if (length != 0)
	{
		negative = vNegative;
	} // end if
} // end function InitFromDigits


//==================================================================
//  Other utilitary methods
//==================================================================

// Frees extra space not used by digits only if auto-normalize is set for the instance.
void IntX::TryNormalize()
{
	if (settings.getAutoNormalize())
	{
		Normalize();
	} // end if
} // end function TryNormalize

bool IntX::ReferenceEquals(const IntX &intX1, const IntX &intX2)
{
	if(intX1.length == 0 && intX2.length == 0) { return true; }
	else if (intX1.length != intX2.length) { return false; }

	return (intX1.digits == intX2.digits &&
		intX1.length == intX2.length && intX1.negative == intX2.negative);
} // end function ReferenceEquals


//==================================================================
//  Other utilitary methods
//==================================================================

ostream &operator<<(ostream &output, const IntX &value)
{
	output << value.ToString();
	return output; // enables cout << a << b << c;
} // end function operator<<

