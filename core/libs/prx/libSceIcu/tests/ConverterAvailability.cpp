#include "prx/libSceIcu/libSceIcu.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <string>

namespace {

void Expect(bool condition, const char* what) {
    if (condition) return;
    std::fprintf(stderr, "converter availability: %s\n", what);
    std::abort();
}

}

int main() {
    const auto count = ucnv_countAvailable_nid_postfix();
    Expect(count > 0, "some converters are available");
    std::set<std::string> names;
    for (std::int32_t index = 0; index < count; ++index) {
        const char* name = ucnv_getAvailableName_nid_postfix(index);
        Expect(name != nullptr && *name != 0, "every available converter has a name");
        names.insert(name);
    }
    Expect(static_cast<std::int32_t>(names.size()) == count, "available converters are distinct");
    Expect(names.count("UTF-8") && names.count("ISO-8859-1") && names.count("US-ASCII") && names.count("UTF-16"), "the built-in converters are available");
    Expect(ucnv_countAvailable_nid_postfix() == count, "the count is stable");
    Expect(ucnv_getAvailableName_nid_postfix(count) == nullptr, "the count as an index returns null");
    Expect(ucnv_getAvailableName_nid_postfix(-1) == nullptr, "a negative index returns null");
    Expect(ucnv_getAvailableName_nid_postfix(0x10000) == nullptr, "an index above 0xffff returns null");
    std::printf("%d converters available\n", count);
}
