#pragma once
#include "prx/libc/include/general/VabiMacros.hpp"
#include <cstdint>

extern "C" {
std::uint16_t APS5_VABI ucnv_countAliases_nid_postfix(const char* alias, std::int32_t* pErrorCode);
const char* APS5_VABI ucnv_getAlias_nid_postfix(const char* alias, std::uint16_t n, std::int32_t* pErrorCode);
void APS5_VABI ucnv_getAliases_nid_postfix(const char* alias, const char** aliases, std::int32_t* pErrorCode);
std::uint16_t APS5_VABI ucnv_countStandards_nid_postfix();
const char* APS5_VABI ucnv_getStandard_nid_postfix(std::uint16_t n, std::int32_t* pErrorCode);
const char* APS5_VABI ucnv_getStandardName_nid_postfix(const char* name, const char* standard, std::int32_t* pErrorCode);
const char* APS5_VABI ucnv_getCanonicalName(const char* alias, const char* standard, std::int32_t* pErrorCode);
std::uint16_t APS5_VABI ucnv_io_countKnownConverters_nid_postfix(std::int32_t* pErrorCode);
const char* APS5_VABI ucnv_io_getConverterName_nid_postfix(const char* alias, std::int8_t* containsOption, std::int32_t* pErrorCode);
}
