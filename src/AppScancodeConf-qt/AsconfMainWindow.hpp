#pragma once

#include <QItemSelection>
#include <QMainWindow>

#include "AsconfMappingTable.hpp"

namespace AppSacnConf {
    class MainWindow : public QMainWindow {
        public:
            explicit MainWindow() noexcept;

        private slots:
            void updateActions(const QItemSelection &selection) noexcept;
            void applyMapping() noexcept;
            void importMapping() noexcept;
            void exportMapping() noexcept;
    
        private:
            void createMenu() noexcept;
            void addMenuItem(QMenu* menu, const QString& text, void (MainWindow::*func)()) noexcept;

            QPointer<MappingTableWidget> m_mappingWidget;
    };
}