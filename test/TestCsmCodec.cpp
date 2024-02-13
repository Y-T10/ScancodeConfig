#include "CsmCodec.hpp"
#include "CsmViewer.hpp"
#include <algorithm>
#include <cstdint>
#include <vector>

using namespace CompScanMap;

// バイナリと表のペア
struct BinMapPair {
    std::vector<uint8_t> bin;
    ConvertMap map;
};

const bool TestDecode(const BinMapPair& pair) noexcept {
    const auto result = DecodeScancodeMap(pair.bin);
    if(result) {
        return false;
    }
    return std::ranges::equal(result.value(), pair.map, [](const ConvertPair& result, const ConvertPair& pairMap)-> bool{
        return result.to == pairMap.to && result.from == pairMap.from;
    });
}

int main() {
    // 以下のURLに掲載されている例についてテストを行う
    // https://learn.microsoft.com/en-us/windows-hardware/drivers/hid/keyboard-and-mouse-class-drivers#scan-code-mapper-for-keyboards

    const BinMapPair Empty = {
        .bin = { 0x00, 0x00, 0x00, 0x00,
                 0x00, 0x00, 0x00, 0x00,
                 0x01, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00 },
        .map = {}
    };
    const BinMapPair Example1 = {
        .bin = { 0x00, 0x00, 0x00, 0x00,
                 0x00, 0x00, 0x00, 0x00,
                 0x03, 0x00, 0x00, 0x00,
                 0x3a, 0x00, 0x1d, 0x00,
                 0x1d, 0x00, 0x3a, 0x00,
                 0x00, 0x00, 0x00, 0x00,
        },
        .map = {ConvertPair{.to = 0x003a, .from = 0x001d}, ConvertPair{.to = 0x001d, .from = 0x003a}}
    };
    const BinMapPair Example2 = {
        .bin = { 0x00, 0x00, 0x00, 0x00,
                 0x00, 0x00, 0x00, 0x00,
                 0x03, 0x00, 0x00, 0x00,
                 0x00, 0x00, 0x1d, 0xe0,
                 0x20, 0xe0, 0x38, 0xe0,
                 0x00, 0x00, 0x00, 0x00,
        },
        .map = {ConvertPair{.to = 0x0000, .from = 0x001d}, ConvertPair{.to = 0xe020, .from = 0xe038}}
    };
    return 0;
}