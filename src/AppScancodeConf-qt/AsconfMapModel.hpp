#pragma once

#include <QAbstractTableModel>
#include <QObject>
#include <QList>
#include <optional>

#include "CsmViewer.hpp"

namespace AppSacnConf {
    class MappingModel : public QAbstractTableModel {
        Q_OBJECT

        public:
            using container_type = CompScanMap::MappingList;

            static constexpr int ColIndexFrom = 0;
            static constexpr int ColIndexTo = 1;

            explicit MappingModel(QObject *parent = nullptr) noexcept;
            MappingModel(const container_type& mappings, QObject *parent = nullptr) noexcept;

            int rowCount(const QModelIndex &parent) const override;
            int columnCount(const QModelIndex &parent) const override;
            QVariant data(const QModelIndex &index, int role) const override;
            QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
            Qt::ItemFlags flags(const QModelIndex &index) const override;
            bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
            bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
            bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
            const container_type &getMappings() const;

            const std::optional<CompScanMap::ScanMapping> rawData(const QModelIndex &index) const;

        private:
            const QVariant getData(const int row, const int col) const noexcept;

            container_type m_mappings;
    };
};