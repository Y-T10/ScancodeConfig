#include "AsconfDialog.hpp"

#include <cstddef>
#include <filesystem>


#include "SDL_dialog.h"

using namespace std::filesystem;

namespace {
    static std::optional<path> SelectedPath = std::nullopt;

    static void DialogCallback(void *userdata, const char * const *filelist, int filter) noexcept {
        // エラーの有無を調べる
        if (filelist == nullptr) {
            // TODO: エラーを表示する
            SelectedPath = std::nullopt;
            return;
        }
        // ファイルが選択されていない or キャンセルした かを調べる
        if (*filelist == nullptr) {
            // TODO: 未選択時の処理を追加
            SelectedPath = path("");
            return;
        }

        // 選択されたパスを代入する
        SelectedPath = path(filelist[0]);
    };

    static const SDL_DialogFileFilter DialogFilters[] = {
    { "All files", "*" },
    { "Import file", "map" },
    { NULL, NULL }
    };
}

namespace AppSacnConf {
    const std::optional<path> ShowOpenDialog(const challenger::Window& parent) noexcept {
        SDL_ShowOpenFileDialog(
            DialogCallback, nullptr,
            parent.get(), DialogFilters,
            NULL, 0
        );

        return SelectedPath;
    };

    const std::optional<path> ShowSaveDialog(const challenger::Window& parent) noexcept {
        SDL_ShowSaveFileDialog(
            DialogCallback, nullptr,
            parent.get(), DialogFilters,
            NULL
        );

        return SelectedPath;
    };
}