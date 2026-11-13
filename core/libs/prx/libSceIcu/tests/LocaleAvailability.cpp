#include "prx/libSceIcu/libSceIcu.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <set>
#include <string>

namespace {

void Expect(bool condition, const char* what) {
    if (condition) return;
    std::fprintf(stderr, "locale availability: %s\n", what);
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

}

int main() {
    const auto count = uloc_countAvailable();
    Expect(count > 100, "more than a hundred locales are available");
    std::set<std::string> names;
    for (std::int32_t index = 0; index < count; ++index) {
        const char* name = uloc_getAvailable(index);
        Expect(name != nullptr && *name != 0, "every available locale has a name");
        names.insert(name);
    }
    Expect(static_cast<std::int32_t>(names.size()) == count, "available locales are distinct");
    Expect(names.count("en") && names.count("en_US") && names.count("ja") && names.count("de_DE"), "common locales are available");
    Expect(std::strcmp(uloc_getAvailable(0), uloc_getAvailable(0)) == 0, "the list is stable");
    Expect(uloc_getAvailable(count + 1) == nullptr, "an index past the end returns null");
    Expect(Throws([] { uloc_getAvailable(-1); }), "a negative index throws where ICU reads outside its array");
    Expect(Throws([&] { uloc_getAvailable(count); }), "the count as an index throws where ICU reads outside its array");
}
