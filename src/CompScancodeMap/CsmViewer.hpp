#pragma  once

#include <cstdint>
#include <type_traits>
#include <vector>

namespace CompScanMap {
    /// スキャンコード
    using Scancode =uint16_t;

    /// スキャンコードの対応
    /// コーデック処理を容易にするため、メンバの順番は入れ替えないこと．
    struct ScanMapping {
        Scancode to;
        Scancode from;
    };
    static_assert(std::has_unique_object_representations_v<ScanMapping>);

    /// Scancodeの変換リスト
    using MappingList = std::vector<ScanMapping>;
}