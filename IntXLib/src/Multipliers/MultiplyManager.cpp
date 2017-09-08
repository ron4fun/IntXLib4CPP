#include "stdafx.h"
#include "MultiplyManager.h"

// Classic multiplier instance.
ClassicMultiplier MultiplyManager::_ClassicMultiplier = ClassicMultiplier();

// FHT multiplier instance.
AutoFhtMultiplier MultiplyManager::_AutoFhtMultiplier = AutoFhtMultiplier(MultiplyManager::_ClassicMultiplier);