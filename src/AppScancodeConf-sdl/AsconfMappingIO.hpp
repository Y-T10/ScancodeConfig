#pragma once

#include <filesystem>

#include "CsmViewer.hpp"

namespace AppSacnConf {
    /**
     * @brief ファイルからマッピングをインポートする
     * @param filepath ファイルパス
     * @return 読み取ったマッピング
     */
    const CompScanMap::MappingList ImportMapping(const std::filesystem::path& filepath) noexcept;

    /**
     * @brief マッピングをエクスポートする
     * @param filepath ファイルパス
     * @param mapping マッピング
     */
    void ExportMapping(const std::filesystem::path& filepath, const CompScanMap::MappingList& mapping) noexcept;
}