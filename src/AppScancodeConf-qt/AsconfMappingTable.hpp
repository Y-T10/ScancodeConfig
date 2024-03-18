#pragma once

#include <QItemSelection>
#include <QTableView>
#include <QPointer>
#include <QWidget>
#include <QAction>

#include "AsconfMapModel.hpp"

#include "CsmViewer.hpp"

namespace AppSacnConf {
    class MappingTableWidget : public QWidget {
        Q_OBJECT
        public:
            explicit MappingTableWidget(
                const MappingModel::container_type& mappings = MappingModel::container_type{},
                QWidget* parent = nullptr
            ) noexcept;
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
            QAction* m_actAdd;
            QAction* m_actEdit;
            QAction* m_actRemove;

            const QList<QAction* > contextMenuActions() noexcept;
        
        protected:
            void resizeEvent(QResizeEvent *event) override;
    };
}