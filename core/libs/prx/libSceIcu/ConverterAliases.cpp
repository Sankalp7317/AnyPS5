#include "prx/libSceIcu/ConverterAliases.hpp"
#include "prx/libSceIcu/Status.hpp"
#include "prx/libc/include/General.hpp"
#include "unicode/ucnv.h"
#include "ucnv_io.h"

#undef ucnv_getCanonicalName

namespace {

template<class TCall>
auto WithStatus(std::int32_t* pErrorCode, TCall call) {
    Icu::RequireStatus(pErrorCode);
    auto status = static_cast<UErrorCode>(*pErrorCode);
    auto result = call(&status);
    *pErrorCode = status;
    return result;
}

}

extern "C" {

std::uint16_t APS5_VABI ucnv_countAliases_nid_postfix(const char* alias, std::int32_t* pErrorCode) {
    return WithStatus(pErrorCode, [&](UErrorCode* status) { return ucnv_countAliases(alias, status); });
}

const char* APS5_VABI ucnv_getAlias_nid_postfix(const char* alias, std::uint16_t n, std::int32_t* pErrorCode) {
    return WithStatus(pErrorCode, [&](UErrorCode* status) { return ucnv_getAlias(alias, n, status); });
}

void APS5_VABI ucnv_getAliases_nid_postfix(const char* alias, const char** aliases, std::int32_t* pErrorCode) {
    WithStatus(pErrorCode, [&](UErrorCode* status) {
        ucnv_getAliases(alias, aliases, status);
        return 0;
    });
}

std::uint16_t APS5_VABI ucnv_countStandards_nid_postfix() {
    return ucnv_countStandards();
}

const char* APS5_VABI ucnv_getStandard_nid_postfix(std::uint16_t n, std::int32_t* pErrorCode) {
    return WithStatus(pErrorCode, [&](UErrorCode* status) { return ucnv_getStandard(n, status); });
}

const char* APS5_VABI ucnv_getStandardName_nid_postfix(const char* name, const char* standard, std::int32_t* pErrorCode) {
    return WithStatus(pErrorCode, [&](UErrorCode* status) { return ucnv_getStandardName(name, standard, status); });
}

const char* APS5_VABI ucnv_getCanonicalName(const char* alias, const char* standard, std::int32_t* pErrorCode) {
    return WithStatus(pErrorCode, [&](UErrorCode* status) { return ucnv_getCanonicalName_78(alias, standard, status); });
}

std::uint16_t APS5_VABI ucnv_io_countKnownConverters_nid_postfix(std::int32_t* pErrorCode) {
    return WithStatus(pErrorCode, [&](UErrorCode* status) { return ucnv_io_countKnownConverters(status); });
}

const char* APS5_VABI ucnv_io_getConverterName_nid_postfix(const char* alias, std::int8_t* containsOption, std::int32_t* pErrorCode) {
    return WithStatus(pErrorCode, [&](UErrorCode* status) { return ucnv_io_getConverterName(alias, containsOption, status); });
}

}
