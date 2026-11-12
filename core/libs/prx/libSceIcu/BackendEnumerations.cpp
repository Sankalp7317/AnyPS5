#include "prx/libSceIcu/BackendEnumerations.hpp"
#include "prx/libSceIcu/Status.hpp"
#include "prx/libc/include/General.hpp"
#include "unicode/ucnv.h"
#include "unicode/ucurr.h"
#include "unicode/uenum.h"
#include <new>

#undef ucurr_openISOCurrencies

namespace {

UEnumeration* Native(UEnumeration* bridge) {
    return static_cast<UEnumeration*>(bridge->context);
}

template<class TCall>
auto WithStatus(std::int32_t* status, TCall call) {
    auto native = static_cast<UErrorCode>(*status);
    auto result = call(&native);
    *status = native;
    return result;
}

void APS5_VABI Close(UEnumeration* bridge) {
    uenum_close(Native(bridge));
    delete bridge;
}

std::int32_t APS5_VABI Count(UEnumeration* bridge, std::int32_t* status) {
    return WithStatus(status, [&](UErrorCode* native) { return uenum_count(Native(bridge), native); });
}

const char16_t* APS5_VABI UnicodeNext(UEnumeration* bridge, std::int32_t* resultLength, std::int32_t* status) {
    return WithStatus(status, [&](UErrorCode* native) { return uenum_unext(Native(bridge), resultLength, native); });
}

const char* APS5_VABI Next(UEnumeration* bridge, std::int32_t* resultLength, std::int32_t* status) {
    return WithStatus(status, [&](UErrorCode* native) { return uenum_next(Native(bridge), resultLength, native); });
}

void APS5_VABI Reset(UEnumeration* bridge, std::int32_t* status) {
    WithStatus(status, [&](UErrorCode* native) {
        uenum_reset(Native(bridge), native);
        return 0;
    });
}

UEnumeration* Bridge(UEnumeration* native, UErrorCode& status) {
    if (!native) return nullptr;
    auto* bridge = new (std::nothrow) UEnumeration{nullptr, native, Close, Count, UnicodeNext, Next, Reset};
    if (!bridge) {
        uenum_close(native);
        status = U_MEMORY_ALLOCATION_ERROR;
    }
    return bridge;
}

template<class TOpen>
UEnumeration* Open(std::int32_t* pErrorCode, TOpen open) {
    Icu::RequireStatus(pErrorCode);
    auto status = static_cast<UErrorCode>(*pErrorCode);
    auto* bridge = Bridge(open(&status), status);
    *pErrorCode = status;
    return bridge;
}

}

extern "C" {

UEnumeration* APS5_VABI ucnv_openStandardNames_nid_postfix(const char* convName, const char* standard, std::int32_t* pErrorCode) {
    return Open(pErrorCode, [&](UErrorCode* status) { return ucnv_openStandardNames(convName, standard, status); });
}

UEnumeration* APS5_VABI ucurr_openISOCurrencies(std::uint32_t currType, std::int32_t* pErrorCode) {
    return Open(pErrorCode, [&](UErrorCode* status) { return ucurr_openISOCurrencies_78(currType, status); });
}

}
