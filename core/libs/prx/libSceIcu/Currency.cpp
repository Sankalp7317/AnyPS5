#include "prx/libSceIcu/Currency.hpp"
#include "prx/libSceIcu/Status.hpp"
#include "prx/libc/include/General.hpp"
#include "unicode/ucurr.h"

namespace {

template<class TCall>
auto WithStatus(std::int32_t* ec, TCall call) {
    Icu::RequireStatus(ec);
    auto status = static_cast<UErrorCode>(*ec);
    auto result = call(&status);
    *ec = status;
    return result;
}

}

extern "C" {

std::int32_t APS5_VABI ucurr_forLocale_nid_postfix(const char* locale, char16_t* buff, std::int32_t buffCapacity, std::int32_t* ec) {
    return WithStatus(ec, [&](UErrorCode* status) { return ucurr_forLocale(locale, buff, buffCapacity, status); });
}

std::int32_t APS5_VABI ucurr_getDefaultFractionDigits_nid_postfix(const char16_t* currency, std::int32_t* ec) {
    return WithStatus(ec, [&](UErrorCode* status) { return ucurr_getDefaultFractionDigits(currency, status); });
}

std::int32_t APS5_VABI ucurr_getDefaultFractionDigitsForUsage_nid_postfix(const char16_t* currency, std::int32_t usage, std::int32_t* ec) {
    return WithStatus(ec, [&](UErrorCode* status) { return ucurr_getDefaultFractionDigitsForUsage(currency, static_cast<UCurrencyUsage>(usage), status); });
}

double APS5_VABI ucurr_getRoundingIncrement_nid_postfix(const char16_t* currency, std::int32_t* ec) {
    return WithStatus(ec, [&](UErrorCode* status) { return ucurr_getRoundingIncrement(currency, status); });
}

double APS5_VABI ucurr_getRoundingIncrementForUsage_nid_postfix(const char16_t* currency, std::int32_t usage, std::int32_t* ec) {
    return WithStatus(ec, [&](UErrorCode* status) { return ucurr_getRoundingIncrementForUsage(currency, static_cast<UCurrencyUsage>(usage), status); });
}

std::int32_t APS5_VABI ucurr_getNumericCode_nid_postfix(const char16_t* currency) {
    return ucurr_getNumericCode(currency);
}

}
