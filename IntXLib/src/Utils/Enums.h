#pragma once

#ifndef ENUMS_H
#define ENUMS_H


enum DivModResultFlags
{
	// Divident is returned.
	dmrfDiv = 1,

	// Remainder is returned.
	dmrfMod = 2
}; // end enum DivModResultFlags

enum MultiplyMode
{
	// FHT (Fast Hartley Transform) is used for really big integers.
	// Time estimate is O(n * log n).
	// Default mode.
	mmAutoFht = 1,

	// Classic method is used.
	// Time estimate is O(n ^ 2).
	mmClassic = 2
};  // end enum MultiplyMode

// Big integers divide mode used in <see cref="IntX" />.
enum DivideMode
{
	// Newton approximation algorithm is used for really big integers.
	// Time estimate is same as for multiplication.
	// Default mode.
	dmAutoNewton = 1,

	// Classic method is used.
	// Time estimate is O(n ^ 2).
	dmClassic = 2
};  // end enum DivideMode
	
// Big integers parsing mode used in <see cref="IntX" />.
enum ParseMode
{
	// Fast method which uses divide-by-two approach and fast multiply to parse numbers.
	// Time estimate is O(n * [log n]^2).
	// Default mode.
	pmFast = 1,

	// Classic method is used (using multiplication).
	// Time estimate is O(n ^ 2).
	pmClassic = 2
}; // end enum ParseMode


// Big integers to string conversion mode used in <see cref="IntX" />.
enum ToStringMode
{
	// Fast method which uses divide-by-two approach to convert numbers.
	// Default mode.
	tsmFast = 1,

	// Classic method is used (using division).
	// Time estimate is O(n ^ 2).
	tsmClassic = 2
}; // end enum ToStringMode


#endif // !ENUMS_H