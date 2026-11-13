#include "prx/libSceIcu/ScriptCode.hpp"
#include "prx/libSceIcu/Status.hpp"
#include "prx/libc/include/General.hpp"
#include "unicode/uscript.h"

extern "C" {

std::int32_t APS5_VABI uscript_getCode_nid_postfix(const char* nameOrAbbrOrLocale, std::int32_t* fillIn, std::int32_t capacity, std::int32_t* err) {
    Icu::RequireStatus(err);
    auto status = static_cast<UErrorCode>(*err);
    const auto count = uscript_getCode(nameOrAbbrOrLocale, reinterpret_cast<UScriptCode*>(fillIn), capacity, &status);
    *err = status;
    return count;
}

}
