#include "AsconfMapModel.hpp"
#include "CsmViewer.hpp"
#include <qabstractitemmodel.h>
#include <qnamespace.h>
#include <qtpreprocessorsupport.h>
#include <qvariant.h>

namespace AppSacnConf {
    MappingModel::MappingModel(QObject *parent) noexcept:
    QAbstractTableModel(parent),
    m_mappings(){
    }

    MappingModel::MappingModel(const container_type& mappings, QObject *parent) noexcept:
    QAbstractTableModel(parent),
    m_mappings(mappings){
    }

    int MappingModel::rowCount(const QModelIndex &parent) const {
        return parent.isValid() ? 0 : m_mappings.size();
    }

    int MappingModel::columnCount(const QModelIndex &parent) const {
        return parent.isValid() ? 0 : 2;
    }

    QVariant MappingModel::data(const QModelIndex &index, int role) const {
        if (!index.isValid()) {
            return QVariant();
        }

        if (index.row() < 0 && m_mappings.size() <= index.row()) {
            return QVariant();
        }

        if (role == Qt::DisplayRole) {
            return getData(index.row(), index.column());
        }

        if (role == Qt::TextAlignmentRole) {
            return Qt::AlignRight;
        }

        return QVariant();
    }

    const QVariant MappingModel::getData(const int row, const int col) const noexcept {
        const auto map = m_mappings.at(row);

        if (col == ColIndexFrom) {
            return map.from;
        }
        if (col == ColIndexTo) {
            return map.to;
        }
        return QVariant();
    }

    QVariant MappingModel::headerData(int section, Qt::Orientation orientation, int role) const {
        if (orientation != Qt::Horizontal) {
            return QVariant();
        }

        const auto GetHeaderName = [](const int colIndex)->QVariant {
            if (colIndex == ColIndexFrom) {
                return tr("Mapping From");
            }
            if (colIndex == ColIndexTo) {
                return tr("Mapping To");
            }
            return QVariant();
        };

        if (role == Qt::DisplayRole) {
            return GetHeaderName(section);
        }
        
        return QVariant();
    }

    Qt::ItemFlags MappingModel::flags(const QModelIndex &index) const {
        if (!index.isValid()) {
            return Qt::ItemIsEnabled;
        }

        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    }

    bool MappingModel::setData(const QModelIndex &index, const QVariant &value, int role) {
        if (!index.isValid() || role != Qt::EditRole) {
            return false;
        }

        if (!value.canConvert<CompScanMap::Scancode>()) {
            return false;
        }

        auto mapping = m_mappings.value(index.row());
        if(index.column() == ColIndexFrom) {
            mapping.from = value.value<CompScanMap::Scancode>();
        } else if (index.column() == ColIndexTo) {
            mapping.to = value.value<CompScanMap::Scancode>();
        } else {
            return false;
        }

        m_mappings.replace(index.row(), mapping);
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

        return true;
    }

    bool MappingModel::insertRows(int position, int rows, const QModelIndex &index) {
        Q_UNUSED(index);

        beginInsertRows(QModelIndex(), position, position + rows - 1);

        for(int row = 0; row < rows; ++row){
            m_mappings.insert(position, container_type::value_type{});
        }
        endInsertRows();
        return true;
    }

    bool MappingModel::removeRows(int position, int rows, const QModelIndex &index) {
        Q_UNUSED(index);

        beginRemoveRows(QModelIndex(), position, position + rows - 1);

        for(int row = 0; row < rows; ++row){
            m_mappings.removeAt(position);
        }
        endRemoveRows();
        return true;
    }

    const MappingModel::container_type &MappingModel::getMappings() const {
        return m_mappings;
    }
};