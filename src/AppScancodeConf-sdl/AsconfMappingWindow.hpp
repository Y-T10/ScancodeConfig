#pragma once

#include "challenger/challenger_video.hpp"

#include "SDL3/SDL_rect.h"
#include "CsmViewer.hpp"
#include <vector>

namespace AppSacnConf {
    /// コンフィグウィンドウ
    struct ConfigWindow {
        struct ConfTableRow {
            CompScanMap::ScanMapping map;
            bool selected;
        };

        // マッピングデータの型
        using container_type = std::vector<ConfTableRow>;

        explicit ConfigWindow(const CompScanMap::MappingList& list) noexcept;
        ConfigWindow(const ConfigWindow& rval) noexcept = delete;
        ConfigWindow(ConfigWindow& rval) noexcept;
        ConfigWindow() noexcept;

        // ウィンドウを描画する
        void show(const SDL_Rect drawArea) noexcept;

        /**
         * @brief ウィンドウへの操作を処理する
         */
        void handleOperations(const challenger::Window& MainWindow) noexcept;
        
        bool importMapping;
        bool exportMapping;
        bool loadMapping;
        bool applyMapping;

        container_type mapping;

    private:
        // 描画下請関数
        void showMenuBar() noexcept;
        void showTable() noexcept;
    };
}