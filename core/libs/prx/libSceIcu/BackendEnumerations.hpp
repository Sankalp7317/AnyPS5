#pragma once
#include "prx/libSceIcu/Enumeration.hpp"
#include "prx/libc/include/general/VabiMacros.hpp"
#include <cstdint>

extern "C" {
UEnumeration* APS5_VABI ucnv_openStandardNames_nid_postfix(const char* convName, const char* standard, std::int32_t* pErrorCode);
UEnumeration* APS5_VABI ucurr_openISOCurrencies(std::uint32_t currType, std::int32_t* pErrorCode);
}
