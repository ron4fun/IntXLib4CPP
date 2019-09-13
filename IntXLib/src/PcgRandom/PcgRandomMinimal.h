#pragma once

#ifndef PCGRANDOMMINIMAL_H
#define PCGRANDOMMINIMAL_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include <chrono>
#include <cstdlib>

using namespace std;

/// <summary>
/// The Pcg Random Number Class.
/// </summary>
class Pcg
{
private:
	/// <summary>
	/// The RNG state. All values are possible.
	/// </summary>
	static UInt64 state;

	/// <summary>
	/// Controls which RNG sequence (stream) is selected.
	/// Must <strong>always</strong> be odd.
	/// </summary>
	static UInt64 inc;

	// Checker to ensure Init was run at least Once
	static bool isInit;

	/// <summary>
	/// static class constructor.
	/// Initializes a new instance of the <see cref="TPcg"/> class
	/// <strong>FOR USAGE</strong> with <strong>SYSTEM TIME</strong> as initState.
	/// </summary>
	static void Init()
	{
		UInt64 LinitState, LinitSeq;

		LinitState = GetInitState(LinitSeq);

		// ==> initializes using system time as initState and calculated value as
		// initSeq
		Seed(LinitState, LinitSeq);
	} // end function Init

	static UInt64 GetInitState(UInt64 &initSeq)
	{
		UInt64 result = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

		initSeq = GetInitSeq(result) * (long long)(1000000);

		return result;
	} // end function GetInitState

	static UInt64 GetInitSeq(const UInt64 tempVal)
	{
		return tempVal * 181783497276652981;
	} // end function GetInitSeq

	static void Seed(const UInt64 &initState, const UInt64 initSeq)
	{
		Pcg::state = UInt32(0);
		Pcg::inc = (initSeq << 1) | UInt64(1);
		NextUInt32();
		Pcg::state += initState;
		NextUInt32();
	} // end function Seed

public:
	static UInt32 Range32(const UInt32 exclusiveBound)
	{
		if (!Pcg::isInit)
		{
			Pcg::isInit = true;
			Init();
		} // end if

		UInt32 r, threshold;

		// To avoid bias, we need to make the range of the RNG
		// a multiple of bound, which we do by dropping output
		// less than a threshold. A naive scheme to calculate the
		// threshold would be to do
		//
		// threshold = UInt64($100000000) mod exclusiveBound;
		//
		// but 64-bit div/mod is slower than 32-bit div/mod
		// (especially on 32-bit platforms). In essence, we do
		//
		// threshold := UInt32((UInt64($100000000) - exclusiveBound) mod exclusiveBound);
		//
		// because this version will calculate the same modulus,
		// but the LHS value is less than 2^32.
		threshold = UInt32((UInt64(0x100000000) - exclusiveBound) % exclusiveBound);

		// Uniformity guarantees that this loop will terminate.
		// In practice, it should terminate quickly; on average
		// (assuming all bounds are equally likely), 82.25% of
		// the time, we can expect it to require just one
		// iteration. In the worst case, someone passes a bound
		// of 2^31 + 1 (i.e., 2147483649), which invalidates
		// almost 50% of the range. In practice bounds are
		// typically small and only a tiny amount of the range
		// is eliminated.
		while (true)
		{
			r = NextUInt32();
			if (r >= threshold)
			{
				return r % exclusiveBound;
			} // end if
		} // end while
		return 0; // to make FixInsight Happy :)
	} // end function Range32

	static int NextInt(const int minimum, const int exclusiveBound)
	{
		UInt32 boundRange, rangeResult;
		
		boundRange = UInt32(exclusiveBound - minimum);
		rangeResult = Range32(boundRange);
		
		return (int)rangeResult + (int)minimum;
	} // end function NextInt

	static UInt32 NextUInt32()
	{
		if (!Pcg::isInit)
		{
			Pcg::isInit = true;
			Init();
		} // end if

		UInt64 oldState;
		UInt32 xorShifted;
		int rot;
		
		oldState = Pcg::state;
		Pcg::state = oldState * UInt64(6364136223846793005) + Pcg::inc;
		xorShifted = UInt32(((oldState >> 18) ^ oldState) >> 27);
		rot = (int)(oldState >> 59);
	
		return (xorShifted >> rot) | (xorShifted <<((-rot) & 31));
	} // end function NextUInt32

	static UInt32 NextUInt32(const UInt32 minimum, const UInt32 exclusiveBound)
	{
		UInt32 boundRange, rangeResult;
		
		boundRange = exclusiveBound - minimum;
		rangeResult = Range32(boundRange);
		
		return rangeResult + minimum;
	} // end function NextUInt32

}; // end class Pcg


#endif // !PCGRANDOMMINIMAL_H
