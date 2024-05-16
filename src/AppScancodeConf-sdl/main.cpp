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
#include <format>
#include <tuple>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include "CsmViewer.hpp"
#include "CregHandler.hpp"
#include "CsmCodec.hpp"

using namespace challenger;

const std::tuple<int, int> GetRenderAreaSize(const Renderer& renderer) noexcept {
    int w, h;
    SDL_GetRenderOutputSize(renderer.get(), &w, &h);
    return {w, h};
}

const CompScanMap::MappingList ReadScancodeMap() noexcept {
    const auto RegKey = CompReg::OpenRegKey(
        HKEY_LOCAL_MACHINE,
        TEXT("SYSTEM\\CurrentControlSet\\Control\\Keyboard Layout"),
        KEY_READ
    );
    if (!RegKey) {
        return {};
    }
    const auto RegValue = ReadKeyValueBin(RegKey, TEXT("Scancode Map"));
    if (!RegValue) {
        return {};
    }
    const auto MappingList = CompScanMap::DecodeScancodeMap(*RegValue);
    return MappingList.has_value() ? *MappingList : CompScanMap::MappingList{};
}

static constexpr auto TextNotApplicable = "N/A";

const std::string GetNameFrom (const CompScanMap::Scancode code) {
    // TODO: エラーコードとキーの番号を標準エラーに書き込む
    return CompScanMap::WindowsScancodeName(code).value_or(TextNotApplicable);
};

const std::string GetNameTo(const CompScanMap::Scancode code) {
    // TODO: エラーコードとキーの番号を標準エラーに書き込む
    const std::string KeyName = CompScanMap::KeyboardKeyName(code).value_or(TextNotApplicable);
    if (!KeyName.empty()) {
        return KeyName;
    }
    return CompScanMap::WindowsScancodeName(code).value_or(TextNotApplicable);
};

template <class F>
const std::string GenerateText(const CompScanMap::Scancode code, const F &func) {
    static_assert(std::is_invocable_r_v<const std::string, F, CompScanMap::Scancode>);
    return std::format("{:s} ({:#x})", func(code), code); 
};

struct ConfigWindow {
    using container_type = CompScanMap::MappingList;

    void show(const SDL_Rect drawArea) noexcept {
        // ウィンドウの設定
        const ImGuiWindowFlags WindowFlags = 
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_MenuBar;
        ImGui::Begin("Hello, world!", NULL, WindowFlags);

        // ウィンドウを描画範囲全体にする
        ImGui::SetWindowSize(ImVec2(drawArea.w, drawArea.h));
        ImGui::SetWindowPos(ImVec2(drawArea.x, drawArea.y));

        showMenuBar();
        showTable();

        ImGui::End();
    }

    bool importMapping;
    bool exportMapping;
    bool loadMapping;
    bool applyMapping;

    container_type mapping;

    private:

    void showMenuBar() noexcept {
        if (!ImGui::BeginMenuBar()) {
            return;
        }

        // メニューを追加する
        if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("Import Mapping", NULL, &importMapping);
            ImGui::MenuItem("Export Mapping", NULL, &exportMapping);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Registry")) {
            ImGui::MenuItem("Load current mappping", NULL, &loadMapping);
            ImGui::MenuItem("Apply mappping", NULL, &applyMapping);

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    void showTable() noexcept {
        if(!ImGui::BeginTable("mapping_table", 2)) {
            return;
        }

        // ヘッダを設定
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("Mapping From");
        ImGui::TableSetupColumn("Mapping To");
        ImGui::TableHeadersRow();

        // 表を作成する
        for (const auto& map: mapping) {
            // 行を追加
            ImGui::TableNextRow();

            // 行の中身を埋める
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", GenerateText(map.from, GetNameFrom).c_str());
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", GenerateText(map.from, GetNameTo).c_str());
        }

        ImGui::EndTable();
    }
};

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

    ConfigWindow configWindow = {
        .importMapping = false,
        .exportMapping = false,
        .loadMapping = false,
        .applyMapping = false,
        .mapping = ReadScancodeMap()
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