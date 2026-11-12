#include "prx/libSceIcu/libSceIcu.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <string>
#include <vector>

namespace {

constexpr std::uint32_t CurrencyCommon = 1;
constexpr std::uint32_t CurrencyNonDeprecated = 8;

void Expect(bool condition, const char* what) {
    if (condition) return;
    std::fprintf(stderr, "backend enumerations: %s\n", what);
    std::abort();
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

std::vector<std::string> Drain(UEnumeration* enumeration) {
    std::vector<std::string> names;
    std::int32_t status = 0;
    std::int32_t length = 0;
    while (const char* name = uenum_next_nid_postfix(enumeration, &length, &status)) {
        Expect(status == 0 && length == static_cast<std::int32_t>(std::strlen(name)), "each name has its length");
        names.emplace_back(name, static_cast<std::size_t>(length));
    }
    Expect(status == 0, "draining ends without an error");
    return names;
}

bool Contains(const std::vector<std::string>& names, const char* name) {
    for (const auto& candidate : names) {
        if (candidate == name) return true;
    }
    return false;
}

void StandardNames() {
    std::int32_t status = 0;
    auto* names = ucnv_openStandardNames_nid_postfix("ISO-8859-1", "IANA", &status);
    Expect(names != nullptr && status == 0, "IANA names of ISO-8859-1 open");
    const auto count = uenum_count_nid_postfix(names, &status);
    const auto all = Drain(names);
    Expect(count == static_cast<std::int32_t>(all.size()) && count > 1, "count matches the names");
    Expect(Contains(all, "ISO-8859-1") && Contains(all, "latin1"), "IANA names include ISO-8859-1 and latin1");

    names->reset(names, &status);
    Expect(status == 0 && Drain(names) == all, "reset restarts the names");

    names->reset(names, &status);
    std::int32_t length = 0;
    const char16_t* first = uenum_unext_nid_postfix(names, &length, &status);
    Expect(first != nullptr && status == 0 && length == static_cast<std::int32_t>(all[0].size()), "unext returns the first name");
    for (std::int32_t index = 0; index < length; ++index) {
        Expect(first[index] == static_cast<char16_t>(all[0][static_cast<std::size_t>(index)]), "unext matches next");
    }
    uenum_close_nid_postfix(names);

    names = ucnv_openStandardNames_nid_postfix("UTF-8", "MIME", &status);
    Expect(names != nullptr && status == 0, "MIME names of UTF-8 open");
    const auto mime = Drain(names);
    Expect(!mime.empty() && mime[0] == "UTF-8", "the preferred MIME name of UTF-8 comes first");
    uenum_close_nid_postfix(names);
}

void Currencies() {
    std::int32_t status = 0;
    auto* currencies = ucurr_openISOCurrencies(CurrencyCommon | CurrencyNonDeprecated, &status);
    Expect(currencies != nullptr && status == 0, "common current currencies open");
    const auto count = uenum_count_nid_postfix(currencies, &status);
    const auto all = Drain(currencies);
    Expect(count == static_cast<std::int32_t>(all.size()) && count > 100, "there are over a hundred common currencies");
    Expect(Contains(all, "USD") && Contains(all, "EUR") && Contains(all, "JPY"), "USD, EUR and JPY are common currencies");
    Expect(!Contains(all, "DEM"), "DEM is not a current currency");
    uenum_close_nid_postfix(currencies);
}

void Statuses() {
    for (const std::int32_t incoming : {1, 7, 15, 27}) {
        std::int32_t status = incoming;
        Expect(ucnv_openStandardNames_nid_postfix("UTF-8", "MIME", &status) == nullptr && status == incoming, "failing status opens no names");
        auto* currencies = ucurr_openISOCurrencies(CurrencyCommon, &status);
        Expect(currencies != nullptr && status == incoming, "ICU opens currencies whatever the incoming status is");
        uenum_close_nid_postfix(currencies);
    }
    Expect(Throws([] { ucnv_openStandardNames_nid_postfix("UTF-8", "MIME", nullptr); }), "null status throws");
    Expect(Throws([] { ucurr_openISOCurrencies(CurrencyCommon, nullptr); }), "null status throws for currencies");
}

}

int main() {
    StandardNames();
    Currencies();
    Statuses();
}
