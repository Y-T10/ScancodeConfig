#pragma once

#include <optional>

namespace CompReg {
    /**
     * @brief 管理者権限が利用できるかを調べる
     * @return true 利用可能
     * @return false 利用不可能
     */
    const std::optional<bool> IsElevated() noexcept;

    /**
     * @brief レジストリ書き込みに必要な権限を有効にする
     * @return true 権限を有効にした
     * @return false 権限を無効にした
     */
    const bool EnableWritingRegPriv() noexcept;

    /**
     * @brief レジストリ書き込みに必要な権限を無効にする
     */
    void DisableWritingRegPriv() noexcept;
}