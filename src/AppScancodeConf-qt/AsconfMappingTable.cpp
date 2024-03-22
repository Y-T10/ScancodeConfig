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
        m_view->horizontalHeader()->setSectionResizeMode(AppSacnConf::MappingModel::ColIndexFrom, QHeaderView::Fixed);
        m_view->horizontalHeader()->setSectionResizeMode(AppSacnConf::MappingModel::ColIndexTo, QHeaderView::Fixed);
        m_view->verticalHeader()->hide();
        m_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_view->setSelectionMode(QAbstractItemView::SingleSelection);
        m_view->setSortingEnabled(false);

        m_view->setContextMenuPolicy(Qt::ActionsContextMenu);
        m_view->insertActions(nullptr, contextMenuActions());

        connect(m_view->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MappingTableWidget::selectionChanged);
        connect(
            m_view->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, [this](const QItemSelection &selection)->void{
                const bool IsSelected = !(selection.indexes().empty());
                m_actEdit->setEnabled(IsSelected);
                m_actRemove->setEnabled(IsSelected);
            }
        );
    }

    const MappingModel::container_type& MappingTableWidget::mappings() const noexcept {
        MappingModel* const model = static_cast<MappingModel*>(m_view->model());
        return model->getMappings();
    }

    void MappingTableWidget::setMappings(const MappingModel::container_type& mappings) noexcept {
        m_view->setModel(new MappingModel(mappings, this));
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

        if (std::ranges::find(model->getMappings(), mapping) != model->getMappings().end()) {
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

        const auto IndexSelected = m_view->selectionModel()->selectedRows();
        const int insertPosition = IndexSelected.empty()? 0: IndexSelected.begin()->row();

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
            const auto map = model->rawData(idx);
            if(!map.has_value()) {
                continue;
            }
            dialog.editMapping(*map);

            // Dialog
            if(!dialog.exec()) {
                return;
            }

            SetMapping(*model, idx.row(), dialog.mapping());
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

        // 行と列の長さを変更
        m_view->setColumnWidth(MappingModel::ColIndexFrom, event->size().width() / 2);
        m_view->setColumnWidth(MappingModel::ColIndexTo, event->size().width() / 2 - (event->size().width() / 2));
        m_view->resizeRowsToContents();
    }

    const QList<QAction*> MappingTableWidget::contextMenuActions() noexcept {
        m_actAdd = new QAction(tr("add a mapping"));
        connect(m_actAdd, &QAction::triggered, this, &MappingTableWidget::showAddMappingDialog);

        m_actEdit = new QAction(tr("Edit"));
        m_actEdit->setEnabled(false);
        connect(m_actEdit, &QAction::triggered, this, &MappingTableWidget::editMapping);

        m_actRemove = new QAction(tr("Remove"));
        m_actRemove->setEnabled(false);
        connect(m_actRemove, &QAction::triggered, this, &MappingTableWidget::removeMapping);

        return {m_actAdd, m_actEdit, m_actRemove};
    }
}