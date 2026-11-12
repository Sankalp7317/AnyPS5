#pragma once
#include "prx/libc/include/general/VabiMacros.hpp"
#include <cstdint>

struct UResourceBundle;

extern "C" {
UResourceBundle* APS5_VABI ures_open(const char* packageName, const char* locale, std::int32_t* status);
UResourceBundle* APS5_VABI ures_openDirect_nid_postfix(const char* packageName, const char* locale, std::int32_t* status);
UResourceBundle* APS5_VABI ures_openNoDefault_nid_postfix(const char* packageName, const char* locale, std::int32_t* status);
std::int32_t APS5_VABI ures_getSize_nid_postfix(const UResourceBundle* resourceBundle);
std::int32_t APS5_VABI ures_getType_nid_postfix(const UResourceBundle* resourceBundle);
const char* APS5_VABI ures_getKey_nid_postfix(const UResourceBundle* resourceBundle);
const char16_t* APS5_VABI ures_getString_nid_postfix(const UResourceBundle* resourceBundle, std::int32_t* len, std::int32_t* status);
std::int32_t APS5_VABI ures_getInt_nid_postfix(const UResourceBundle* resourceBundle, std::int32_t* status);
const std::int32_t* APS5_VABI ures_getIntVector_nid_postfix(const UResourceBundle* resourceBundle, std::int32_t* len, std::int32_t* status);
std::int8_t APS5_VABI ures_hasNext_nid_postfix(const UResourceBundle* resourceBundle);
UResourceBundle* APS5_VABI ures_getNextResource_nid_postfix(UResourceBundle* resourceBundle, UResourceBundle* fillIn, std::int32_t* status);
const char16_t* APS5_VABI ures_getNextString_nid_postfix(UResourceBundle* resourceBundle, std::int32_t* len, const char** key, std::int32_t* status);
UResourceBundle* APS5_VABI ures_getByIndex_nid_postfix(const UResourceBundle* resourceBundle, std::int32_t indexR, UResourceBundle* fillIn, std::int32_t* status);
const char16_t* APS5_VABI ures_getStringByIndex_nid_postfix(const UResourceBundle* resourceBundle, std::int32_t indexS, std::int32_t* len, std::int32_t* status);
UResourceBundle* APS5_VABI ures_getByKey(const UResourceBundle* resourceBundle, const char* key, UResourceBundle* fillIn, std::int32_t* status);
const char16_t* APS5_VABI ures_getStringByKey(const UResourceBundle* resB, const char* key, std::int32_t* len, std::int32_t* status);
}
