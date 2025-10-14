#pragma once

#include "challenger/challenger_video.hpp"

#include "SDL3/SDL_rect.h"
#include "CsmViewer.hpp"

#include "challenger/challenger_render.hpp"
#include "TGUI/Backend/SDL-Renderer.hpp"

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

        explicit ConfigWindow(const CompScanMap::MappingList& list, const challenger::Renderer& renderer) noexcept;
        ConfigWindow(const ConfigWindow& rval) noexcept = delete;
        ConfigWindow(ConfigWindow&& rval) noexcept = delete;
        ConfigWindow() noexcept = delete;

        ConfigWindow& operator=(const ConfigWindow& rval) noexcept = delete;
        ConfigWindow& operator=(ConfigWindow&& rval) noexcept = delete;

        // イベントを処理する
        void handle_event(const SDL_Event& e) noexcept;

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
        tgui::Gui gui;

        container_type mapping;

    private:
        // 描画下請関数
        void showMenuBar() noexcept;
        void showTable() noexcept;
    };
}