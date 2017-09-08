#pragma once

#ifndef INTX_H
#define INTX_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include <string>
#include <cstring>
#include <vector>

#include "Settings/IntXGlobalSettings.h"
#include "Settings/IntXSettings.h"

#include "Utils/Enums.h"

using namespace std;

//==================================================================
//  class prototypes
//==================================================================

class DividerBase;
class DigitHelper;
class ParserBase;
class FastParser;
class StringConverterBase;
class MultiplierBase;
class FastStringConverter;


class IntX
{
	//==================================================================
	//  Friend class
	//==================================================================
	
	friend ostream &operator<<(ostream &, const IntX &);
	friend class DigitHelper;
	friend class OpHelper;
	friend class ParserBase;
	friend class MultiplierBase;
	friend class FastParser;
	friend class DividerBase;
	friend class StringConverterBase;
	friend class FastStringConverter;

public:
	//==================================================================
	//  Static fields
	//==================================================================
	
	static IntXGlobalSettings globalSettings;

public:
	//==================================================================
	//  Constructors
	//==================================================================

	// Creates new big integer with zero value.
	IntX() : digits(0), length(0), negative(false)
	{
		// Empty
	} // end default Constructor  
	
	/// <summary>
	/// Creates new big integer from integer value.
	/// </summary>
	/// <param name="value">Integer value to create big integer from.</param>
	IntX(const int value);
		  
	/// <summary>
	/// Creates new big integer from unsigned integer value.
	/// </summary>
	/// <param name="value">Unsigned integer value to create big integer from.</param>
	IntX(const UInt32 value);

	/// <summary>
	/// Creates new big integer from unsigned long value.
	/// </summary>
	/// <param name="value">Unsigned long value to create big integer from.</param>
	IntX(const unsigned long value);

	/// <summary>
	/// Creates new big integer from long value.
	/// </summary>
	/// <param name="value">Long value to create big integer from.</param>
	IntX(const long long value);
	
	/// <summary>
	/// Creates new big integer from unsigned long value.
	/// </summary>
	/// <param name="value">Unsigned long value to create big integer from.</param>
	IntX(const UInt64 value);
	
	/// <summary>
	/// Creates new big integer from float value.
	/// </summary>
	/// <param name="value">Float value to create big integer from.</param>
	IntX(const float value);

	/// <summary>
	/// Creates new big integer from double value.
	/// </summary>
	/// <param name="value">Double value to create big integer from.</param>
	IntX(const double value);

	/// <summary>
	/// Creates new <see cref="IntX" /> from char*.
	/// </summary>
	/// <param name="value">Number as char*.</param>
	IntX(const char *value);

	/// <summary>
	/// Creates new <see cref="IntX" /> from char*.
	/// </summary>
	/// <param name="value">Number as char*.</param>
	/// <param name="numberBase">Number base.</param>
	IntX(const char *value, const UInt32 numberBase);

	/// <summary>
	/// Creates new <see cref="IntX" /> from string.
	/// </summary>
	/// <param name="value">Number as string.</param>
	IntX(const string &value);
	
	/// <summary>
	/// Creates new <see cref="IntX" /> from string.
	/// </summary>
	/// <param name="value">Number as string.</param>
	/// <param name="numberBase">Number base.</param>
	IntX(const string &value, const UInt32 numberBase);

	/// <summary>
	/// Copy constructor.
	/// </summary>
	/// <param name="value">Value to copy from.</param>
	IntX(const IntX &value);

	IntX(const vector<UInt32> &digits, const bool negative)
	{
		InitFromDigits(digits, negative, digits.size());
	} // end cctor

private:
	
	/// <summary>
	/// Creates new empty big integer with desired sign and length.
	///
	/// For internal use.
	/// </summary>
	/// <param name="length">Desired digits length.</param>
	/// <param name="negative">Desired integer sign.</param>
	IntX(const UInt32 length, const bool negative);
	  
	/// <summary>
	/// Creates new big integer from array of it's "digits" but with given length.
	/// Digit with lower index has less weight.
	///
	/// For internal use.
	/// </summary>
	/// <param name="digits">Array of <see cref="IntX" /> digits.</param>
	/// <param name="negative">True if this number is negative.</param>
	/// <param name="length">Length to use for internal digits array.</param>
	/// <exception cref="ArgumentNullException"><paramref name="digits" /> is a null reference.</exception>
	IntX(const UInt32 *digits, const bool negative, const UInt32 length);
	
	IntX(const vector<UInt32> &digits, const bool negative, const UInt32 length);
		
public:
	//==================================================================
	//  Static public properties
	//==================================================================

	static IntXGlobalSettings* getGlobalSettings()
	{
		return &globalSettings;
	} // end function getGlobalSettings

	//==================================================================
	//  Public properties
	//==================================================================

	
	/// <summary>
	/// Gets flag indicating if big integer is odd.
	/// </summary>
	bool IsOdd() const 
	{
		return length > 0 && (digits[0] & 1) == 1;
	} // end function IsOdd

	IntXSettings* getSettings()
	{
		return &settings;
	} // end function getSettings

	//==================================================================
	//  Comparable implementation
	//==================================================================
	
	/// <summary>
	/// Compares current object with another big integer.
	/// </summary>
	/// <param name="n">Big integer to compare with.</param>
	/// <returns>1 if object is bigger than <paramref name="n" />, -1 if object is smaller than <paramref name="n" />, 0 if they are equal.</returns>
	int CompareTo(const IntX &n) const;
	
	/// <summary>
	/// Compares current object with another integer.
	/// </summary>
	/// <param name="n">Integer to compare with.</param>
	/// <returns>1 if object is bigger than <paramref name="n" />, -1 if object is smaller than <paramref name="n" />, 0 if they are equal.</returns>
	int CompareTo(const int n) const;

	/// <summary>
	/// Compares current object with another unsigned integer.
	/// </summary>
	/// <param name="n">Unsigned integer to compare with.</param>
	/// <returns>1 if object is bigger than <paramref name="n" />, -1 if object is smaller than <paramref name="n" />, 0 if they are equal.</returns>
	int CompareTo(const UInt32 n) const;

	/// <summary>
	/// Compares current object with another long integer.
	/// </summary>
	/// <param name="n">Long integer to compare with.</param>
	/// <returns>1 if object is bigger than <paramref name="n" />, -1 if object is smaller than <paramref name="n" />, 0 if they are equal.</returns>
	int CompareTo(const long long n) const;

	/// <summary>
	/// Compares current object with another unsigned long integer.
	/// </summary>
	/// <param name="n">Unsigned long integer to compare with.</param>
	/// <returns>1 if object is bigger than <paramref name="n" />, -1 if object is smaller than <paramref name="n" />, 0 if they are equal.</returns>
	int CompareTo(const UInt64 n) const;

	int CompareTo(const string &n) const;
	
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
	static IntX Parse(const string &value);

	/// <summary>
	/// Parses provided string representation of <see cref="IntX" /> object.
	/// </summary>
	/// <param name="value">Number as string.</param>
	/// <param name="numberBase">Number base.</param>
	/// <returns>Parsed object.</returns>
	static IntX Parse(const string &value, const UInt32 numberBase);

	/// <summary>
	/// Parses provided string representation of <see cref="IntX" /> object using custom alphabet.
	/// </summary>
	/// <param name="value">Number as string.</param>
	/// <param name="numberBase">Number base.</param>
	/// <param name="alphabet">Alphabet which contains chars used to represent big integer, char position is coresponding digit value.</param>
	/// <returns>Parsed object.</returns>
	static IntX Parse(const string &value, const UInt32 numberBase, const string &alphabet);

	/// <summary>
	/// Parses provided string representation of <see cref="IntX" /> object in decimal base.
	/// If number starts from "0" then it's treated as octal; if number starts fropm "0x"
	/// then it's treated as hexadecimal.
	/// </summary>
	/// <param name="value">Number as string.</param>
	/// <param name="mode">Parse mode.</param>
	/// <returns>Parsed object.</returns>
	static IntX Parse(const string &value, ParseMode mode);

	/// <summary>
	/// Parses provided string representation of <see cref="IntX" /> object.
	/// </summary>
	/// <param name="value">Number as string.</param>
	/// <param name="numberBase">Number base.</param>
	/// <param name="mode">Parse mode.</param>
	/// <returns>Parsed object.</returns>
	static IntX Parse(const string &value, const UInt32 numberBase, ParseMode mode);

	/// <summary>
	/// Parses provided string representation of <see cref="IntX" /> object using custom alphabet.
	/// </summary>
	/// <param name="value">Number as string.</param>
	/// <param name="numberBase">Number base.</param>
	/// <param name="alphabet">Alphabet which contains chars used to represent big integer, char position is coresponding digit value.</param>
	/// <param name="mode">Parse mode.</param>
	/// <returns>Parsed object.</returns>
	static IntX Parse(const string &value, const UInt32 numberBase, const string &alphabet, ParseMode mode);
	
	//==================================================================
	//  ToString overrides
	//==================================================================

	/// <summary>
	/// Returns decimal string representation of this <see cref="IntX" /> object.
	/// </summary>
	/// <returns>Decimal number in string.</returns>
	string ToString() const;

	/// <summary>
	/// Returns string representation of this <see cref="IntX" /> object in given base.
	/// </summary>
	/// <param name="numberBase">Base of system in which to do output.</param>
	/// <returns>Object string representation.</returns>
	string ToString(const UInt32 numberBase) const;

	/// <summary>
	/// Returns string representation of this <see cref="IntX" /> object in given base.
	/// </summary>
	/// <param name="numberBase">Base of system in which to do output.</param>
	/// <param name="upperCase">Use uppercase for bases from 11 to 16 (which use letters A-F).</param>
	/// <returns>Object string representation.</returns>
	string ToString(const UInt32 numberBase, const bool upperCase) const;

	/// <summary>
	/// Returns string representation of this <see cref="IntX" /> object in given base using custom alphabet.
	/// </summary>
	/// <param name="numberBase">Base of system in which to do output.</param>
	/// <param name="alphabet">Alphabet which contains chars used to represent big integer, char position is coresponding digit value.</param>
	/// <returns>Object string representation.</returns>
	string ToString(const UInt32 numberBase, const string &alphabet) const;

	//==================================================================
	//  Other public methods
	//==================================================================

	/// <summary>
	/// Frees extra space not used by digits.
	/// </summary>
	void Normalize();

	/// <summary>
	/// Retrieves this <see cref="IntX" /> internal state as digits array and sign.
	/// Can be used for serialization and other purposes.
	/// Note: please use constructor instead to clone <see cref="IntX" /> object.
	/// </summary>
	/// <param name="digits">Digits array.</param>
	/// <param name="negative">Is negative integer.</param>
	void GetInternalState(vector<UInt32> &digitsTo, bool &negativeTo) const;

	/// <summary>
	/// Return if <see cref="IntX" /> object is negative.
	/// </summary>
	bool IsNegative() const;

	/// <summary>
	/// Return if <see cref="IntX" /> object is zero.
	/// </summary>
	bool IsZero() const;
	
	//==================================================================
	//  Other public static methods
	//==================================================================

	/// <summary>
	/// Returns a Non-Negative Random <see cref="IntX" /> object using Pcg Random.
	/// </summary>
	/// <returns>Random IntX value.</returns>
	static IntX Random();

	/// <summary>
	/// Returns a Non-Negative Random <see cref="IntX" /> object using Pcg Random within the specified Range. (Max not Included)
	/// </summary>
	/// <param name="Min">Minimum value.</param>
	/// <param name="Max">Maximum value (Max not Included)</param>
	/// <returns>Random IntX value.</returns>
	static IntX RandomRange(const UInt32 Min, const UInt32 Max);

	/// <summary>
	/// Returns a non negative big integer.
	/// </summary>
	/// <param name="value">value to get its absolute value.</param>
	/// <returns>Absolute number.</returns>
	static IntX AbsoluteValue(const IntX &value);

	/// <summary>
	/// Returns a specified big integer raised to the power of 2.
	/// </summary>
	/// <param name="value">Number to get its square.</param>
	/// <returns>Squared number.</returns>
	static IntX Square(const IntX &value);

	/// <summary>
	/// Calculates Integer SquareRoot of <see cref="IntX" /> object
	/// </summary>
	/// <param name="value">value to get Integer squareroot of.</param>
	/// <returns>Integer SquareRoot.</returns>
	/// <seealso href="http://www.dahuatu.com/RkWdPBx6W8.html">[IntegerSquareRoot Implementation]</seealso>
	static IntX IntegerSquareRoot(const IntX &value);

	/// <summary>
	/// Returns a specified big integer holding the factorial of value.
	/// </summary>
	/// <param name="value">Number to get its factorial.</param>
	/// <returns>factorialed number.</returns>
	/// <exception cref="EArgumentException"><paramref name="value" /> is a negative value.</exception>
	static IntX Factorial(const IntX &value);

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
	static IntX GCD(const IntX &int1, const IntX &int2);

	// HCF is thesame as GCD
	static IntX HCF(const IntX &int1, const IntX &int2)
	{
		return GCD(int1, int2);
	} // end function HCF

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
	static IntX LCM(const IntX &int1, const IntX &int2);
	
	/// <summary>
	/// Calculate Modular Inverse for two <see cref="IntX" /> objects using Euclids Extended Algorithm.
	/// returns Zero if no Modular Inverse Exists for the Inputs
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <returns>Modular Inverse.</returns>
	/// <seealso href="https://en.wikipedia.org/wiki/Modular_multiplicative_inverse">[Modular Inverse Explanation]</seealso>
	/// <seealso href="http://www.di-mgt.com.au/euclidean.html">[Modular Inverse Implementation]</seealso>
	static IntX InvMod(const IntX &int1, const IntX &int2);

	/// <summary>
	/// Calculates Calculates Modular Exponentiation of <see cref="IntX" /> object.
	/// </summary>
	/// <param name="value">value to compute ModPow of.</param>
	/// <param name="exponent">exponent to use.</param>
	/// <param name="modulus">modulus to use.</param>
	/// <returns>Computed value.</returns>
	/// <seealso href="https://en.wikipedia.org/wiki/Modular_exponentiation">[Modular Exponentiation Explanation]</seealso>
	static IntX ModPow(const IntX &value, const IntX &exponent, const IntX &modulus);

	/// <summary>
	/// Calculates Bézoutsidentity for two <see cref="IntX" /> objects using Euclids Extended Algorithm
	/// </summary>
	/// <param name="int1">first value.</param>
	/// <param name="int2">second value.</param>
	/// <param name="bezOne">first bezout value.</param>
	/// <param name="bezTwo">second bezout value.</param>
	/// <returns>GCD (Greatest Common Divisor) value.</returns>
	/// <seealso href="https://en.wikipedia.org/wiki/Bézout's_identity">[Bézout's identity Explanation]</seealso>
	/// <seealso href="https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm#Pseudocode">[Bézout's identity Pseudocode using Extended Euclidean algorithm]</seealso>
	static IntX Bezoutsidentity(const IntX &int1, const IntX &int2, IntX &bezOne, IntX &bezTwo);

	/// <summary>
	/// Checks if a <see cref="IntX" /> object is Probably Prime using Miller–Rabin primality test.
	/// </summary>
	/// <param name="value">big integer to check primality.</param>
	/// <param name="Accuracy">Accuracy parameter `k´ of the Miller-Rabin algorithm. Default is 5. The execution time is proportional to the value of the accuracy parameter.</param>
	/// <returns>Boolean value.</returns>
	/// <seealso href="https://en.wikipedia.org/wiki/Miller–Rabin_primality_test">[Miller–Rabin primality test Explanation]</seealso>
	/// <seealso href="https://github.com/cslarsen/miller-rabin">[Miller–Rabin primality test Implementation in C]</seealso>
	static bool IsProbablyPrime(const IntX &value, const int Accuracy = 5);

	/// <summary>
	/// The Max Between Two IntX values.
	/// </summary>
	/// <param name="left">left value.</param>
	/// <param name="right">right value.</param>
	/// <returns>The Maximum IntX value.</returns>
	static IntX Max(const IntX &left, const IntX &right);

	/// <summary>
	/// The Min Between Two IntX values.
	/// </summary>
	/// <param name="left">left value.</param>
	/// <param name="right">right value.</param>
	/// <returns>The Minimum IntX value.</returns>
	static IntX Min(const IntX &left, const IntX &right);

	/// <summary>
	/// The base-10 logarithm of the value.
	/// </summary>
	/// <param name="value">The value.</param>
	/// <returns>The base-10 logarithm of the value.</returns>
	/// <remarks> Source : Microsoft .NET Reference on GitHub </remarks>
	static double Log10(const IntX &value);

	/// <summary>
	/// Calculates the natural logarithm of the value.
	/// </summary>
	/// <param name="value">The value.</param>
	/// <returns>The natural logarithm.</returns>
	/// <remarks> Source : Microsoft .NET Reference on GitHub </remarks>
	static double Ln(const IntX &value);

	/// <summary>
	/// Calculates Logarithm of a number <see cref="IntX" /> object for a specified base.
	/// the largest power the base can be raised to that does not exceed the number.
	/// </summary>
	/// <param name="base">base.</param>
	/// <param name="value">number to get log of.</param>
	/// <returns>Log value.</returns>
	/// <remarks> Source : Microsoft .NET Reference on GitHub </remarks>
	static double LogN(const double base, const IntX &value);

	/// <summary>
	/// Calculates Integer Logarithm of a number <see cref="IntX" /> object for a specified base.
	/// the largest power the base can be raised to that does not exceed the number.
	/// </summary>
	/// <param name="base">base.</param>
	/// <param name="number">number to get Integer log of.</param>
	/// <returns>Integer Log.</returns>
	/// <seealso href="http://gist.github.com/dharmatech/409723">[IntegerLogN Implementation]</seealso>
	static IntX IntegerLogN(const IntX &base, const IntX &number);


	//==================================================================
	//  IEquatable/Equals/GetHashCode implementation/overrides
	//==================================================================

	/// <summary>
	/// Returns equality of this <see cref="IntX" /> with another big integer.
	/// </summary>
	/// <param name="n">Big integer to compare with.</param>
	/// <returns>True if equals.</returns>
	bool Equals(const IntX &n) const;

	/// <summary>
	/// Returns hash code for this <see cref="IntX" /> object.
	/// </summary>
	/// <returns>Object hash code.</returns>
	int GetHashCode() const;

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
	static IntX Multiply(const IntX &int1, const IntX &int2, MultiplyMode mode);

	/// <summary>
	/// Divides one <see cref="IntX" /> object by another.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <param name="mode">Divide mode.</param>
	/// <returns>Division result.</returns>
	static IntX Divide(const IntX &int1, const IntX &int2, DivideMode mode);

	/// <summary>
	/// Divides one <see cref="IntX" /> object by another and returns division modulo.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <param name="mode">Divide mode.</param>
	/// <returns>Modulo result.</returns>
	static IntX Modulo(const IntX &int1, const IntX &int2, DivideMode mode);

	/// <summary>
	/// Divides one <see cref="IntX" /> object on another.
	/// Returns both divident and remainder
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <param name="modRes">Remainder big integer.</param>
	/// <returns>Division result.</returns>
	static IntX DivideModulo(const IntX &int1, const IntX &int2, IntX &modRes);

	/// <summary>
	/// Divides one <see cref="IntX" /> object on another.
	/// Returns both divident and remainder
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <param name="modRes">Remainder big integer.</param>
	/// <param name="mode">Divide mode.</param>
	/// <returns>Division result.</returns>
	static IntX DivideModulo(const IntX &int1, const IntX &int2, IntX &modRes, DivideMode mode);
	
	/// <summary>
	/// Returns a specified big integer raised to the specified power.
	/// </summary>
	/// <param name="value">Number to raise.</param>
	/// <param name="power">Power.</param>
	/// <returns>Number in given power.</returns>
	static IntX Pow(const IntX &value, const UInt32 power);

	/// <summary>
	/// Returns a specified big integer raised to the specified power.
	/// </summary>
	/// <param name="value">Number to raise.</param>
	/// <param name="power">Power.</param>
	/// <param name="multiplyMode">Multiply mode set explicitly.</param>
	/// <returns>Number in given power.</returns>
	static IntX Pow(const IntX &value, const UInt32 power, MultiplyMode mode);

	//==================================================================
	//  Operators
	//==================================================================

	//==================================================================
	//  Assignment operator=
	//==================================================================
	
	// assignment operator
	const IntX &operator =(const IntX &value);
	
	//==================================================================
	//  operator==
	//==================================================================

	/// <summary>
	/// Compares two <see cref="IntX" /> objects and returns true if their internal state is equal.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <returns>True if equals.</returns>
	bool operator==(const IntX &int2) const;

	template<typename T1>
	friend bool operator==(const IntX &int1, const T1 int2)
	{
		return int1 == IntX(int2);
	} // end function operator==

	template<typename T1>
	friend bool operator==(const T1 int1, const IntX &int2)
	{
		return IntX(int1) == int2;
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
	bool operator!=(const IntX &int2) const;

	template<typename T1>
	friend bool operator!=(const IntX &int1, const T1 int2)
	{
		return int1 != IntX(int2);
	} // end function operator!=

	template<typename T1>
	friend bool operator!=(const T1 int1, const IntX &int2)
	{
		return IntX(int1) != int2;
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
	bool operator>(const IntX &int2) const;
	
	template<typename T1>
	friend bool operator>(const IntX &int1, const T1 int2)
	{
		return int1 > IntX(int2);
	} // end function operator>

	template<typename T1>
	friend bool operator>(const T1 int1, const IntX &int2)
	{
		return IntX(int1) > int2;
	} // end function operator>

	//==================================================================
	//  operator>=
	//==================================================================

	
	/// <summary>
	/// Compares two <see cref="IntX" /> objects and returns true if first is greater or equal.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <returns>True if first is greater or equal.</returns>
	bool operator>=(const IntX &int2) const;
	
	template<typename T1>
	friend bool operator>=(const IntX &int1, const T1 int2)
	{
		return int1 >= IntX(int2);
	} // end function operator>=

	template<typename T1>
	friend bool operator>=(const T1 int1, const IntX &int2)
	{
		return IntX(int1) >= int2;
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
	bool operator<(const IntX &int2) const;

	template<typename T1>
	friend bool operator<(const IntX &int1, const T1 int2)
	{
		return int1 < IntX(int2);
	} // end function operator<

	template<typename T1>
	friend bool operator<(const T1 int1, const IntX &int2)
	{
		return IntX(int1) < int2;
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
	bool operator<=(const IntX &int2) const;
	
	template<typename T1>
	friend bool operator<=(const IntX &int1, const T1 int2)
	{
		return int1 <= IntX(int2);
	} // end function operator<=

	template<typename T1>
	friend bool operator<=(const T1 int1, const IntX &int2)
	{
		return IntX(int1) <= int2;
	} // end function operator<=

	//==================================================================
	//  operator+ / += and operator- / -=
	//==================================================================

	
	/// <summary>
	/// Adds one <see cref="IntX" /> object to another.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <returns>Addition result.</returns>
	IntX operator+(const IntX &int2) const;
	IntX operator+(const int int2) const;
	IntX operator+(const UInt32 int2) const;
	IntX operator+(const unsigned long int2) const;
	IntX operator+(const long long int2) const;
	IntX operator+(const UInt64 int2) const;
	IntX operator+(const double int2) const;
	IntX operator+(const string &int2) const;

	IntX operator+=(const IntX &int2);
	IntX operator+=(const int int2);
	IntX operator+=(const UInt32 int2);
	IntX operator+=(const unsigned long int2);
	IntX operator+=(const long long int2);
	IntX operator+=(const UInt64 int2);
	IntX operator+=(const double int2);
	IntX operator+=(const string &int2);

	/// <summary>
	/// Subtracts one <see cref="IntX" /> object from another.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <returns>Subtraction result.</returns>
	IntX operator-(const IntX &int2) const;
	IntX operator-(const int int2) const;
	IntX operator-(const UInt32 int2) const;
	IntX operator-(const unsigned long int2) const;
	IntX operator-(const long long int2) const;
	IntX operator-(const UInt64 int2) const;
	IntX operator-(const double int2) const;
	IntX operator-(const string &int2) const;

	IntX operator-=(const IntX &int2);
	IntX operator-=(const int int2);
	IntX operator-=(const UInt32 int2);
	IntX operator-=(const unsigned long int2);
	IntX operator-=(const long long int2);
	IntX operator-=(const UInt64 int2);
	IntX operator-=(const double int2);
	IntX operator-=(const string &int2);		
	
	//==================================================================
	//  operator* / *=
	//==================================================================

	
	/// <summary>
	/// Multiplies one <see cref="IntX" /> object on another.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <returns>Multiply result.</returns>
	IntX operator*(const IntX &int2) const;
	IntX operator*(const int int2) const;
	IntX operator*(const UInt32 int2) const;
	IntX operator*(const unsigned long int2) const;
	IntX operator*(const long long int2) const;
	IntX operator*(const UInt64 int2) const;
	IntX operator*(const double int2) const;
	IntX operator*(const string &int2) const;

	IntX operator*=(const IntX &int2);
	IntX operator*=(const int int2);
	IntX operator*=(const UInt32 int2);
	IntX operator*=(const unsigned long int2);
	IntX operator*=(const long long int2);
	IntX operator*=(const UInt64 int2);
	IntX operator*=(const double int2);
	IntX operator*=(const string &int2);
	
	//==================================================================
	//  operator/ and /= and operator% / %=
	//==================================================================


	/// <summary>
	/// Divides one <see cref="IntX" /> object by another.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <returns>Division result.</returns>
	IntX operator/(const IntX &int2) const;
	IntX operator/(const int int2) const;
	IntX operator/(const UInt32 int2) const;
	IntX operator/(const unsigned long int2) const;
	IntX operator/(const long long int2) const;
	IntX operator/(const UInt64 int2) const;
	IntX operator/(const double int2) const;
	IntX operator/(const string &int2) const;

	IntX operator/=(const IntX &int2);
	IntX operator/=(const int int2);
	IntX operator/=(const UInt32 int2);
	IntX operator/=(const unsigned long int2);
	IntX operator/=(const long long int2);
	IntX operator/=(const UInt64 int2);
	IntX operator/=(const double int2);
	IntX operator/=(const string &int2);
	//
	//
	//
	/// <summary>
	/// Divides one <see cref="IntX" /> object by another and returns division modulo.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <returns>Modulo result.</returns>
	//
	IntX operator%(const IntX &int2) const;
	IntX operator%(const int int2) const;
	IntX operator%(const UInt32 int2) const;
	IntX operator%(const unsigned long int2) const;
	IntX operator%(const long long int2) const;
	IntX operator%(const UInt64 int2) const;
	IntX operator%(const double int2) const;
	IntX operator%(const string &int2) const;

	IntX operator%=(const IntX &int2);
	IntX operator%=(const int int2);
	IntX operator%=(const UInt32 int2);
	IntX operator%=(const unsigned long int2);
	IntX operator%=(const long long int2);
	IntX operator%=(const UInt64 int2);
	IntX operator%=(const double int2);
	IntX operator%=(const string &int2);

	//==================================================================
	//  operator<< / <<= and operator>> / >>=
	//==================================================================

	
	/// <summary>
	/// Shifts <see cref="IntX" /> object on selected bits count to the left.
	/// </summary>
	/// <param name="intX">Big integer.</param>
	/// <param name="shift">Bits count.</param>
	/// <returns>Shifting result.</returns>
	friend IntX operator<<(const IntX &intX, const UInt32 shift);
	friend IntX operator<<(const IntX &intX, const int shift);
	friend IntX operator<<(const IntX &intX, const long long shift);

	friend IntX operator<<=(IntX &int1, const UInt32 int2);
	friend IntX operator<<=(IntX &int1, const int int2);
	friend IntX operator<<=(IntX &int1, const long long int2);

	/// <summary>
	/// Shifts <see cref="IntX" /> object on selected bits count to the right.
	/// </summary>
	/// <param name="intX">Big integer.</param>
	/// <param name="shift">Bits count.</param>
	/// <returns>Shifting result.</returns>
	friend IntX operator>>(const IntX &intX, const UInt32 shift);
	friend IntX operator>>(const IntX &intX, const int shift);
	friend IntX operator>>(const IntX &intX, const long long shift);

	friend IntX operator>>=(IntX &int1, const UInt32 int2);
	friend IntX operator>>=(IntX &int1, const int int2);
	friend IntX operator>>=(IntX &int1, const long long int2);

	//==================================================================
	//  +, -, ++, -- unary operators
	//==================================================================

	
	/// <summary>
	/// Returns the same <see cref="IntX" /> value.
	/// </summary>
	/// <param name="value">Initial value.</param>
	/// <returns>The same value, but new object.</returns>
	/// <exception cref="ArgumentNullException"><paramref name="value" /> is a null reference.</exception>
	IntX operator+();

	/// <summary>
	/// Returns the same <see cref="IntX" /> value, but with other sign.
	/// </summary>
	/// <param name="value">Initial value.</param>
	/// <returns>The same value, but with other sign.</returns>
	/// <exception cref="ArgumentNullException"><paramref name="value" /> is a null reference.</exception>
	IntX operator-();

	/// <summary>
	/// Returns increased <see cref="IntX" /> value.
	/// </summary>
	/// <param name="value">Initial value.</param>
	/// <returns>Increased value.</returns>
	/// <exception cref="ArgumentNullException"><paramref name="value" /> is a null reference.</exception>
	const IntX operator++(); 

	/// <summary>
	/// Returns increased <see cref="IntX" /> value.
	/// </summary>
	/// <param name="value">Initial value.</param>
	/// <returns>Increased value.</returns>
	/// <exception cref="ArgumentNullException"><paramref name="value" /> is a null reference.</exception>
	const IntX operator++(int);

	/// <summary>
	/// Returns decreased <see cref="IntX" /> value.
	/// </summary>
	/// <param name="value">Initial value.</param>
	/// <returns>Decreased value.</returns>
	/// <exception cref="ArgumentNullException"><paramref name="value" /> is a null reference.</exception>
	const IntX operator--();

	/// <summary>
	/// Returns decreased <see cref="IntX" /> value.
	/// </summary>
	/// <param name="value">Initial value.</param>
	/// <returns>Decreased value.</returns>
	/// <exception cref="ArgumentNullException"><paramref name="value" /> is a null reference.</exception>
	const IntX operator--(int);

	//==================================================================
	//  Bitwise operations
	//==================================================================	
	//
	/// <summary>
	/// Performs bitwise OR for two big integers.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <returns>Resulting big integer.</returns>
	IntX operator|(const IntX &int2) const;
	IntX operator|(const int int2) const;
	IntX operator|(const UInt32 int2) const;
	IntX operator|(const unsigned long int2) const;
	IntX operator|(const long long int2) const;
	IntX operator|(const UInt64 int2) const;
	IntX operator|(const double int2) const;
	IntX operator|(const string &int2) const;

	IntX operator|=(const IntX &int2);
	IntX operator|=(const int int2);
	IntX operator|=(const UInt32 int2);
	IntX operator|=(const unsigned long int2);
	IntX operator|=(const long long int2);
	IntX operator|=(const UInt64 int2);
	IntX operator|=(const double int2);
	IntX operator|=(const string &int2);

	/// <summary>
	/// Performs bitwise AND for two big integers.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <returns>Resulting big integer.</returns>
	IntX operator&(const IntX &int2) const;
	IntX operator&(const int int2) const;
	IntX operator&(const UInt32 int2) const;
	IntX operator&(const unsigned long int2) const;
	IntX operator&(const long long int2) const;
	IntX operator&(const UInt64 int2) const;
	IntX operator&(const double int2) const;
	IntX operator&(const string &int2) const;

	IntX operator&=(const IntX &int2);
	IntX operator&=(const int int2);
	IntX operator&=(const UInt32 int2);
	IntX operator&=(const unsigned long int2);
	IntX operator&=(const long long int2);
	IntX operator&=(const UInt64 int2);
	IntX operator&=(const double int2);
	IntX operator&=(const string &int2);

	/// <summary>
	/// Performs bitwise XOR for two big integers.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <returns>Resulting big integer.</returns>
	IntX operator^(const IntX &int2) const;
	IntX operator^(const int int2) const;
	IntX operator^(const UInt32 int2) const;
	IntX operator^(const unsigned long int2) const;
	IntX operator^(const long long int2) const;
	IntX operator^(const UInt64 int2) const;
	IntX operator^(const double int2) const;
	IntX operator^(const string &int2) const;

	IntX operator^=(const IntX &int2);
	IntX operator^=(const int int2);
	IntX operator^=(const UInt32 int2);
	IntX operator^=(const unsigned long int2);
	IntX operator^=(const long long int2);
	IntX operator^=(const UInt64 int2);
	IntX operator^=(const double int2);
	IntX operator^=(const string &int2);

	/// <summary>
	/// Performs bitwise NOT for big integer.
	/// </summary>
	/// <param name="value">Big integer.</param>
	/// <returns>Resulting big integer.</returns>
	IntX operator~();

	//==================================================================
	// From IntX (Explicit)
	//==================================================================	

	
	/// <summary>
	/// Explicitly converts <see cref="IntX" /> to <see cref="int" />.
	/// </summary>
	/// <param name="value">Value to convert.</param>
	/// <returns>Conversion result.</returns>
	explicit operator int()  const;

	/// <summary>
	/// Explicitly converts <see cref="IntX" /> to <see cref="uint" />.
	/// </summary>
	/// <param name="value">Value to convert.</param>
	/// <returns>Conversion result.</returns>
	explicit operator UInt32() const;

	/// <summary>
	/// Explicitly converts <see cref="IntX" /> to <see cref="long" />.
	/// </summary>
	/// <param name="value">Value to convert.</param>
	/// <returns>Conversion result.</returns>
	explicit operator long long() const;

	/// <summary>
	/// Explicitly converts <see cref="IntX" /> to <see cref="unsigned long" />.
	/// </summary>
	/// <param name="value">Value to convert.</param>
	/// <returns>Conversion result.</returns>
	explicit operator unsigned long() const;

	/// <summary>
	/// Explicitly converts <see cref="IntX" /> to <see cref="ulong" />.
	/// </summary>
	/// <param name="value">Value to convert.</param>
	/// <returns>Conversion result.</returns>
	explicit operator UInt64() const;

	/// <summary>
	/// Explicitly converts <see cref="IntX" /> to <see cref="float" />.
	/// </summary>
	/// <param name="value">Value to convert.</param>
	/// <returns>Conversion result.</returns>
	explicit operator float() const;

	/// <summary>
	/// Explicitly converts <see cref="IntX" /> to <see cref="double" />.
	/// </summary>
	/// <param name="value">Value to convert.</param>
	/// <returns>Conversion result.</returns>
	explicit operator double() const;

	/// <summary>
	/// Explicitly converts <see cref="IntX" /> to <see cref="ushort" />.
	/// </summary>
	/// <param name="value">Value to convert.</param>
	/// <returns>Conversion result.</returns>
	explicit operator unsigned short() const;

	/// <summary>
	/// Explicitly converts <see cref="IntX" /> to <see cref="string" />.
	/// </summary>
	/// <param name="value">Value to convert.</param>
	/// <returns>Conversion result.</returns>
	explicit operator string() const;
	

private:
	//==================================================================
	//  Init utilitary methods
	//==================================================================	

	
	/// <summary>
	/// Initializes class instance from zero.
	/// For internal use.
	/// </summary>
	void InitFromZero();

	/// <summary>
	/// Initializes class instance from <see cref="UInt64" /> value.
	/// Doesn't initialize sign.
	/// For internal use.
	/// </summary>
	/// <param name="value">Unsigned long value.</param>
	void InitFromUlong(const UInt64 value);

	/// <summary>
	/// Initializes class instance from another <see cref="IntX" /> value.
	/// For internal use.
	/// </summary>
	/// <param name="value">Big integer value.</param>
	void InitFromIntX(const IntX &value);

	void InitFromDigits(const vector<UInt32> &digits, const bool negative, const UInt32 length);

	/// <summary>
	/// Initializes class instance from digits array.
	/// For internal use.
	/// </summary>
	/// <param name="digits">Big integer digits.</param>
	/// <param name="negative">Big integer sign.</param>
	/// <param name="length">Big integer length.</param>
	void InitFromDigits(const UInt32 *digits, const bool negative, const UInt32 length);

	//==================================================================
	//  Other utilitary methods
	//==================================================================
		
	// Frees extra space not used by digits only if auto-normalize is set for the instance.
	void TryNormalize();

	static bool ReferenceEquals(const IntX &intX1, const IntX &intX2);

	//==================================================================
	//  Internal fields
	//==================================================================

	vector<UInt32> digits; // big integer digits
	UInt32 length = 0; // big integer digits length
	bool negative = false; // big integer sign ("-" if true)

	IntXSettings settings = IntXSettings(globalSettings);

	
}; // end class IntX


#endif // !INTX_H