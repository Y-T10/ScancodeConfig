#pragma once

#include <QItemSelection>
#include <QMainWindow>

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
    };
}