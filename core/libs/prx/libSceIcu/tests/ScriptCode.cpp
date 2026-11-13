#include "prx/libSceIcu/libSceIcu.hpp"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <initializer_list>

namespace {

constexpr std::int32_t Arabic = 2;
constexpr std::int32_t Cyrillic = 8;
constexpr std::int32_t Greek = 14;
constexpr std::int32_t Han = 17;
constexpr std::int32_t Hiragana = 20;
constexpr std::int32_t Katakana = 22;
constexpr std::int32_t Latin = 25;
constexpr std::int32_t NewTaiLue = 59;
constexpr std::int32_t OldHungarian = 76;
constexpr std::int32_t IllegalArgument = 1;
constexpr std::int32_t BufferOverflow = 15;

void Expect(bool condition, const char* what) {
    if (condition) return;
    std::fprintf(stderr, "script code: %s\n", what);
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

std::int32_t One(const char* name, std::int32_t& status) {
    std::array<std::int32_t, 4> codes{-1, -1, -1, -1};
    const auto count = uscript_getCode_nid_postfix(name, codes.data(), static_cast<std::int32_t>(codes.size()), &status);
    return count == 1 ? codes[0] : -1;
}

}

int main() {
    std::int32_t status = 0;
    Expect(One("Latin", status) == Latin && status == 0, "Latin by name");
    Expect(One("Latn", status) == Latin && status == 0, "Latin by code");
    Expect(One("Cyrl", status) == Cyrillic && status == 0, "Cyrillic by code");
    Expect(One("Greek", status) == Greek && status == 0, "Greek by name");
    Expect(One("Old_Hungarian", status) == OldHungarian && status == 0, "a name with an underscore");
    Expect(One("New_Tai_Lue", status) == NewTaiLue && status == 0, "a three-part name");
    Expect(One("ru", status) == Cyrillic && status == 0, "Russian uses Cyrillic");
    Expect(One("ar_EG", status) == Arabic && status == 0, "Egyptian Arabic uses Arabic");
    Expect(One("sr_Latn", status) == Latin && status == 0, "an explicit script subtag wins");

    std::array<std::int32_t, 4> codes{};
    auto count = uscript_getCode_nid_postfix("ja", codes.data(), static_cast<std::int32_t>(codes.size()), &status);
    Expect(count == 3 && status == 0, "Japanese has three scripts");
    Expect(codes[0] == Katakana && codes[1] == Hiragana && codes[2] == Han, "Japanese uses Katakana, Hiragana and Han");
    count = uscript_getCode_nid_postfix("ja", codes.data(), 1, &status);
    Expect(count == 3 && status == BufferOverflow, "a short buffer reports overflow and the count");
    status = 0;
    Expect(uscript_getCode_nid_postfix("ja", nullptr, 0, &status) == 3 && status == BufferOverflow, "preflight reports the count");

    status = 0;
    Expect(uscript_getCode_nid_postfix("not a script", codes.data(), 4, &status) == 0, "an unknown name has no script");
    status = 0;
    Expect(uscript_getCode_nid_postfix(nullptr, codes.data(), 4, &status) == 0 && status == IllegalArgument, "a null name is an illegal argument");
    status = 0;
    Expect(uscript_getCode_nid_postfix("Latn", nullptr, 4, &status) == 0 && status == IllegalArgument, "a null buffer with capacity is an illegal argument");
    for (const std::int32_t incoming : {1, 7, 27}) {
        status = incoming;
        Expect(uscript_getCode_nid_postfix("Latn", codes.data(), 4, &status) == 0 && status == incoming, "failing status finds nothing");
    }
    Expect(Throws([&] { uscript_getCode_nid_postfix("Latn", codes.data(), 4, nullptr); }), "null status throws");
}
