#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_hints.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "challenger/challenger_memory.hpp"
#include "challenger/challenger_video.hpp"
#include "challenger/challenger_render.hpp"
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <fontconfig/fontconfig.h>
#include <string>
#include <tuple>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include "AsconfMappingWindow.hpp"
#include "AsconfRegistry.hpp"

#include "Fcpp.hpp"

using namespace challenger;

const std::tuple<int, int> GetRenderAreaSize(const Renderer& renderer) noexcept {
    int w, h;
    SDL_GetRenderOutputSize(renderer.get(), &w, &h);
    return {w, h};
}

void SetImGUITheme() noexcept {
    const auto Theme = SDL_GetSystemTheme();
    if (Theme == SDL_SystemTheme::SDL_SYSTEM_THEME_DARK) {
        ImGui::StyleColorsDark();
        return;
    }
    if (Theme == SDL_SystemTheme::SDL_SYSTEM_THEME_LIGHT) {
        ImGui::StyleColorsLight();
        return;
    }
    // TODO: ライトやダークではない場合に対応する
};

int GUIMain() {
    // ウィンドウとレンダラを作成
    const auto MainWindow = Create<Window, SDL_CreateWindow>("Scancode Configure", 400, 300, SDL_WINDOW_OPENGL);
    const auto WindowRenderer = Create<Renderer, SDL_CreateRenderer>(MainWindow.get(), nullptr, SDL_RENDERER_ACCELERATED);

    // 使用する日本語フォントの検索パターンを作る
    // TODO: FcChar8ではなくcharを代入できるようにする
    // TODO: FcPatternAdd(Double/Integer/Bool/...)を呼び出すようにする
    const auto JPPattern = Fcpp::CreatePattern({
        {FC_FAMILY, (const FcChar8*)"Monospace"},
        {FC_FAMILYLANG, (const FcChar8*)"ja"},
        {FC_LANG, (const FcChar8*)"ja"},
    });
    FcPatternAddDouble(JPPattern.get(), FC_SIZE, 18.0f);

    // 検索パターンを満たすフォントをシステムから取得する
    const auto JPFontPath = Fcpp::SearchFont(JPPattern);
    if (!std::filesystem::exists(JPFontPath)) {
        return EXIT_FAILURE;
    }

    // ImGuiのセットアップ
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    ImGui::GetIO().Fonts->AddFontFromFileTTF(JPFontPath.string().c_str(), 18.0f, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesJapanese());

    // ImGuiのスタイルをシステムのテーマに合わせる
    SetImGUITheme();

    // ImGuiSDL3向けの初期化を行う
    ImGui_ImplSDL3_InitForSDLRenderer(MainWindow.get(), WindowRenderer.get());
    ImGui_ImplSDLRenderer3_Init(WindowRenderer.get());

    auto configWindow = AppSacnConf::ConfigWindow(AppSacnConf::ReadScancodeMap());

    while (true) {
        // イベント処理
        {
            SDL_Event event;
            SDL_WaitEvent(&event);
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT) {
                break;
            }
            if (event.type == SDL_EVENT_SYSTEM_THEME_CHANGED) {
                SetImGUITheme();
            }
        }

        // ImGuiのウィンドウ処理
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        {
            const auto [w, h] = GetRenderAreaSize(WindowRenderer);
            configWindow.show(SDL_Rect{.x = 0, .y = 0, .w =w, .h = h});

            configWindow.handleOperations(MainWindow);
        }

        // 描画処理
        ImGui::Render();
        SDL_SetRenderDrawColor(WindowRenderer.get(), 0, 0, 0, 0);
        SDL_RenderClear(WindowRenderer.get());
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), WindowRenderer.get());
        SDL_RenderPresent(WindowRenderer.get());
    }


    // ImGuiを閉じる
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    // SDLのサブシステムを立ち上げる
    if(!!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO)) {
        return EXIT_FAILURE;
    }
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

    // Fontconfigの初期化
    static_assert(!!FcTrue);
    static_assert(!(!!FcFalse));
    if (!FcInit()) {
        return EXIT_FAILURE;
    }

    const auto Resutlt = GUIMain();

    FcFini();

    // SDLのサブシステムを閉じる
    SDL_Quit();

    return Resutlt;
}