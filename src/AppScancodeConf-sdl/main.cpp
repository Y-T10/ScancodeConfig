#include "SDL_events.h"
#include "SDL_init.h"
#include "SDL_hints.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include "challenger/challenger_memory.hpp"
#include "challenger/challenger_video.hpp"
#include "challenger/challenger_render.hpp"
#include <cstddef>
#include <cstdlib>
#include <tuple>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include "AsconfMappingWindow.hpp"
#include "AsconfMappingIO.hpp"
#include "AsconfRegistry.hpp"
#include "AsconfDialog.hpp"


using namespace challenger;

const std::tuple<int, int> GetRenderAreaSize(const Renderer& renderer) noexcept {
    int w, h;
    SDL_GetRenderOutputSize(renderer.get(), &w, &h);
    return {w, h};
}

int main(int argc, char* argv[]) {
    // SDLのサブシステムを立ち上げる
    if(!!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO)) {
        return EXIT_FAILURE;
    }
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

    // ウィンドウとレンダラを作成
    const auto MainWindow = Create<Window, SDL_CreateWindow>("Scancode Configure", 300, 300, SDL_WINDOW_OPENGL);
    const auto WindowRenderer = Create<Renderer, SDL_CreateRenderer>(MainWindow.get(), nullptr, SDL_RENDERER_ACCELERATED);

    // ImGuiのセットアップ
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    ImGui::GetIO().Fonts->AddFontFromFileTTF("C:/Windows/Fonts/YUGOTHL.TTC", 18.0f, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesJapanese());

    // ImGuiのスタイルをシステムのテーマに合わせる
    if (SDL_GetSystemTheme() == SDL_SystemTheme::SDL_SYSTEM_THEME_DARK) {
        ImGui::StyleColorsDark();
    } else {
        ImGui::StyleColorsLight();
    }

    // ImGuiSDL3向けの初期化を行う
    ImGui_ImplSDL3_InitForSDLRenderer(MainWindow.get(), WindowRenderer.get());
    ImGui_ImplSDLRenderer3_Init(WindowRenderer.get());

    AppSacnConf::ConfigWindow configWindow = {
        .importMapping = false,
        .exportMapping = false,
        .loadMapping = false,
        .applyMapping = false,
        .mapping = AppSacnConf::ReadScancodeMap()
    };

    while (true) {
        // イベント処理
        {
            SDL_Event event;
            SDL_WaitEvent(&event);
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT) {
                break;
            }
        }

        // ImGuiのウィンドウ処理
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        {
            const auto [w, h] = GetRenderAreaSize(WindowRenderer);
            configWindow.show(SDL_Rect{.x = 0, .y = 0, .w =w, .h = h});

            // レジストリから値を取り出す．
            if (configWindow.loadMapping) {
                configWindow.mapping = AppSacnConf::ReadScancodeMap();
                configWindow.loadMapping = false;
            }

            // レジストリに値を書き込む．
            if (configWindow.applyMapping) {
                AppSacnConf::WriteScancodeMap(configWindow.mapping);
                configWindow.applyMapping = false;
            }

            if (configWindow.importMapping) {
                configWindow.importMapping = false;
                
                const auto Path = AppSacnConf::ShowOpenDialog(MainWindow);
                if (Path) {
                    configWindow.mapping = AppSacnConf::ImportMapping(*Path);
                }
            }

            if (configWindow.exportMapping) {
                configWindow.exportMapping = false;
                
                const auto Path = AppSacnConf::ShowSaveDialog(MainWindow);
                if (Path) {
                    AppSacnConf::ExportMapping(*Path, configWindow.mapping);
                }
            }
        }

        // 描画処理
        ImGui::Render();
        SDL_SetRenderDrawColor(WindowRenderer.get(), 0, 0, 0, 0);
        SDL_RenderClear(WindowRenderer.get());
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(WindowRenderer.get());
    }


    // ImGuiを閉じる
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    // SDLのサブシステムを閉じる
    SDL_Quit();

    return EXIT_SUCCESS;
}