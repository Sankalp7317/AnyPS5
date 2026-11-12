#pragma once
#include "prx/libc/include/general/VabiMacros.hpp"
#include <cstdint>

struct UDataMemory;

extern "C" {
UDataMemory* APS5_VABI udata_open_nid_postfix(const char* path, const char* type, const char* name, std::int32_t* pErrorCode);
void APS5_VABI udata_close_nid_postfix(UDataMemory* pData);
const void* APS5_VABI udata_getMemory_nid_postfix(UDataMemory* pData);
std::int32_t APS5_VABI udata_getLength_nid_postfix(const UDataMemory* pData);
void APS5_VABI udata_getInfo_nid_postfix(UDataMemory* pData, void* pInfo);
void APS5_VABI udata_setAppData_nid_postfix(const char* packageName, const void* data, std::int32_t* err);
}
