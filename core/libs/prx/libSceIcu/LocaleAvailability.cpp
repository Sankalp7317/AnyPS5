#include "prx/libSceIcu/LocaleAvailability.hpp"
#include "prx/libc/include/General.hpp"
#include "unicode/uloc.h"

#undef uloc_countAvailable
#undef uloc_getAvailable

extern "C" {

std::int32_t APS5_VABI uloc_countAvailable() {
    return uloc_countAvailable_78();
}

const char* APS5_VABI uloc_getAvailable(std::int32_t n) {
    const auto count = uloc_countAvailable_78();
    if (n < 0 || n == count) APS5_INVALID_ARG_EX;
    return uloc_getAvailable_78(n);
}

}
