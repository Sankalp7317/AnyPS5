#include "prx/libSceIcu/RecoveredSignatures.hpp"
#include "prx/libSceIcu/ResourceBundle.hpp"
#include "prx/libSceIcu/Status.hpp"
#include "prx/libc/include/General.hpp"
#include "unicode/ures.h"
#include "uresimp.h"
#include <mutex>
#include <new>
#include <unordered_set>

#undef ures_close
#undef ures_open
#undef ures_getByKey
#undef ures_getStringByKey

namespace {

std::mutex g_lock;
std::unordered_set<const UResourceBundle*> g_live;

const UResourceBundle* Live(const UResourceBundle* bundle, const char* function) {
    if (!bundle) return nullptr;
    std::lock_guard lock(g_lock);
    if (g_live.find(bundle) == g_live.end()) NotImplemented_nid_no_patch(function);
    return bundle;
}

UResourceBundle* Live(UResourceBundle* bundle, const char* function) {
    return const_cast<UResourceBundle*>(Live(static_cast<const UResourceBundle*>(bundle), function));
}

const char* RequireLocale(const char* locale, const char* function) {
    if (!locale) NotImplemented_nid_no_patch(function);
    return locale;
}

UResourceBundle* Track(UResourceBundle* bundle, UErrorCode& status) {
    if (!bundle) return nullptr;
    try {
        std::lock_guard lock(g_lock);
        g_live.insert(bundle);
    } catch (const std::bad_alloc&) {
        ures_close_78(bundle);
        status = U_MEMORY_ALLOCATION_ERROR;
        return nullptr;
    }
    return bundle;
}

template<class TCall>
auto WithStatus(std::int32_t* status, TCall call) {
    Icu::RequireStatus(status);
    auto native = static_cast<UErrorCode>(*status);
    auto result = call(native);
    *status = native;
    return result;
}

}

extern "C" {

void APS5_VABI ures_close(UResourceBundle* resB) {
    if (!resB) return;
    {
        std::lock_guard lock(g_lock);
        if (g_live.erase(resB) == 0) NotImplemented_nid_no_patch(__func__);
    }
    ures_close_78(resB);
}

UResourceBundle* APS5_VABI ures_open(const char* packageName, const char* locale, std::int32_t* status) {
    return WithStatus(status, [&](UErrorCode& native) {
        if (U_FAILURE(native)) return static_cast<UResourceBundle*>(nullptr);
        return Track(ures_open_78(packageName, RequireLocale(locale, "ures_open"), &native), native);
    });
}

UResourceBundle* APS5_VABI ures_openDirect_nid_postfix(const char* packageName, const char* locale, std::int32_t* status) {
    return WithStatus(status, [&](UErrorCode& native) { return Track(ures_openDirect(packageName, locale, &native), native); });
}

UResourceBundle* APS5_VABI ures_openNoDefault_nid_postfix(const char* packageName, const char* locale, std::int32_t* status) {
    return WithStatus(status, [&](UErrorCode& native) {
        if (U_FAILURE(native)) return static_cast<UResourceBundle*>(nullptr);
        return Track(ures_openNoDefault(packageName, RequireLocale(locale, "ures_openNoDefault"), &native), native);
    });
}

std::int32_t APS5_VABI ures_getSize_nid_postfix(const UResourceBundle* resourceBundle) {
    return ures_getSize(Live(resourceBundle, __func__));
}

std::int32_t APS5_VABI ures_getType_nid_postfix(const UResourceBundle* resourceBundle) {
    return ures_getType(Live(resourceBundle, __func__));
}

const char* APS5_VABI ures_getKey_nid_postfix(const UResourceBundle* resourceBundle) {
    return ures_getKey(Live(resourceBundle, __func__));
}

const char16_t* APS5_VABI ures_getString_nid_postfix(const UResourceBundle* resourceBundle, std::int32_t* len, std::int32_t* status) {
    return WithStatus(status, [&](UErrorCode& native) { return ures_getString(Live(resourceBundle, "ures_getString"), len, &native); });
}

std::int32_t APS5_VABI ures_getInt_nid_postfix(const UResourceBundle* resourceBundle, std::int32_t* status) {
    return WithStatus(status, [&](UErrorCode& native) { return ures_getInt(Live(resourceBundle, "ures_getInt"), &native); });
}

const std::int32_t* APS5_VABI ures_getIntVector_nid_postfix(const UResourceBundle* resourceBundle, std::int32_t* len, std::int32_t* status) {
    return WithStatus(status, [&](UErrorCode& native) { return ures_getIntVector(Live(resourceBundle, "ures_getIntVector"), len, &native); });
}

std::int8_t APS5_VABI ures_hasNext_nid_postfix(const UResourceBundle* resourceBundle) {
    return ures_hasNext(Live(resourceBundle, __func__));
}

UResourceBundle* APS5_VABI ures_getNextResource_nid_postfix(UResourceBundle* resourceBundle, UResourceBundle* fillIn, std::int32_t* status) {
    return WithStatus(status, [&](UErrorCode& native) {
        return Track(ures_getNextResource(Live(resourceBundle, "ures_getNextResource"), Live(fillIn, "ures_getNextResource"), &native), native);
    });
}

const char16_t* APS5_VABI ures_getNextString_nid_postfix(UResourceBundle* resourceBundle, std::int32_t* len, const char** key, std::int32_t* status) {
    return WithStatus(status, [&](UErrorCode& native) { return ures_getNextString(Live(resourceBundle, "ures_getNextString"), len, key, &native); });
}

UResourceBundle* APS5_VABI ures_getByIndex_nid_postfix(const UResourceBundle* resourceBundle, std::int32_t indexR, UResourceBundle* fillIn, std::int32_t* status) {
    return WithStatus(status, [&](UErrorCode& native) {
        return Track(ures_getByIndex(Live(resourceBundle, "ures_getByIndex"), indexR, Live(fillIn, "ures_getByIndex"), &native), native);
    });
}

const char16_t* APS5_VABI ures_getStringByIndex_nid_postfix(const UResourceBundle* resourceBundle, std::int32_t indexS, std::int32_t* len, std::int32_t* status) {
    return WithStatus(status, [&](UErrorCode& native) { return ures_getStringByIndex(Live(resourceBundle, "ures_getStringByIndex"), indexS, len, &native); });
}

UResourceBundle* APS5_VABI ures_getByKey(const UResourceBundle* resourceBundle, const char* key, UResourceBundle* fillIn, std::int32_t* status) {
    return WithStatus(status, [&](UErrorCode& native) {
        return Track(ures_getByKey_78(Live(resourceBundle, "ures_getByKey"), key, Live(fillIn, "ures_getByKey"), &native), native);
    });
}

const char16_t* APS5_VABI ures_getStringByKey(const UResourceBundle* resB, const char* key, std::int32_t* len, std::int32_t* status) {
    return WithStatus(status, [&](UErrorCode& native) { return ures_getStringByKey_78(Live(resB, "ures_getStringByKey"), key, len, &native); });
}

}
