#pragma once

#ifndef INTXGLOBALSETTINGS_H
#define INTXGLOBALSETTINGS_H

#include "../Utils/Enums.h"

class IntXGlobalSettings
{
public:
	IntXGlobalSettings() {}

	MultiplyMode getMultiplyMode() const
	{
			return multiplyMode; 
	} // end function getMultiplyMode

	void setMultiplyMode(MultiplyMode value)
	{
		multiplyMode = value;
	} // end function setMultiplyMode

	DivideMode getDivideMode() const
	{
		return divideMode; 
	} // end function getDivideMode

	void setDivideMode(DivideMode value)
	{
		divideMode = value;
	} // end function getDivideMode
	
	ParseMode getParseMode() const
	{
		return parseMode;
	} // end function getParseMode

	void setParseMode(ParseMode value)
	{
		parseMode = value;
	} // end function setParseMode

	ToStringMode getToStringMode() const
	{
		return toStringMode; 
	} // end function getToStringMode

	void setToStringMode(ToStringMode value)
	{
		toStringMode = value;
	} // end function getToStringMode

	bool getAutoNormalize() const
	{
		return autoNormalize; 
	} // end function getAutoNormalize

	void setAutoNormalize(bool value)
	{
		autoNormalize = value;
	} // end function setAutoNormalize

	bool getApplyFhtValidityCheck() const
	{
		return applyFhtValidityCheck; 
	} // end function getApplyFhtValidityCheck

	void setApplyFhtValidityCheck(bool value)
	{
		applyFhtValidityCheck = value;
	} // end function setApplyFhtValidityCheck

private:
	MultiplyMode multiplyMode = MultiplyMode::mmAutoFht;
	DivideMode divideMode = DivideMode::dmAutoNewton;
	ParseMode parseMode = ParseMode::pmFast;
	ToStringMode toStringMode = ToStringMode::tsmFast;
	bool autoNormalize = false;
	bool applyFhtValidityCheck = true;

}; // end class


#endif // !INTXGLOBALSETTINGS_H

