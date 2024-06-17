#pragma once

#include "challenger/challenger_video.hpp"
#include <filesystem>
#include <optional>

namespace AppSacnConf {
    /**
     * @brief ファイル読み取りくダイアログを表示する．
     * @param parent ダイアログの親ウィンドウ
     * @return 無効値かファイルパス
     */
    const std::optional<std::filesystem::path>
    ShowOpenDialog(const challenger::Window& parent) noexcept;

    /**
     * @brief ファイル保存ダイアログを表示する．
     * @param parent ダイアログの親ウィンドウ
     * @return 無効値かファイルパス
     */
    const std::optional<std::filesystem::path>
    ShowSaveDialog(const challenger::Window& parent) noexcept;
}