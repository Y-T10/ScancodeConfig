#include "AsconfMappingWindow.hpp"

#include <algorithm>
#include <iterator>
#include <cstddef>
#include <cstdlib>
#include <format>

#include "imgui.h"

#include "AsconfMappingWindow.hpp"
#include "AsconfMappingIO.hpp"
#include "AsconfRegistry.hpp"
#include "AsconfDialog.hpp"

namespace {
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

    const auto ToConfWindowContainer(const CompScanMap::MappingList& list) noexcept {
        using namespace AppSacnConf;

        ConfigWindow::container_type rows = {};
        std::transform(
            list.begin(), list.end(),
            std::back_inserter(rows),
            [](const auto& mapping) {
                return ConfigWindow::ConfTableRow{.map = mapping, .selected = false};
            }
        );
        return rows;
    }

    const auto ToMappingList(const AppSacnConf::ConfigWindow::container_type& rows) noexcept {
        using namespace CompScanMap;

        MappingList list = {};
        std::transform(
            rows.begin(), rows.end(),
            std::back_inserter(list),
            [](const auto& row) {
                return row.map;
            }
        );
        return list;
    }
}

namespace AppSacnConf {
    void ConfigWindow::show(const SDL_Rect drawArea) noexcept {
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

    void ConfigWindow::handleOperations(const challenger::Window& MainWindow) noexcept {
        // レジストリから値を取り出す．
        if (loadMapping) {
            mapping = ToConfWindowContainer(AppSacnConf::ReadScancodeMap());
            loadMapping = false;
        }

        // レジストリに値を書き込む．
        if (applyMapping) {
            AppSacnConf::WriteScancodeMap(ToMappingList(mapping));
            applyMapping = false;
        }

        if (importMapping) {
            importMapping = false;
            
            const auto Path = AppSacnConf::ShowOpenDialog(MainWindow);
            if (Path) {
                mapping = ToConfWindowContainer(AppSacnConf::ImportMapping(*Path));
            }
        }

        if (exportMapping) {
            exportMapping = false;
            
            const auto Path = AppSacnConf::ShowSaveDialog(MainWindow);
            if (Path && (!Path->empty())) {
                AppSacnConf::ExportMapping(*Path, ToMappingList(mapping));
            }
        }
    };

    void ConfigWindow::showMenuBar() noexcept {
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

    void ConfigWindow::showTable() noexcept {
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
            ImGui::Text("%s", GenerateText(map.to, GetNameTo).c_str());
        }

        ImGui::EndTable();
    }
}