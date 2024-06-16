#pragma once

#include "challenger/challenger_video.hpp"

#include "SDL3/SDL_rect.h"
#include "CsmViewer.hpp"

namespace AppSacnConf {
    /// コンフィグウィンドウ
    struct ConfigWindow {
        // マッピングデータの型
        using container_type = CompScanMap::MappingList;

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