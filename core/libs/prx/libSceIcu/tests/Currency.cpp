#include "prx/libSceIcu/libSceIcu.hpp"
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <exception>

namespace {

constexpr std::int32_t BufferOverflow = 15;
constexpr std::int32_t UsageStandard = 0;
constexpr std::int32_t UsageCash = 1;

void Expect(bool condition, const char* what) {
    if (condition) return;
    std::fprintf(stderr, "currency: %s\n", what);
    std::abort();
}

bool Code(const std::array<char16_t, 8>& buffer, std::int32_t length, const char16_t* expected) {
    if (length != 3) return false;
    for (int index = 0; index < 3; ++index) {
        if (buffer[index] != expected[index]) return false;
    }
    return buffer[3] == 0;
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

void ForLocale() {
    std::array<char16_t, 8> buffer{};
    std::int32_t status = 0;
    Expect(Code(buffer, ucurr_forLocale_nid_postfix("en_US", buffer.data(), static_cast<std::int32_t>(buffer.size()), &status), u"USD") && status == 0, "en_US uses USD");
    Expect(Code(buffer, ucurr_forLocale_nid_postfix("ja_JP", buffer.data(), static_cast<std::int32_t>(buffer.size()), &status), u"JPY") && status == 0, "ja_JP uses JPY");
    Expect(Code(buffer, ucurr_forLocale_nid_postfix("de_DE", buffer.data(), static_cast<std::int32_t>(buffer.size()), &status), u"EUR") && status == 0, "de_DE uses EUR");
    Expect(Code(buffer, ucurr_forLocale_nid_postfix("en_US@currency=JPY", buffer.data(), static_cast<std::int32_t>(buffer.size()), &status), u"JPY") && status == 0, "the currency keyword wins");

    std::array<char16_t, 2> small{u'x', u'y'};
    Expect(ucurr_forLocale_nid_postfix("en_US", small.data(), static_cast<std::int32_t>(small.size()), &status) == 3 && status == BufferOverflow, "a short buffer reports overflow and the length");
}

void Metadata() {
    std::int32_t status = 0;
    Expect(ucurr_getDefaultFractionDigits_nid_postfix(u"USD", &status) == 2 && status == 0, "USD has two fraction digits");
    Expect(ucurr_getDefaultFractionDigits_nid_postfix(u"JPY", &status) == 0 && status == 0, "JPY has no fraction digits");
    Expect(ucurr_getDefaultFractionDigitsForUsage_nid_postfix(u"CHF", UsageCash, &status) == 2 && status == 0, "CHF cash has two fraction digits");
    Expect(ucurr_getRoundingIncrement_nid_postfix(u"USD", &status) == 0.0 && status == 0, "USD has no rounding increment");
    Expect(std::fabs(ucurr_getRoundingIncrementForUsage_nid_postfix(u"CHF", UsageCash, &status) - 0.05) < 1e-9 && status == 0, "CHF cash rounds to 0.05");
    Expect(ucurr_getRoundingIncrementForUsage_nid_postfix(u"CHF", UsageStandard, &status) == 0.0 && status == 0, "CHF standard has no rounding increment");
    Expect(ucurr_getNumericCode_nid_postfix(u"USD") == 840, "USD is 840");
    Expect(ucurr_getNumericCode_nid_postfix(u"JPY") == 392, "JPY is 392");
    Expect(ucurr_getNumericCode_nid_postfix(u"EUR") == 978, "EUR is 978");
}

void Statuses() {
    std::array<char16_t, 8> buffer{u'k', u'e', u'e', u'p'};
    for (const std::int32_t incoming : {1, 7, 15, 27}) {
        std::int32_t status = incoming;
        Expect(ucurr_forLocale_nid_postfix("en_US", buffer.data(), static_cast<std::int32_t>(buffer.size()), &status) == 0 && status == incoming, "failing status finds no currency");
        Expect(buffer[0] == u'k', "failing status leaves the buffer");
        Expect(ucurr_getDefaultFractionDigits_nid_postfix(u"USD", &status) == 0 && status == incoming, "failing status gives no digits");
        Expect(ucurr_getRoundingIncrement_nid_postfix(u"USD", &status) == 0.0 && status == incoming, "failing status gives no increment");
    }
    Expect(Throws([] { ucurr_getDefaultFractionDigits_nid_postfix(u"USD", nullptr); }), "null status throws");
}

}

int main() {
    ForLocale();
    Metadata();
    Statuses();
}
