#pragma  once

#include <type_traits>
#include <optional>
#include <cstdint>
#include <string>
#include <vector>

namespace CompScanMap {
    /// スキャンコード
    using Scancode =uint16_t;

    /// スキャンコードの対応
    /// コーデック処理を容易にするため、メンバの順番は入れ替えないこと．
    struct ScanMapping {
        Scancode to;
        Scancode from;

        inline bool operator==(const CompScanMap::ScanMapping& value) const noexcept {
            return from == value.from && to == value.to;
        }
    };
    static_assert(std::has_unique_object_representations_v<ScanMapping>);

    /// Scancodeの変換リスト
    using MappingList = std::vector<ScanMapping>;

    /**
     * @brief スキャンコードに対応するキーの名前を返す
     * @param code スキャンコード
     * @return キー名．nulloptの場合は失敗．
     */
    const std::optional<std::string> ScancodeName(const Scancode code) noexcept;

    /**
     * @brief 現在のキーボードレイアウト上のキー名を返す
     * 
     * @param code Windows スキャンコード
     * @return code に対応するキーの名前．nulloptの場合は失敗．
     */
    const std::optional<std::string> KeyboardKeyName(const Scancode code) noexcept;
}