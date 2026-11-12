#include "prx/libSceIcu/libSceIcu.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <iterator>
#include <vector>

namespace {

constexpr std::int32_t IllegalArgument = 1;
constexpr std::int32_t InvalidFormat = 3;
constexpr std::int32_t FileAccess = 4;
constexpr std::int32_t UsingDefault = -127;

struct Info {
    std::uint16_t size = 0;
    std::uint16_t reservedWord = 0x5a5a;
    std::uint8_t isBigEndian = 0x5a;
    std::uint8_t charsetFamily = 0x5a;
    std::uint8_t sizeofUChar = 0x5a;
    std::uint8_t reservedByte = 0x5a;
    std::array<std::uint8_t, 4> dataFormat{0x5a, 0x5a, 0x5a, 0x5a};
    std::array<std::uint8_t, 4> formatVersion{0x5a, 0x5a, 0x5a, 0x5a};
    std::array<std::uint8_t, 4> dataVersion{0x5a, 0x5a, 0x5a, 0x5a};
};

static_assert(sizeof(Info) == 20);

void Expect(bool condition, const char* what) {
    if (condition) return;
    std::fprintf(stderr, "data access: %s\n", what);
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

bool IsNormalization(UDataMemory* data) {
    Info info;
    info.size = sizeof(Info);
    udata_getInfo_nid_postfix(data, &info);
    return info.size == sizeof(Info) && info.isBigEndian == 0 && info.charsetFamily == 0 && info.sizeofUChar == 2 &&
           std::memcmp(info.dataFormat.data(), "Nrm2", 4) == 0;
}

void OpenAndRead() {
    std::int32_t status = 0;
    auto* data = udata_open_nid_postfix(nullptr, "nrm", "nfkc", &status);
    Expect(data != nullptr && status == 0, "nfkc.nrm opens from the built-in package");
    Expect(IsNormalization(data), "nfkc.nrm reports the Nrm2 format");
    Expect(udata_getMemory_nid_postfix(data) != nullptr, "nfkc.nrm has memory");
    Expect(udata_getLength_nid_postfix(data) > 0, "nfkc.nrm has a length");

    Info partial;
    partial.size = 8;
    udata_getInfo_nid_postfix(data, &partial);
    Expect(partial.size == 8 && partial.sizeofUChar == 2 && partial.dataFormat[0] == 0x5a, "a short info gets only its own bytes");

    udata_close_nid_postfix(data);
    Expect(Throws([&] { udata_close_nid_postfix(data); }), "closing twice throws");
    Expect(Throws([&] { udata_getMemory_nid_postfix(data); }), "a closed handle throws");
}

void Errors() {
    std::int32_t status = 0;
    Expect(udata_open_nid_postfix(nullptr, "nrm", "no-such-item", &status) == nullptr && status == FileAccess, "a missing item reports a file access error");
    status = 0;
    Expect(udata_open_nid_postfix(nullptr, "nrm", "", &status) == nullptr && status == IllegalArgument, "an empty name is an illegal argument");
    for (const std::int32_t incoming : {1, 7, 15, 27}) {
        status = incoming;
        Expect(udata_open_nid_postfix(nullptr, "nrm", "nfkc", &status) == nullptr && status == incoming, "failing status opens nothing");
        status = incoming;
        udata_setAppData_nid_postfix("anyps5-failing", nullptr, &status);
        Expect(status == incoming, "failing status registers nothing");
    }
    Expect(Throws([] { udata_open_nid_postfix(nullptr, "nrm", "nfkc", nullptr); }), "null status throws");

    Expect(udata_getMemory_nid_postfix(nullptr) == nullptr, "null data has no memory");
    Expect(udata_getLength_nid_postfix(nullptr) == -1, "null data has no length");
    Info info;
    info.size = sizeof(Info);
    udata_getInfo_nid_postfix(nullptr, &info);
    Expect(info.size == 0, "null data reports an empty info");
    udata_close_nid_postfix(nullptr);

    alignas(16) std::array<std::uint8_t, 64> foreign{};
    Expect(Throws([&] { udata_close_nid_postfix(reinterpret_cast<UDataMemory*>(foreign.data())); }), "an unknown handle throws");
}

void AppData() {
    std::vector<std::uint8_t> garbage(64, 0);
    std::int32_t status = 0;
    udata_setAppData_nid_postfix("anyps5-garbage", garbage.data(), &status);
    Expect(status == InvalidFormat, "garbage is not common data");

    std::ifstream input(ANYPS5_ICU_PACKAGED_DATA, std::ios::binary);
    Expect(input.good(), "packaged data file");
    const std::vector<char> bytes{std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
    static std::vector<std::max_align_t> aligned;
    aligned.assign((bytes.size() + sizeof(std::max_align_t) - 1) / sizeof(std::max_align_t), {});
    std::memcpy(aligned.data(), bytes.data(), bytes.size());

    status = 0;
    udata_setAppData_nid_postfix("anyps5pkg", aligned.data(), &status);
    Expect(status == 0, "the packaged data registers as app data");
    udata_setAppData_nid_postfix("anyps5pkg", aligned.data(), &status);
    Expect(status == UsingDefault, "a second registration of the name reports the default warning");

    status = 0;
    udata_setAppData_nid_postfix("anyps5-null", nullptr, &status);
    Expect(status == IllegalArgument, "null data is an illegal argument");
}

}

int main() {
    OpenAndRead();
    Errors();
    AppData();
}
