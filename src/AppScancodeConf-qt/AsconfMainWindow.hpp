#pragma once

#include <QItemSelection>
#include <QMainWindow>
#include <QMenu>

#include "AsconfMappingTable.hpp"

#include "CregTypes.hpp"
#include "CproHandle.hpp"

namespace AppSacnConf {
    class MainWindow : public QMainWindow {
        public:
            explicit MainWindow() noexcept;

        private slots:
            void applyMapping() noexcept;
            void readMapping() noexcept;
            void importMapping() noexcept;
            void exportMapping() noexcept;
    
        private:
            void createMenu() noexcept;
            void writeMapping(const CmpProc::object_handle& pipe, const CompReg::win32str& pipeName) noexcept;

            QPointer<MappingTableWidget> m_mappingWidget;
    };
}