#pragma once
#include "prx/libc/include/general/VabiMacros.hpp"
#include <cstdint>

extern "C" {
std::int32_t APS5_VABI ucurr_forLocale_nid_postfix(const char* locale, char16_t* buff, std::int32_t buffCapacity, std::int32_t* ec);
std::int32_t APS5_VABI ucurr_getDefaultFractionDigits_nid_postfix(const char16_t* currency, std::int32_t* ec);
std::int32_t APS5_VABI ucurr_getDefaultFractionDigitsForUsage_nid_postfix(const char16_t* currency, std::int32_t usage, std::int32_t* ec);
double APS5_VABI ucurr_getRoundingIncrement_nid_postfix(const char16_t* currency, std::int32_t* ec);
double APS5_VABI ucurr_getRoundingIncrementForUsage_nid_postfix(const char16_t* currency, std::int32_t usage, std::int32_t* ec);
std::int32_t APS5_VABI ucurr_getNumericCode_nid_postfix(const char16_t* currency);
}
