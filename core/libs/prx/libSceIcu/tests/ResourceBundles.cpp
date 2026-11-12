#include "prx/libSceIcu/libSceIcu.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <initializer_list>

namespace {

constexpr std::int32_t MissingResource = 2;
constexpr std::int32_t TypeString = 0;
constexpr std::int32_t TypeTable = 2;
constexpr std::int32_t TypeInt = 7;
constexpr std::int32_t TypeIntVector = 14;

void Expect(bool condition, const char* what) {
    if (condition) return;
    std::fprintf(stderr, "resource bundles: %s\n", what);
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

bool Same(const char16_t* actual, std::int32_t length, const char16_t* expected) {
    std::int32_t index = 0;
    for (; expected[index] != 0; ++index) {
        if (index >= length || actual[index] != expected[index]) return false;
    }
    return index == length;
}

void Integers() {
    std::int32_t status = 0;
    auto* bundle = ures_openDirect_nid_postfix(nullptr, "currencyNumericCodes", &status);
    Expect(bundle != nullptr && status == 0 && ures_getType_nid_postfix(bundle) == TypeTable, "currencyNumericCodes opens as a table");
    auto* codes = ures_getByKey(bundle, "codeMap", nullptr, &status);
    Expect(codes != nullptr && status == 0 && ures_getType_nid_postfix(codes) == TypeTable, "codeMap is a table");
    const auto size = ures_getSize_nid_postfix(codes);
    Expect(size > 100, "codeMap has over a hundred currencies");

    auto* item = ures_getByKey(codes, "USD", nullptr, &status);
    Expect(item != nullptr && status == 0 && ures_getType_nid_postfix(item) == TypeInt, "USD is an integer");
    Expect(ures_getInt_nid_postfix(item, &status) == 840 && status == 0, "USD is 840");
    Expect(std::strcmp(ures_getKey_nid_postfix(item), "USD") == 0, "the item knows its key");
    Expect(ures_getByKey(codes, "JPY", item, &status) == item && status == 0, "a fill-in bundle is reused");
    Expect(ures_getInt_nid_postfix(item, &status) == 392 && status == 0, "JPY is 392");

    Expect(ures_getByKey(codes, "NOPE", item, &status) == item && status == MissingResource, "a missing key reports a missing resource");
    status = 0;

    std::int32_t walked = 0;
    while (ures_hasNext_nid_postfix(codes)) {
        Expect(ures_getNextResource_nid_postfix(codes, item, &status) == item && status == 0, "iteration fills the same bundle");
        Expect(ures_getKey_nid_postfix(item) != nullptr, "each iterated item has a key");
        ++walked;
    }
    Expect(walked == size, "iteration visits every item");
    auto* first = ures_getByIndex_nid_postfix(codes, 0, nullptr, &status);
    Expect(first != nullptr && status == 0 && ures_getKey_nid_postfix(first) != nullptr, "the first item opens by index");

    ures_close(first);
    ures_close(item);
    ures_close(codes);
    ures_close(bundle);
    Expect(Throws([&] { ures_close(bundle); }), "closing twice throws");
    Expect(Throws([&] { ures_getSize_nid_postfix(bundle); }), "a closed bundle throws");
}

void Strings() {
    std::int32_t status = 0;
    auto* bundle = ures_openDirect_nid_postfix(nullptr, "keyTypeData", &status);
    Expect(bundle != nullptr && status == 0, "keyTypeData opens");
    auto* keys = ures_getByKey(bundle, "keyMap", nullptr, &status);
    Expect(keys != nullptr && status == 0, "keyMap opens");
    std::int32_t length = 0;
    const auto* calendar = ures_getStringByKey(keys, "calendar", &length, &status);
    Expect(calendar != nullptr && status == 0 && Same(calendar, length, u"ca"), "calendar maps to ca");

    auto* entry = ures_getByKey(keys, "collation", nullptr, &status);
    Expect(entry != nullptr && status == 0 && ures_getType_nid_postfix(entry) == TypeString, "collation is a string");
    const auto* collation = ures_getString_nid_postfix(entry, &length, &status);
    Expect(collation != nullptr && status == 0 && Same(collation, length, u"co"), "collation maps to co");

    const auto* byIndex = ures_getStringByIndex_nid_postfix(keys, 0, &length, &status);
    Expect(byIndex != nullptr && status == 0 && length > 0, "the first string opens by index");

    std::int32_t walked = 0;
    const char* key = nullptr;
    while (ures_hasNext_nid_postfix(keys)) {
        Expect(ures_getNextString_nid_postfix(keys, &length, &key, &status) != nullptr && status == 0 && key != nullptr, "each iterated string has a key");
        ++walked;
    }
    Expect(walked == ures_getSize_nid_postfix(keys), "string iteration visits every item");

    ures_close(entry);
    ures_close(keys);
    ures_close(bundle);
}

void Vectors() {
    std::int32_t status = 0;
    auto* bundle = ures_openDirect_nid_postfix(nullptr, "supplementalData", &status);
    Expect(bundle != nullptr && status == 0, "supplementalData opens");
    auto* weeks = ures_getByKey(bundle, "weekData", nullptr, &status);
    auto* world = ures_getByKey(weeks, "001", nullptr, &status);
    Expect(world != nullptr && status == 0 && ures_getType_nid_postfix(world) == TypeIntVector, "weekData/001 is an integer vector");
    std::int32_t length = 0;
    const auto* values = ures_getIntVector_nid_postfix(world, &length, &status);
    Expect(values != nullptr && status == 0 && length >= 6, "weekData/001 has its fields");
    ures_close(world);
    ures_close(weeks);
    ures_close(bundle);
}

void Opening() {
    std::int32_t status = 0;
    auto* root = ures_open(nullptr, "root", &status);
    Expect(root != nullptr && status <= 0, "the root locale opens");
    ures_close(root);
    status = 0;
    root = ures_openNoDefault_nid_postfix(nullptr, "root", &status);
    Expect(root != nullptr && status <= 0, "the root locale opens without default fallback");
    ures_close(root);

    status = 0;
    Expect(Throws([&] { ures_open(nullptr, nullptr, &status); }), "the default locale is not modelled");
    Expect(Throws([&] { ures_openNoDefault_nid_postfix(nullptr, nullptr, &status); }), "the default locale is not modelled without fallback");
}

void Statuses() {
    for (const std::int32_t incoming : {1, 7, 15, 27}) {
        std::int32_t status = incoming;
        Expect(ures_openDirect_nid_postfix(nullptr, "keyTypeData", &status) == nullptr && status == incoming, "failing status opens nothing directly");
        Expect(ures_open(nullptr, "root", &status) == nullptr && status == incoming, "failing status opens no locale");
        Expect(ures_getByKey(nullptr, "keyMap", nullptr, &status) == nullptr && status == incoming, "failing status finds nothing");
    }
    std::int32_t status = 0;
    Expect(ures_getByKey(nullptr, "keyMap", nullptr, &status) == nullptr && status != 0, "a null bundle reports an error");
    Expect(Throws([] { ures_openDirect_nid_postfix(nullptr, "keyTypeData", nullptr); }), "null status throws");
    alignas(16) unsigned char foreign[64] = {};
    Expect(Throws([&] { ures_getSize_nid_postfix(reinterpret_cast<UResourceBundle*>(foreign)); }), "an unknown bundle throws");
}

}

int main() {
    Integers();
    Strings();
    Vectors();
    Opening();
    Statuses();
}
