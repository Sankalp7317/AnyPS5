#include "prx/libSceIcu/libSceIcu.hpp"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>

namespace {

constexpr std::int32_t BufferOverflow = 15;

void Expect(bool condition, const char* what) {
    if (condition) return;
    std::fprintf(stderr, "locale tags: %s\n", what);
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

using Buffer = std::array<char, 64>;

bool Result(const Buffer& buffer, std::int32_t length, const char* expected) {
    return length == static_cast<std::int32_t>(std::strlen(expected)) && Same(buffer.data(), expected);
}

void Keywords() {
    Expect(Same(uloc_toUnicodeLocaleKey_nid_postfix("calendar"), "ca"), "calendar maps to ca");
    Expect(Same(uloc_toUnicodeLocaleKey_nid_postfix("collation"), "co"), "collation maps to co");
    Expect(Same(uloc_toUnicodeLocaleKey_nid_postfix("ca"), "ca"), "ca stays ca");
    Expect(Same(uloc_toUnicodeLocaleType("calendar", "gregorian"), "gregory"), "gregorian maps to gregory");
    Expect(Same(uloc_toUnicodeLocaleType("collation", "phonebook"), "phonebk"), "phonebook maps to phonebk");
    Expect(Same(uloc_toLegacyType_nid_postfix("ca", "gregory"), "gregorian"), "gregory maps back to gregorian");
    Expect(Same(uloc_toLegacyType_nid_postfix("co", "phonebk"), "phonebook"), "phonebk maps back to phonebook");
}

void LikelySubtags() {
    Buffer buffer{};
    std::int32_t status = 0;
    const auto capacity = static_cast<std::int32_t>(buffer.size());
    Expect(Result(buffer, uloc_addLikelySubtags("en", buffer.data(), capacity, &status), "en_Latn_US") && status == 0, "en maximizes to en_Latn_US");
    Expect(Result(buffer, uloc_addLikelySubtags("zh_TW", buffer.data(), capacity, &status), "zh_Hant_TW") && status == 0, "zh_TW maximizes to zh_Hant_TW");
    Expect(Result(buffer, uloc_addLikelySubtags("sr", buffer.data(), capacity, &status), "sr_Cyrl_RS") && status == 0, "sr maximizes to sr_Cyrl_RS");
    Expect(Result(buffer, uloc_minimizeSubtags("en_Latn_US", buffer.data(), capacity, &status), "en") && status == 0, "en_Latn_US minimizes to en");
    Expect(Result(buffer, uloc_minimizeSubtags("zh_Hant_TW", buffer.data(), capacity, &status), "zh_TW") && status == 0, "zh_Hant_TW minimizes to zh_TW");

    std::array<char, 4> small{};
    Expect(uloc_addLikelySubtags("en", small.data(), static_cast<std::int32_t>(small.size()), &status) == 10 && status == BufferOverflow, "a short buffer reports overflow and the length");
}

void LanguageTags() {
    Buffer buffer{};
    std::int32_t status = 0;
    const auto capacity = static_cast<std::int32_t>(buffer.size());
    Expect(Result(buffer, uloc_toLanguageTag("en_US", buffer.data(), capacity, 0, &status), "en-US") && status == 0, "en_US becomes en-US");
    Expect(Result(buffer, uloc_toLanguageTag("de_DE@collation=phonebook", buffer.data(), capacity, 0, &status), "de-DE-u-co-phonebk") && status == 0, "collation becomes a u extension");
    Expect(Result(buffer, uloc_toLanguageTag("ja_JP@calendar=japanese", buffer.data(), capacity, 1, &status), "ja-JP-u-ca-japanese") && status == 0, "calendar becomes a u extension");

    std::array<char, 2> small{};
    Expect(uloc_toLanguageTag("en_US", small.data(), static_cast<std::int32_t>(small.size()), 0, &status) == 5 && status == BufferOverflow, "a short buffer reports overflow and the length");
}

void Statuses() {
    Buffer buffer{};
    buffer[0] = 'k';
    const auto capacity = static_cast<std::int32_t>(buffer.size());
    for (const std::int32_t incoming : {1, 7, 15, 27}) {
        std::int32_t status = incoming;
        Expect(uloc_addLikelySubtags("en", buffer.data(), capacity, &status) == 0 && status == incoming, "failing status maximizes nothing");
        Expect(uloc_minimizeSubtags("en_Latn_US", buffer.data(), capacity, &status) == 0 && status == incoming, "failing status minimizes nothing");
        Expect(uloc_toLanguageTag("en_US", buffer.data(), capacity, 0, &status) == 0 && status == incoming, "failing status writes no tag");
        Expect(buffer[0] == 'k', "failing status leaves the buffer");
    }
    Expect(Throws([&] { uloc_addLikelySubtags("en", buffer.data(), capacity, nullptr); }), "null status throws");
    std::int32_t status = 0;
    Expect(Throws([&] { uloc_toLanguageTag(nullptr, buffer.data(), capacity, 0, &status); }), "the default locale is not modelled");
    Expect(Throws([&] { uloc_addLikelySubtags(nullptr, buffer.data(), capacity, &status); }), "the default locale is not modelled for addLikelySubtags");
    Expect(Throws([&] { uloc_minimizeSubtags(nullptr, buffer.data(), capacity, &status); }), "the default locale is not modelled for minimizeSubtags");
}

}

int main() {
    Keywords();
    LikelySubtags();
    LanguageTags();
    Statuses();
}
