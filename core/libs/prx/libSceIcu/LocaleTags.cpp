#include "prx/libSceIcu/LocaleTags.hpp"
#include "prx/libSceIcu/Status.hpp"
#include "prx/libc/include/General.hpp"
#include "unicode/uloc.h"

#undef uloc_toUnicodeLocaleType
#undef uloc_addLikelySubtags
#undef uloc_minimizeSubtags
#undef uloc_toLanguageTag

namespace {

const char* RequireLocale(const char* localeID, const char* function) {
    if (!localeID) NotImplemented_nid_no_patch(function);
    return localeID;
}

template<class TCall>
std::int32_t WithStatus(std::int32_t* err, TCall call) {
    Icu::RequireStatus(err);
    auto status = static_cast<UErrorCode>(*err);
    const auto result = call(&status);
    *err = status;
    return result;
}

}

extern "C" {

const char* APS5_VABI uloc_toUnicodeLocaleKey_nid_postfix(const char* keyword) {
    return uloc_toUnicodeLocaleKey_78(keyword);
}

const char* APS5_VABI uloc_toUnicodeLocaleType(const char* keyword, const char* value) {
    return uloc_toUnicodeLocaleType_78(keyword, value);
}

const char* APS5_VABI uloc_toLegacyType_nid_postfix(const char* keyword, const char* value) {
    return uloc_toLegacyType_78(keyword, value);
}

std::int32_t APS5_VABI uloc_addLikelySubtags(const char* localeID, char* maximizedLocaleID, std::int32_t maximizedLocaleIDCapacity, std::int32_t* err) {
    return WithStatus(err, [&](UErrorCode* status) {
        if (U_FAILURE(*status)) return 0;
        return uloc_addLikelySubtags_78(RequireLocale(localeID, "uloc_addLikelySubtags"), maximizedLocaleID, maximizedLocaleIDCapacity, status);
    });
}

std::int32_t APS5_VABI uloc_minimizeSubtags(const char* localeID, char* minimizedLocaleID, std::int32_t minimizedLocaleIDCapacity, std::int32_t* err) {
    return WithStatus(err, [&](UErrorCode* status) {
        if (U_FAILURE(*status)) return 0;
        return uloc_minimizeSubtags_78(RequireLocale(localeID, "uloc_minimizeSubtags"), minimizedLocaleID, minimizedLocaleIDCapacity, status);
    });
}

std::int32_t APS5_VABI uloc_toLanguageTag(const char* localeID, char* langtag, std::int32_t langtagCapacity, std::int8_t strict, std::int32_t* err) {
    return WithStatus(err, [&](UErrorCode* status) {
        if (U_FAILURE(*status)) return 0;
        return uloc_toLanguageTag_78(RequireLocale(localeID, "uloc_toLanguageTag"), langtag, langtagCapacity, strict, status);
    });
}

}
