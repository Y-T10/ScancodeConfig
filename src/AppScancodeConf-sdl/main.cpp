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

using namespace challenger;

const std::tuple<int, int> GetRenderAreaSize(const Renderer& renderer) noexcept {
    int w, h;
    SDL_GetRenderOutputSize(renderer.get(), &w, &h);
    return {w, h};
}

void ShowConfigWindow(const SDL_Rect drawArea) noexcept {
    // ウィンドウの設定
    const ImGuiWindowFlags WindowFlags = 
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize;
    ImGui::Begin("Hello, world!", NULL, WindowFlags);

    // ウィンドウを描画範囲全体にする
    ImGui::SetWindowSize(ImVec2(drawArea.w, drawArea.h));
    ImGui::SetWindowPos(ImVec2(drawArea.x, drawArea.y));

    ImGui::End();
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

    // ImGuiのスタイルをシステムのテーマに合わせる
    if (SDL_GetSystemTheme() == SDL_SystemTheme::SDL_SYSTEM_THEME_DARK) {
        ImGui::StyleColorsDark();
    } else {
        ImGui::StyleColorsLight();
    }

    // ImGuiSDL3向けの初期化を行う
    ImGui_ImplSDL3_InitForSDLRenderer(MainWindow.get(), WindowRenderer.get());
    ImGui_ImplSDLRenderer3_Init(WindowRenderer.get());

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