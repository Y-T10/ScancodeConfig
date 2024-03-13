#include "AsconfMappingTable.hpp"

#include "CsmViewer.hpp"

#include <QResizeEvent>
#include <QHeaderView>
#include <QMessageBox>
#include <QTableView>
#include <algorithm>
#include <cassert>

#include "AsconfAddDialog.hpp"

namespace {
    const bool FindWithFrom(const AppSacnConf::MappingModel& model, const CompScanMap::Scancode from) noexcept {
        return std::ranges::any_of(model.getMappings(), [&from](const CompScanMap::ScanMapping& map){
            return map.from == from;
        });
    }

    template<Qt::ItemDataRole ItemRole = Qt::DisplayRole>
    const CompScanMap::ScanMapping GetMapping(const AppSacnConf::MappingModel& model, const int row) {
        const auto indexFrom = model.index(row, AppSacnConf::MappingModel::ColIndexFrom);
        const auto indexTo = model.index(row, AppSacnConf::MappingModel::ColIndexTo);

        const auto valueFrom = model.data(indexFrom, ItemRole);
        const auto valueTo = model.data(indexTo, ItemRole);

        assert(valueFrom.canConvert<CompScanMap::Scancode>());
        assert(valueTo.canConvert<CompScanMap::Scancode>());

        return CompScanMap::ScanMapping{
            .to = valueTo.value<CompScanMap::Scancode>(),
            .from = valueFrom.value<CompScanMap::Scancode>()
        };
    }

    void SetMapping(AppSacnConf::MappingModel& model, const int row, const CompScanMap::ScanMapping& mapping) {
        const auto indexFrom = model.index(row, AppSacnConf::MappingModel::ColIndexFrom);
        const auto indexTo = model.index(row, AppSacnConf::MappingModel::ColIndexTo);

        model.setData(indexFrom, mapping.from, Qt::EditRole);
        model.setData(indexTo, mapping.to, Qt::EditRole);
    }
}

namespace AppSacnConf {
    MappingTableWidget::MappingTableWidget(
        const MappingModel::container_type& mappings,
        QWidget* parent) noexcept:
    QWidget(parent),
    m_view(new QTableView(this)){
        // ビューを設定する
        m_view->setModel(new MappingModel(mappings, this));
        m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_view->horizontalHeader()->setStretchLastSection(true);
        m_view->verticalHeader()->hide();
        m_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_view->setSelectionMode(QAbstractItemView::SingleSelection);
        m_view->setSortingEnabled(false);

        connect(m_view->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MappingTableWidget::selectionChanged);
    }

    void MappingTableWidget::showAddMappingDialog() noexcept {
        AddDialog dialog;
        dialog.setWindowTitle(tr("Add a Scancode Mapping"));

        if (!dialog.exec()) {
            return;
        }

        addMapping(dialog.mapping());
    }

    void MappingTableWidget::addMapping(const CompScanMap::ScanMapping& mapping) noexcept {
        MappingModel* const model = static_cast<MappingModel*>(m_view->model());

        if (model->getMappings().contains(mapping)) {
            QMessageBox::information(this,
                tr("Duplicate Mapping"),
                tr("Mapping {%1, %2} already exists.")
                    .arg(mapping.from, 4, 16)
                    .arg(mapping.to, 4, 16)
            );
            return;
        }

        if (FindWithFrom(*model, mapping.from)) {
            QMessageBox::information(this,
                tr("Duplicate Mapping"),
                tr("Mapping from %1 already exists.")
                    .arg(mapping.from, 4, 16)
            );
            return;
        }

        constexpr int insertPosition = 0;
        model->insertRows(insertPosition, 1);
        model->setData(model->index(insertPosition, MappingModel::ColIndexFrom), mapping.from);
        model->setData(model->index(insertPosition, MappingModel::ColIndexTo), mapping.to);
    }

    void MappingTableWidget::editMapping() noexcept {
        MappingModel* const model = static_cast<MappingModel*>(m_view->model());
        const auto IndexSelected = m_view->selectionModel()->selectedRows();
        AddDialog dialog(this);
        dialog.setWindowTitle(tr("Edit Scancode Mapping"));

        for(const auto& idx: IndexSelected) {
            const int row = idx.row();
            dialog.editMapping(GetMapping(*model, row));

            // Dialog
            if(!dialog.exec()) {
                return;
            }

            SetMapping(*model, row, dialog.mapping());
        }
    }

    void MappingTableWidget::removeMapping() noexcept {
        MappingModel* const model = static_cast<MappingModel*>(m_view->model());
        const auto IndexSelected = m_view->selectionModel()->selectedRows();

        for(const auto& idx: IndexSelected) {
            model->removeRows(idx.row(), 1);
        }

        return;
    }

    void MappingTableWidget::resizeEvent(QResizeEvent *event) {
        m_view->resize(event->size());
    }
}