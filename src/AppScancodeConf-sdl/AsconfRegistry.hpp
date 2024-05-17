#pragma once

#include "CsmViewer.hpp"

namespace AppSacnConf {
    /**
     * @brief レジストリから現在の`ScancodeMap`を読み取る
     * @return `ScancodeMap`の値を持つ表．値の取得に失敗した場合は空データ．
     */
    const CompScanMap::MappingList ReadScancodeMap() noexcept;

    /**
     * @brief マッピング情報をレジストリに書き込む
     * @param mapping マッピング
     * @return true 書き込み成功
     * @return false 書き込み失敗
     */
    const bool WriteScancodeMap(const CompScanMap::MappingList& mapping) noexcept;
}