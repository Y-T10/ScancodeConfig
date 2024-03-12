#pragma once

#include <QItemSelection>
#include <QTableView>
#include <QPointer>
#include <QWidget>

#include "CsmViewer.hpp"

namespace AppSacnConf {
    class MappingTableWidget : public QWidget {
        Q_OBJECT
        public:
            explicit MappingTableWidget(QWidget* parent = nullptr) noexcept;
            void importMapping() noexcept;
            void exportMapping() noexcept;

        public slots:
            void showAddMappingDialog() noexcept;
            void addMapping(const CompScanMap::ScanMapping& mapping) noexcept;
            void editMapping() noexcept;
            void removeMapping() noexcept;

        signals:
            void selectionChanged(const QItemSelection& selected);
        
        private:
            QPointer<QTableView> m_view;
    };
}