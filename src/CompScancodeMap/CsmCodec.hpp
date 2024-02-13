#pragma once

#include <optional>
#include <cstdint>
#include <vector>

#include "CsmViewer.hpp"

namespace CompScanMap {
    /**
     * @brief バイナリ列から変換表を取り出す．
     * @param bin `Scancode Map`のバイナリ列．
     * @return 失敗時はnulloptを、成功時はオブジェクトを返す．
     */
    const std::optional<MappingList> DecodeScancodeMap(const std::vector<uint8_t>& bin) noexcept;

    /**
     * @brief 変換表からバイナリ列を生成する
     * @param map 変換表
     * @return 失敗時はnulloptを、成功時はオブジェクトを返す．
     */
    const std::optional<std::vector<uint8_t>> EncodeScancodeMap(const MappingList& map) noexcept;
}