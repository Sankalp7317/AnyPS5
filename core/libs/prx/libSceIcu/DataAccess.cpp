#include "prx/libSceIcu/DataAccess.hpp"
#include "prx/libSceIcu/Status.hpp"
#include "prx/libc/include/General.hpp"
#include "unicode/udata.h"
#include "udatamem.h"
#include <mutex>
#include <new>
#include <unordered_set>

namespace {

std::mutex g_lock;
std::unordered_set<const UDataMemory*> g_live;

void RequireLive(const UDataMemory* data, const char* function) {
    std::lock_guard lock(g_lock);
    if (g_live.find(data) == g_live.end()) NotImplemented_nid_no_patch(function);
}

}

extern "C" {

UDataMemory* APS5_VABI udata_open_nid_postfix(const char* path, const char* type, const char* name, std::int32_t* pErrorCode) {
    Icu::RequireStatus(pErrorCode);
    auto status = static_cast<UErrorCode>(*pErrorCode);
    auto* data = udata_open(path, type, name, &status);
    if (data) {
        try {
            std::lock_guard lock(g_lock);
            g_live.insert(data);
        } catch (const std::bad_alloc&) {
            udata_close(data);
            data = nullptr;
            status = U_MEMORY_ALLOCATION_ERROR;
        }
    }
    *pErrorCode = status;
    return data;
}

void APS5_VABI udata_close_nid_postfix(UDataMemory* pData) {
    if (!pData) return;
    {
        std::lock_guard lock(g_lock);
        if (g_live.erase(pData) == 0) NotImplemented_nid_no_patch(__func__);
    }
    udata_close(pData);
}

const void* APS5_VABI udata_getMemory_nid_postfix(UDataMemory* pData) {
    if (!pData) return nullptr;
    RequireLive(pData, __func__);
    return udata_getMemory(pData);
}

std::int32_t APS5_VABI udata_getLength_nid_postfix(const UDataMemory* pData) {
    if (!pData) return -1;
    RequireLive(pData, __func__);
    return udata_getLength(pData);
}

void APS5_VABI udata_getInfo_nid_postfix(UDataMemory* pData, void* pInfo) {
    if (pData) RequireLive(pData, __func__);
    udata_getInfo(pData, static_cast<UDataInfo*>(pInfo));
}

void APS5_VABI udata_setAppData_nid_postfix(const char* packageName, const void* data, std::int32_t* err) {
    Icu::RequireStatus(err);
    auto status = static_cast<UErrorCode>(*err);
    udata_setAppData(packageName, data, &status);
    *err = status;
}

}
