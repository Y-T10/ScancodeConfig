#include "AsconfMappingWindow.hpp"

#include <algorithm>
#include <iterator>
#include <format>
#include <array>

#include "challenger/challenger_dialog.hpp"
#include "TGUI/Widgets/ListView.hpp"
#include "TGUI/Widgets/MenuBar.hpp"
#include "TGUI/Layout.hpp"
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

    template <size_t N>
    void RegisterMenu(tgui::MenuBar::Ptr& widget, const char* menu, const std::array<const char*, N>& items) noexcept {
        widget->addMenu(menu);
        for(const auto s: items) {
            widget->addMenuItem(s);
        }
    }

    const char* MSG_Menu_File = "File";
    const auto MSG_Menu_Item_File = std::array<const char*, 2>({"Import", "Export"});
    const char* MSG_Menu_Reg = "Registry";
    const auto MSG_Menu_Item_Reg = std::array<const char*, 2>({"Load", "Apply"});

    const challenger::FilterList DialogFilters = {
        { "Mapping file", "map" },
        { "All files", "*" }
    };

    const char* NameMapTable = "MapTable";
    const char* NameMenuBar = "MappingMenuBar";
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

        RegisterMenu(menu, MSG_Menu_File, MSG_Menu_Item_File);
        RegisterMenu(menu, MSG_Menu_Reg, MSG_Menu_Item_Reg);

        menu->connectMenuItem({MSG_Menu_File, MSG_Menu_Item_File[0]}, [this]{
            importMapping = true;
        });
        menu->connectMenuItem({MSG_Menu_File, MSG_Menu_Item_File[1]}, [this]{
            exportMapping = true;
        });
        menu->connectMenuItem({MSG_Menu_Reg, MSG_Menu_Item_Reg[0]}, [this]{
            loadMapping = true;
        });
        menu->connectMenuItem({MSG_Menu_Reg, MSG_Menu_Item_Reg[1]}, [this]{
            applyMapping = true;
        });
        
        gui.add(menu, NameMenuBar);

        const auto MenuSize = menu->getFullSize();

        auto table = tgui::ListView::create();
        table->addColumn("Mapping From", MenuSize.x / 2);
        table->addColumn("Mapping To", MenuSize.x - MenuSize.x / 2);
        table->setPosition({0, MenuSize.y});
        table->setSize(MenuSize.x, std::format("parent.h - {:s}.h", NameMenuBar).c_str());
        table->setResizableColumns(false);
        table->getHorizontalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);

        gui.add(table, NameMapTable);
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
}