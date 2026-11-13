#include "prx/libSceIcu/ConverterAvailability.hpp"
#include "unicode/ucnv.h"

extern "C" {

std::int32_t APS5_VABI ucnv_countAvailable_nid_postfix() {
    return ucnv_countAvailable();
}

const char* APS5_VABI ucnv_getAvailableName_nid_postfix(std::int32_t n) {
    return ucnv_getAvailableName(n);
}

}
