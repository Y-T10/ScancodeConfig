#pragma once

#include <QAbstractTableModel>
#include <qlist.h>
#include <qobject.h>

#include "CsmViewer.hpp"

namespace AppSacnConf {
    /// スキャンマップ構造体の等号関数
    inline bool operator==(const CompScanMap::ScanMapping& l, const CompScanMap::ScanMapping& r) noexcept {
        return l.from == r.from && l.to == r.to;
    }

    inline QDataStream &operator<<(QDataStream& stream, const CompScanMap::ScanMapping& map) noexcept {
        const QString fromName = CompScanMap::ScancodeName(map.from).value_or("name_not_found").c_str();
        const QString toName = CompScanMap::ScancodeName(map.from).value_or("name_not_found").c_str();
        return stream << fromName << toName;
    };

    inline QDataStream &operator>>(QDataStream& stream, CompScanMap::ScanMapping& map) noexcept {
        return stream >> map.from >> map.to;
    };

    class MappingModel : public QAbstractTableModel {
        Q_OBJECT

        public:
            using element_type = CompScanMap::ScanMapping;
            using container_type = QList<element_type>;

            explicit MappingModel(QObject *parent = nullptr) noexcept;
            MappingModel(const container_type, QObject *parent = nullptr) noexcept;

            int rowCount(const QModelIndex &parent) const override;
            int columnCount(const QModelIndex &parent) const override;
            QVariant data(const QModelIndex &index, int role) const override;
            QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
            Qt::ItemFlags flags(const QModelIndex &index) const override;
            bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
            bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
            bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
            const container_type &getMappings() const;
        
        private:
            container_type m_mappings;
    };
};