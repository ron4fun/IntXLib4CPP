#pragma once

#ifndef UTILS_H
#define UTILS_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include <stdexcept>
#include <string>

using namespace std;

class OverflowException : public runtime_error
{
	public:
	// constructor specifies default error message
		OverflowException(const string &text)
			: runtime_error(text.c_str()) {}
}; // end class OverflowException

class ArgumentNullException : public  runtime_error
{
public:
	// constructor specifies default error message
	ArgumentNullException(const string &text)
		: runtime_error(text.c_str()) {}
}; // end class ArgumentNullException

class FhtMultiplicationException : public runtime_error
{
public:
	// constructor specifies default error message
	FhtMultiplicationException(const string &text)
		: runtime_error(text.c_str()) {}
}; // end class FhtMultiplicationException

class ArithmeticException : public runtime_error
{
public:
	// constructor specifies default error message
	ArithmeticException(const string text)
		: runtime_error(text.c_str()) {}
}; // end class ArithmeticException

class ArgumentException : public runtime_error
{
public:
	// constructor specifies default error message
	ArgumentException(const string &text)
		: runtime_error(text.c_str()) {}
}; // end class ArgumentException

class FormatException : public runtime_error
{
public:
	// constructor specifies default error message
	FormatException(const string &text)
		: runtime_error(text.c_str()) {}
}; // end class FormatException

class ArgumentOutOfRangeException : public runtime_error
{
public:
	// constructor specifies default error message
	ArgumentOutOfRangeException(const string &text)
		: runtime_error(text.c_str()) {}
}; // end class ArgumentOutOfRangeException

class DivideByZeroException : public runtime_error
{
public:
	// constructor specifies default error message
	DivideByZeroException(const string &text)
		: runtime_error(text.c_str()) {}
}; // end class DivideByZeroException


#endif // !UTILS_H