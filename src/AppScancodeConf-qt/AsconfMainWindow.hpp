#pragma once

#include <QItemSelection>
#include <QMainWindow>
#include <QMenu>

#include <type_traits>

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

            template <class class_type>
            void addMenuItem(QMenu* menu, const QString& text, class_type* obj, void (class_type::*func)()) noexcept {
                static_assert(std::is_base_of_v<QObject, class_type>);

                QAction* act = new QAction(text);
                menu->addAction(act);
                connect(act, &QAction::triggered, obj, func);
            }


            QPointer<MappingTableWidget> m_mappingWidget;
    };
}