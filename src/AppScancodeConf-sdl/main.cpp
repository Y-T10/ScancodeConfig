#include "SDL_init.h"
#include "SDL_hints.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include "challenger/challenger_memory.hpp"
#include "challenger/challenger_video.hpp"
#include "challenger/challenger_render.hpp"
#include <cstddef>
#include <cstdlib>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

using namespace challenger;

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


    // ImGuiを閉じる
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    // SDLのサブシステムを閉じる
    SDL_Quit();

    return EXIT_SUCCESS;
}