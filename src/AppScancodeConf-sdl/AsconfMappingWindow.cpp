#include "AsconfMappingWindow.hpp"

#include <algorithm>
#include <iterator>
#include <cstddef>
#include <cstdlib>
#include <format>

#include "challenger/challenger_dialog.hpp"
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
    ConfigWindow::ConfigWindow(const CompScanMap::MappingList& list) noexcept:
    importMapping(false),
    exportMapping(false),
    loadMapping(false),
    applyMapping(false),
    mapping(ToConfWindowContainer(list)){
    }
    ConfigWindow::ConfigWindow(ConfigWindow& rval) noexcept:
    importMapping(std::move(rval.importMapping)),
    exportMapping(std::move(rval.exportMapping)),
    loadMapping(std::move(rval.loadMapping)),
    applyMapping(std::move(rval.applyMapping)),
    mapping(std::move(rval.mapping)){
    }
    ConfigWindow::ConfigWindow() noexcept:
    importMapping(false),
    exportMapping(false),
    loadMapping(false),
    applyMapping(false),
    mapping({}){
    }

    ConfigWindow& ConfigWindow::operator=(ConfigWindow&& rval) noexcept {
        if (this == &rval) {
            return *this;
        }

        importMapping = std::move(rval.importMapping);
        exportMapping = std::move(rval.exportMapping);
        loadMapping = std::move(rval.loadMapping);
        applyMapping = std::move(rval.applyMapping);
        mapping = std::move(rval.mapping);

        return *this;
    }


    void ConfigWindow::show(const SDL_Rect drawArea) noexcept {
        showMenuBar();
        showTable();
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