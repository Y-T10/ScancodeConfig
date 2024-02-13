#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

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
    CHECK(result.has_value());
    if (!result.has_value()) {
        return false;
    }
    return std::ranges::equal(result.value(), pair.map, [](const ConvertPair& result, const ConvertPair& pairMap)-> bool{
        return result.to == pairMap.to && result.from == pairMap.from;
    });
}

void TestEncode(const BinMapPair& pair) noexcept {
    const auto result = EncodeScancodeMap(pair.map);
    CHECK(result.has_value());
    if (!result.has_value()) {
        return;
    }
    CHECK(std::ranges::equal(result.value(), pair.bin));
}

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
    .map = {ConvertPair{.to = 0x0000, .from = 0xe01d}, ConvertPair{.to = 0xe020, .from = 0xe038}}
};

TEST_CASE("Decoding `Scancode Map` binary") {
    CHECK(TestDecode(Empty));
    CHECK(TestDecode(Example1));
    CHECK(TestDecode(Example2));
};

TEST_CASE("Encoding ConvertMap to binary") {
    TestEncode(Empty);
    TestEncode(Example1);
    TestEncode(Example2);
}

#define ScanMapVerBin   0x00, 0x00, 0x00, 0x00
#define ScanMapFlagsBin 0x00, 0x00, 0x00, 0x00
#define ScanMapEmptyMap 0x01, 0x00, 0x00, 0x00
#define ScanMapSentiBin 0x00, 0x00, 0x00, 0x00

TEST_CASE("Decoding broken binary") {
    const std::vector<uint8_t> BinBrokVersion = {
        0x01, 0x00, 0x00, 0x00,
        ScanMapFlagsBin,
        ScanMapEmptyMap,
        ScanMapSentiBin
    };
    const std::vector<uint8_t> BinBrokFlags = {
        ScanMapVerBin,
        0x01, 0x00, 0x00, 0x00,
        ScanMapEmptyMap,
        ScanMapSentiBin
    };
    const std::vector<uint8_t> BinBrokMap = {
        ScanMapVerBin,
        ScanMapFlagsBin,
        0x02, 0x00, 0x00, 0x00,
        ScanMapSentiBin
    };

    const std::vector<uint8_t> BinBrokMapTail = {
        ScanMapVerBin,
        ScanMapFlagsBin,
        ScanMapEmptyMap,
    };

    const std::vector<uint8_t> BinShifted = {
        ScanMapVerBin,
        ScanMapFlagsBin,
        ScanMapEmptyMap,
        
    };

    CHECK_FALSE(DecodeScancodeMap(BinBrokVersion));
    CHECK_FALSE(DecodeScancodeMap(BinBrokFlags));
    CHECK_FALSE(DecodeScancodeMap(BinBrokMap));
    CHECK_FALSE(DecodeScancodeMap(BinBrokMapTail));
};