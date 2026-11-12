#pragma once
#include "prx/libc/include/general/VabiMacros.hpp"
#include <cstdint>

extern "C" {
const char* APS5_VABI uloc_toUnicodeLocaleKey_nid_postfix(const char* keyword);
const char* APS5_VABI uloc_toUnicodeLocaleType(const char* keyword, const char* value);
const char* APS5_VABI uloc_toLegacyType_nid_postfix(const char* keyword, const char* value);
std::int32_t APS5_VABI uloc_addLikelySubtags(const char* localeID, char* maximizedLocaleID, std::int32_t maximizedLocaleIDCapacity, std::int32_t* err);
std::int32_t APS5_VABI uloc_minimizeSubtags(const char* localeID, char* minimizedLocaleID, std::int32_t minimizedLocaleIDCapacity, std::int32_t* err);
std::int32_t APS5_VABI uloc_toLanguageTag(const char* localeID, char* langtag, std::int32_t langtagCapacity, std::int8_t strict, std::int32_t* err);
}
