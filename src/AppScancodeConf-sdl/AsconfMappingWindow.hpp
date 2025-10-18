#pragma once

#include "challenger/challenger_video.hpp"

#include "SDL3/SDL_rect.h"
#include "CsmViewer.hpp"

#include "challenger/challenger_render.hpp"
#include "TGUI/Backend/SDL-Renderer.hpp"

#include <vector>

namespace AppSacnConf {
    /// Guiにムーブコンストラクタを追加した版
    struct movable_gui : public tgui::Gui {
        movable_gui() : tgui::Gui(){};
        movable_gui(SDL_Window* window, SDL_Renderer* renderer) noexcept : tgui::Gui(window, renderer){};
        movable_gui(const movable_gui& rval) = delete;
        movable_gui(movable_gui&& rval) noexcept{
            *this = std::move(std::forward<movable_gui>(rval));
        }

        movable_gui& operator=(const movable_gui& rval) noexcept = delete;
        movable_gui& operator=(movable_gui&& rval) noexcept {
            if (this == &rval) {
                return *this;
            }

            this->m_renderer = std::move(rval.m_renderer); rval.m_renderer = nullptr;

            this->m_window = std::move(rval.m_window); rval.m_window = nullptr;
            this->m_dpiScale = std::move(rval.m_dpiScale); rval.m_dpiScale = 1;
            this->m_textInputStarted = std::move(rval.m_textInputStarted); rval.m_textInputStarted = false;
            this->m_textInputRect = std::move(rval.m_textInputRect); rval.m_textInputRect = {};
            this->m_touchFirstFingerDown = std::move(rval.m_touchFirstFingerDown); rval.m_touchFirstFingerDown = false;
            this->m_touchFirstFingerId = std::move(rval.m_touchFirstFingerId); rval.m_touchFirstFingerId = 0;
            this->m_touchFirstFingerTouchId = std::move(rval.m_touchFirstFingerTouchId); rval.m_touchFirstFingerTouchId = 0;

            this->onViewChange = std::move(rval.onViewChange);
            this->onWindowFocus = std::move(rval.onWindowFocus);
            this->onWindowUnfocus = std::move(rval.onWindowUnfocus);

            this->m_lastUpdateTime = std::move(rval.m_lastUpdateTime);
            this->m_windowFocused = std::move(rval.m_windowFocused);

            this->m_container = std::move(rval.m_container);

            this->m_visibleToolTip = std::move(rval.m_visibleToolTip);
            this->m_tooltipTime = std::move(rval.m_tooltipTime);
            this->m_tooltipPossible = std::move(rval.m_tooltipPossible);
            this->m_toolTipRelativePos = std::move(rval.m_toolTipRelativePos);
            this->m_lastMousePos = std::move(rval.m_lastMousePos);
            this->m_twoFingerScroll = std::move(rval.m_twoFingerScroll);

            this->m_framebufferSize = std::move(rval.m_framebufferSize);
            this->m_viewport = std::move(rval.m_viewport);
            this->m_view = std::move(rval.m_view);
            this->m_lastView = std::move(rval.m_lastView);

            this->m_drawUpdatesTime = std::move(rval.m_drawUpdatesTime);
            this->m_tabKeyUsageEnabled = std::move(rval.m_tabKeyUsageEnabled);
            this->m_keyboardNavigationEnabled = std::move(rval.m_keyboardNavigationEnabled);

            this->m_requestedMouseCursor = std::move(rval.m_requestedMouseCursor);
            this->m_overrideMouseCursors = std::move(rval.m_overrideMouseCursors);
            
            this->m_backendRenderTarget = std::move(rval.m_backendRenderTarget);
            
            return *this;
        };
    };

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
    };
}