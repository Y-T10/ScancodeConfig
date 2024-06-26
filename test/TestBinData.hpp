#pragma once

#include <cstdint>
#include <vector>

#define ScanMapVer   0x00, 0x00, 0x00, 0x00
#define ScanMapFlags 0x00, 0x00, 0x00, 0x00
#define ScanEmptyMap 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

const std::vector<uint8_t> EmptyMap ={
    ScanMapVer, ScanMapFlags, ScanEmptyMap
};
const std::vector<uint8_t> ExaMap1 = {
    ScanMapVer, ScanMapFlags,
    0x03, 0x00, 0x00, 0x00,
    0x3a, 0x00, 0x1d, 0x00,
    0x1d, 0x00, 0x3a, 0x00,
    0x00, 0x00, 0x00, 0x00,
};
const std::vector<uint8_t> ExaMap2 = {
    ScanMapVer, ScanMapFlags,
    0x03, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x1d, 0xe0,
    0x20, 0xe0, 0x38, 0xe0,
    0x00, 0x00, 0x00, 0x00,
};