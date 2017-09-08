#include "stdafx.h"
#include "PcgRandomMinimal.h"

bool Pcg::isInit = false;
UInt64 Pcg::state = 0;
UInt64 Pcg::inc = 0;