#pragma once
#include "prx/libc/include/general/VabiMacros.hpp"
#include <cstdint>

extern "C" {
std::int32_t APS5_VABI uloc_countAvailable();
const char* APS5_VABI uloc_getAvailable(std::int32_t n);
}
