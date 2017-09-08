#pragma once

#ifndef MULTIPLYMANAGER_H
#define MULTIPLYMANAGER_H

#include "IMultiplier.h"
#include "ClassicMultiplier.h"
#include "AutoFhtMultiplier.h"
#include "../IntX.h"

// Used to retrieve needed multiplier.
class MultiplyManager
{
private:
	
	// Classic multiplier instance.
	static ClassicMultiplier _ClassicMultiplier;

	// FHT multiplier instance.
	static AutoFhtMultiplier _AutoFhtMultiplier;

public:
	/*
	/// <summary>
	/// Returns multiplier instance for given multiply mode.
	/// </summary>
	/// <param name="mode">Multiply mode.</param>
	/// <returns>Multiplier instance.</returns>
	/// <exception cref="ArgumentOutOfRangeException"><paramref name="mode" /> is out of range.</exception>
	*/
	static IMultiplier* GetMultiplier(MultiplyMode mode)
	{
		switch (mode)
		{
		case MultiplyMode::mmAutoFht:
			return &_AutoFhtMultiplier;
		case MultiplyMode::mmClassic:
			return &_ClassicMultiplier;
		default:
			throw ArgumentOutOfRangeException("mode");
		} // end switch
	} // end function GetMultiplier

	/*
	/// <summary>
	/// Returns current multiplier instance.
	/// </summary>
	/// <returns>Current multiplier instance.</returns>
	*/
	static IMultiplier* GetCurrentMultiplier()
	{
		return GetMultiplier(IntX::getGlobalSettings()->getMultiplyMode());
	} // end function GetCurrentMultiplier

}; // end class MultiplyManager

#endif // !MULTIPLYMANAGER_H
