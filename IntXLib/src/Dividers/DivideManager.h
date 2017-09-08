#pragma once

#ifndef DIVIDERMANAGER_H
#define DIVIDERMANAGER_H

#include "IDivider.h"
#include "ClassicDivider.h"
#include "AutoNewtonDivider.h"
#include "IntX.h"

// Used to retrieve needed divider.
class DivideManager
{

private:
	// Classic divider instance.
	static ClassicDivider _ClassicDivider;

	// Newton divider instance.
	static AutoNewtonDivider _AutoNewtonDivider;

public:
	/*
	/// <summary>
	/// Returns divider instance for given divide mode.
	/// </summary>
	/// <param name="mode">Divide mode.</param>
	/// <returns>Divider instance.</returns>
	/// <exception cref="ArgumentOutOfRangeException"><paramref name="mode" /> is out of range.</exception>
	*/
	static IDivider* GetDivider(DivideMode mode)
	{
		switch (mode)
		{
		case DivideMode::dmAutoNewton:
			return &_AutoNewtonDivider;
		case DivideMode::dmClassic:
			return &_ClassicDivider;
		default:
			throw ArgumentOutOfRangeException("mode");
		} // end switch
	} // end function GetDivider

	/*
	/// <summary>
	/// Returns current divider instance.
	/// </summary>
	/// <returns>Current divider instance.</returns>
	*/
	static IDivider* GetCurrentDivider()
	{
		return GetDivider(IntX::getGlobalSettings()->getDivideMode());
	} // end function GetCurrentDivider

}; // end class DivideManager

// Classic divider instance.
ClassicDivider DivideManager::_ClassicDivider = ClassicDivider();

// Newton divider instance.
AutoNewtonDivider DivideManager::_AutoNewtonDivider = AutoNewtonDivider(DivideManager::_ClassicDivider);

#endif // !DIVIDERMANAGER_H
