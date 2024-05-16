#pragma once

#include "CsmViewer.hpp"

namespace AppSacnConf {
    /**
     * @brief レジストリから現在の`ScancodeMap`を読み取る
     * @return `ScancodeMap`の値を持つ表．値の取得に失敗した場合は空データ．
     */
    const CompScanMap::MappingList ReadScancodeMap() noexcept;
}