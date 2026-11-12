#include "prx/libSceIcu/libSceIcu.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <vector>

namespace {

constexpr std::int32_t IndexOutOfBounds = 8;

void Expect(bool condition, const char* what) {
    if (condition) return;
    std::fprintf(stderr, "converter aliases: %s\n", what);
    std::abort();
}

bool Same(const char* actual, const char* expected) {
    return actual != nullptr && std::strcmp(actual, expected) == 0;
}

template<class TAction>
bool Throws(TAction action) {
    try {
        action();
    } catch (const std::exception&) {
        return true;
    }
    return false;
}

void Lookups() {
    std::int32_t status = 0;
    Expect(Same(ucnv_getCanonicalName("csISOLatin1", "IANA", &status), "ISO-8859-1") && status == 0, "IANA alias csISOLatin1 maps to ISO-8859-1");
    Expect(Same(ucnv_getStandardName_nid_postfix("ISO-8859-1", "MIME", &status), "ISO-8859-1") && status == 0, "MIME name of ISO-8859-1");
    Expect(Same(ucnv_getStandardName_nid_postfix("ibm-1208", "IANA", &status), "UTF-8") && status == 0, "IANA name of ibm-1208");

    std::int8_t containsOption = 0x5a;
    Expect(Same(ucnv_io_getConverterName_nid_postfix("latin1", &containsOption, &status), "ISO-8859-1") && status == 0, "latin1 converter name");
    Expect(containsOption == 0, "latin1 has no option");
    Expect(Same(ucnv_io_getConverterName_nid_postfix("utf8", &containsOption, &status), "UTF-8") && status == 0, "utf8 converter name");
    Expect(ucnv_io_countKnownConverters_nid_postfix(&status) > 0 && status == 0, "known converters are counted");
}

void Aliases() {
    std::int32_t status = 0;
    const auto count = ucnv_countAliases_nid_postfix("utf8", &status);
    Expect(count > 1 && status == 0, "UTF-8 has several aliases");
    Expect(Same(ucnv_getAlias_nid_postfix("utf8", 0, &status), "UTF-8") && status == 0, "first UTF-8 alias is its converter name");
    std::vector<const char*> aliases(count, nullptr);
    ucnv_getAliases_nid_postfix("utf8", aliases.data(), &status);
    Expect(status == 0 && Same(aliases[0], "UTF-8"), "getAliases starts with the converter name");
    bool hasIbm1208 = false;
    for (std::uint16_t index = 0; index < count; ++index) {
        Expect(Same(ucnv_getAlias_nid_postfix("utf8", index, &status), aliases[index]), "getAlias matches getAliases");
        hasIbm1208 = hasIbm1208 || Same(aliases[index], "ibm-1208");
    }
    Expect(hasIbm1208, "ibm-1208 is among the UTF-8 aliases");
    status = 0;
    ucnv_getAlias_nid_postfix("utf8", count, &status);
    Expect(status == IndexOutOfBounds, "alias past the end reports an index error");
}

void Standards() {
    const auto count = ucnv_countStandards_nid_postfix();
    Expect(count > 0, "standards are counted");
    std::int32_t status = 0;
    bool hasIana = false;
    for (std::uint16_t index = 0; index < count; ++index) {
        const auto* name = ucnv_getStandard_nid_postfix(index, &status);
        Expect(name != nullptr && status == 0, "every counted standard has a name");
        hasIana = hasIana || Same(name, "IANA");
    }
    Expect(hasIana, "IANA is a standard");
    Expect(ucnv_getStandard_nid_postfix(count, &status) == nullptr && status == IndexOutOfBounds, "standard past the end reports an index error");
}

void Statuses() {
    for (const std::int32_t incoming : {1, 7, 15, 27}) {
        std::int32_t status = incoming;
        Expect(ucnv_countAliases_nid_postfix("utf8", &status) == 0 && status == incoming, "failing status counts no aliases");
        Expect(ucnv_getAlias_nid_postfix("utf8", 0, &status) == nullptr && status == incoming, "failing status returns no alias");
        const char* untouched[1] = {"keep"};
        ucnv_getAliases_nid_postfix("utf8", untouched, &status);
        Expect(status == incoming && Same(untouched[0], "keep"), "failing status leaves the alias array");
        Expect(ucnv_getStandard_nid_postfix(0, &status) == nullptr && status == incoming, "failing status returns no standard");
        Expect(ucnv_getStandardName_nid_postfix("utf8", "IANA", &status) == nullptr && status == incoming, "failing status returns no standard name");
        Expect(ucnv_getCanonicalName("utf8", "IANA", &status) == nullptr && status == incoming, "failing status returns no canonical name");
        Expect(ucnv_io_countKnownConverters_nid_postfix(&status) == 0 && status == incoming, "failing status counts no converters");
        std::int8_t containsOption = 0;
        Expect(ucnv_io_getConverterName_nid_postfix("utf8", &containsOption, &status) == nullptr && status == incoming, "failing status returns no converter");
    }
    Expect(Throws([] { ucnv_countAliases_nid_postfix("utf8", nullptr); }), "null status throws");
    Expect(Throws([] { ucnv_getCanonicalName("utf8", "IANA", nullptr); }), "null status throws for getCanonicalName");
}

}

int main() {
    Lookups();
    Aliases();
    Standards();
    Statuses();
}
