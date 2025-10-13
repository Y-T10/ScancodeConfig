#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "challenger/challenger_memory.hpp"
#include "challenger/challenger_render.hpp"
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <fontconfig/fontconfig.h>
#include <tuple>

#include "AsconfMappingWindow.hpp"
#include "AsconfRegistry.hpp"

#include "Fcpp.hpp"
#include "SDL3_ttf/SDL_ttf.h"

using namespace challenger;

const std::tuple<int, int> GetRenderAreaSize(const Renderer& renderer) noexcept {
    int w, h;
    SDL_GetRenderOutputSize(renderer.get(), &w, &h);
    return {w, h};
}

int GUIMain() {
    // ウィンドウとレンダラを作成
    const auto MainWindow = Create<SDL_CreateWindow, SDL_DestroyWindow>(
        "Scancode Configure", 300, 300, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    const auto WindowRenderer = Create<SDL_CreateRenderer, SDL_DestroyRenderer>(MainWindow.get(), nullptr);

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

    auto configWindow = AppSacnConf::ConfigWindow(AppSacnConf::ReadScancodeMap());

    while (true) {
        // イベント処理
        {
            SDL_Event event;
            SDL_WaitEvent(&event);
            if (event.type == SDL_EVENT_QUIT) {
                break;
            }
        }

        {
            const auto [w, h] = GetRenderAreaSize(WindowRenderer);
            configWindow.show(SDL_Rect{.x = 0, .y = 0, .w =w, .h = h});

            configWindow.handleOperations(MainWindow);
        }

        SDL_SetRenderDrawColor(WindowRenderer.get(), 0, 0, 0, 0);
        SDL_RenderClear(WindowRenderer.get());
        SDL_RenderPresent(WindowRenderer.get());
    }

    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    // SDLのサブシステムを立ち上げる
    if(!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO)) {
        return EXIT_FAILURE;
    }

    // Fontconfigの初期化
    static_assert(!!FcTrue);
    static_assert(!(!!FcFalse));
    if (!FcInit()) {
        return EXIT_FAILURE;
    }

    if (!TTF_Init()) {
        return EXIT_FAILURE;
    };

    const auto Resutlt = GUIMain();

    TTF_Quit();

    FcFini();

    // SDLのサブシステムを閉じる
    SDL_Quit();

    return Resutlt;
}