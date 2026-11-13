#pragma once
#include "prx/libc/include/general/VabiMacros.hpp"
#include <cstdint>

extern "C" {
std::int32_t APS5_VABI ucnv_countAvailable_nid_postfix();
const char* APS5_VABI ucnv_getAvailableName_nid_postfix(std::int32_t n);
}
