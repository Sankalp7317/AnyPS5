#include "prx/libSceIcu/RecoveredSignatures.hpp"
#include "prx/libSceIcu/Status.hpp"
#include "prx/libc/include/General.hpp"
#include "unicode/uidna.h"
#include <cstdint>
#include <mutex>
#include <new>
#include <unordered_set>

namespace {

struct InfoSize {
    std::int16_t size;
};

std::mutex g_lock;
std::unordered_set<const UIDNA*> g_live;

void RequireLive(const UIDNA* idna, const char* function) {
    std::lock_guard lock(g_lock);
    if (g_live.find(idna) == g_live.end()) NotImplemented_nid_no_patch(function);
}

} // namespace

extern "C" {

void APS5_VABI uidna_close_nid_postfix(UIDNA* idna) {
    if (!idna) return;
    {
        std::lock_guard lock(g_lock);
        if (g_live.erase(idna) == 0) NotImplemented_nid_no_patch(__func__);
    }
    uidna_close(idna);
}

std::int32_t APS5_VABI uidna_nameToASCII_nid_postfix(const UIDNA* idna, const char16_t* name, std::int32_t length, char16_t* dest,
                             std::int32_t capacity, UIDNAInfo* pInfo, std::int32_t* pErrorCode) {
    if (Icu::Failure(pErrorCode)) return 0;
    if (!pInfo || reinterpret_cast<const InfoSize*>(pInfo)->size < 16) {
        *pErrorCode = Icu::IllegalArgument;
        return 0;
    }
    if ((name == nullptr) ? length != 0 : length < -1) {
        *pErrorCode = Icu::IllegalArgument;
        return 0;
    }
    if ((dest == nullptr) ? capacity != 0 : capacity < 0) {
        *pErrorCode = Icu::IllegalArgument;
        return 0;
    }
    if (dest == name && name != nullptr) {
        *pErrorCode = Icu::IllegalArgument;
        return 0;
    }
    RequireLive(idna, __func__);
    auto status = static_cast<UErrorCode>(*pErrorCode);
    const auto written = uidna_nameToASCII(idna, name, length, dest, capacity, pInfo, &status);
    *pErrorCode = status;
    return written;
}

std::int32_t APS5_VABI uidna_nameToUnicode_nid_postfix(const UIDNA* idna, const char16_t* name, std::int32_t length, char16_t* dest,
                             std::int32_t capacity, UIDNAInfo* pInfo, std::int32_t* pErrorCode) {
    if (Icu::Failure(pErrorCode)) return 0;
    if (!pInfo || reinterpret_cast<const InfoSize*>(pInfo)->size < 16) {
        *pErrorCode = Icu::IllegalArgument;
        return 0;
    }
    if ((name == nullptr) ? length != 0 : length < -1) {
        *pErrorCode = Icu::IllegalArgument;
        return 0;
    }
    if ((dest == nullptr) ? capacity != 0 : capacity < 0) {
        *pErrorCode = Icu::IllegalArgument;
        return 0;
    }
    if (dest == name && name != nullptr) {
        *pErrorCode = Icu::IllegalArgument;
        return 0;
    }
    RequireLive(idna, __func__);
    auto status = static_cast<UErrorCode>(*pErrorCode);
    const auto written = uidna_nameToUnicode(idna, name, length, dest, capacity, pInfo, &status);
    *pErrorCode = status;
    return written;
}

UIDNA* APS5_VABI uidna_openUTS46_nid_postfix(std::uint32_t options, std::int32_t* pErrorCode) {
    if (Icu::Failure(pErrorCode)) return nullptr;
    auto status = static_cast<UErrorCode>(*pErrorCode);
    auto* idna = uidna_openUTS46(options, &status);
    if (idna) {
        try {
            std::lock_guard lock(g_lock);
            g_live.insert(idna);
        } catch (const std::bad_alloc&) {
            uidna_close(idna);
            idna = nullptr;
            status = U_MEMORY_ALLOCATION_ERROR;
        }
    }
    *pErrorCode = status;
    return idna;
}

}
