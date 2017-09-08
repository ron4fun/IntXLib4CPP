#pragma once

#ifndef INTXSETTINGS_H
#define INTXSETTINGS_H

#include "../Utils/Enums.h"
#include "IntXGlobalSettings.h"


class IntXSettings
{
public:
	// Creates new <see cref="IntXSettings" /> instance.
	// <param name="globalSettings">IntX global settings to copy.</param>
	IntXSettings(IntXGlobalSettings &globalSettings)
	{
		// Copy local settings from global ones
		autoNormalize = globalSettings.getAutoNormalize();
		toStringMode = globalSettings.getToStringMode();
	} // end Constructor

	// To string conversion mode used in this <see cref="IntX" /> instance.
	ToStringMode getToStringMode() const
	{
		return toStringMode;
	} // end function getToStringMode

	// Set to value from <see cref="IntX.GlobalSettings" /> by default.
	void setToStringMode(ToStringMode value)
	{
		toStringMode = value;
	} // end function setToStringMode

	// If true then each operation is ended with big integer normalization.
	bool getAutoNormalize() const
	{
		return autoNormalize;
	} // end function getAutoNormalize

	// Set to value from <see cref="IntX.GlobalSettings" /> by default.
	void setAutoNormalize(bool value)
	{
		autoNormalize = value;
	} // end function setAutoNormalize

private:
	ToStringMode toStringMode = ToStringMode::tsmFast;
	bool autoNormalize = false;

}; // end class IntXSettings


#endif // !INTXSETTINGS_H

