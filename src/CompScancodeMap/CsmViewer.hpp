#pragma  once

#include <cstdint>
#include <vector>

namespace CompScanMap {
    /// スキャンコード
    using Scancode =uint16_t;

    /// スキャンコードの対応
    struct ConvertPair {
        Scancode from;
        Scancode to;
    };

    /// Scancodeの変換表
    using ConvertMap = std::vector<ConvertPair>;
}