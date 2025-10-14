#include "AsconfMappingWindow.hpp"

#include <algorithm>
#include <iterator>
#include <format>

#include "challenger/challenger_dialog.hpp"
#include "TGUI/Widgets/MenuBar.hpp"
#include "AsconfMappingWindow.hpp"
#include "AsconfMappingIO.hpp"
#include "AsconfRegistry.hpp"

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

    const challenger::FilterList DialogFilters = {
    { "Mapping file", "map" },
    { "All files", "*" }
    };
}

namespace AppSacnConf {
    ConfigWindow::ConfigWindow(const CompScanMap::MappingList& list, const challenger::Renderer& renderer) noexcept:
    importMapping(false),
    exportMapping(false),
    loadMapping(false),
    applyMapping(false),
    gui(SDL_GetRenderWindow(renderer.get()), renderer.get()),
    mapping(ToConfWindowContainer(list)){
        auto menu = tgui::MenuBar::create();

        menu->addMenu("File");
        menu->addMenuItem("Import");
        menu->addMenuItem("Export");
        menu->addMenu("Registry");
        menu->addMenuItem("Load");
        menu->addMenuItem("Apply");

        menu->onMenuItemClick([this](const std::vector<tgui::String>& menu_chain){
            if (menu_chain.size() != 2) {
                return;
            }

            importMapping = false;
            exportMapping = false;
            loadMapping = false;
            applyMapping = false;

            if (menu_chain[0] == "File" && menu_chain[1] == "Import") {
                importMapping = true;
            }
            if (menu_chain[0] == "File" && menu_chain[1] == "Export") {
                exportMapping = true;
            }
            if (menu_chain[0] == "Registry" && menu_chain[1] == "Load") {
                loadMapping = true;
            }
            if (menu_chain[0] == "Registry" && menu_chain[1] == "Apply") {
                applyMapping = true;
            }
        });
        
        gui.add(menu);
    }

    void ConfigWindow::show(const SDL_Rect drawArea) noexcept {
        gui.draw();
    }

    void ConfigWindow::handle_event(const SDL_Event& e) noexcept {
        gui.handleEvent(e);
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

            challenger::ShowOpenFileDialog(MainWindow, [this](const challenger::PathList& list, const int n) mutable {
                if (list.empty() || list.front().empty()) {
                    return;
                }
                mapping = ToConfWindowContainer(AppSacnConf::ImportMapping(list[0]));
            }, DialogFilters, "", false);
        }

        if (exportMapping) {
            exportMapping = false;
            
            challenger::ShowSaveFileDialog(MainWindow, [this](const challenger::PathList& list, const int n) mutable {
                if (list.empty() || list.front().empty()) {
                    return;
                }
                AppSacnConf::ExportMapping(list[0], ToMappingList(mapping));
            }, DialogFilters);
        }
    };

    void ConfigWindow::showMenuBar() noexcept {
    }

    void ConfigWindow::showTable() noexcept {
    }
}