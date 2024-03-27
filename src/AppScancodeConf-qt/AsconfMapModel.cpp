#include "AsconfMapModel.hpp"
#include "CsmViewer.hpp"

#include <QAbstractItemModel>
#include <QErrorMessage>
#include <QApplication>
#include <QVariant>
#include <QFont>

#include <optional>
#include <qerrormessage.h>
#include <string>
#include <type_traits>

namespace {
    static constexpr auto TextNotApplicable = "N/A";

    const std::string GetNameFrom (const CompScanMap::Scancode code) {
        // TODO: エラーコードとキーの番号を標準エラーに書き込む
        return CompScanMap::WindowsScancodeName(code).value_or(TextNotApplicable);
    };

    const std::string GetNameTo(const CompScanMap::Scancode code) {
        // TODO: エラーコードとキーの番号を標準エラーに書き込む
        const std::string KeyName = CompScanMap::KeyboardKeyName(code).value_or(TextNotApplicable);
        if (!KeyName.empty()) {
            return KeyName;
        }
        return CompScanMap::WindowsScancodeName(code).value_or(TextNotApplicable);
    };

    template <class F>
    const QString GenerateText(const CompScanMap::Scancode code, const F &func) {
        static_assert(std::is_invocable_r_v<const std::string, F, CompScanMap::Scancode>);
        return QString("%1 (%2)").arg(func(code).c_str()).arg(code, 0, 16); 
    };
}

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

        if (index.row() < 0 || m_mappings.size() <= (size_t)(index.row())) {
            return QVariant();
        }

        if (role == Qt::DisplayRole) {
            return getData(index.row(), index.column());
        }

        if (role == Qt::TextAlignmentRole) {
            return (int)(Qt::AlignLeft | Qt::AlignVCenter);
        }

        if (role == Qt::FontRole) {
            const auto AppFont = QApplication::font();
            return QFont(AppFont.family(), 12, AppFont.weight(), AppFont.italic());
        }

        return QVariant();
    }

    const QVariant MappingModel::getData(const int row, const int col) const noexcept {
        const auto map = m_mappings.at(row);

        if (col == ColIndexFrom) {
            return GenerateText(map.from, GetNameFrom);
        }
        if (col == ColIndexTo) {
            return GenerateText(map.to, GetNameTo);
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

        if (role == Qt::TextAlignmentRole) {
            return Qt::AlignCenter;
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

        // TODO: 実際の列数に応じて反応できるようにする
        if (index.column() < 0 || 2 < index.column()) {
            return false;
        }

        if(index.column() == ColIndexFrom) {
            m_mappings[index.row()].from = value.value<CompScanMap::Scancode>();
        } else if (index.column() == ColIndexTo) {
            m_mappings[index.row()].to = value.value<CompScanMap::Scancode>();
        }

        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

        return true;
    }

    bool MappingModel::insertRows(int position, int rows, const QModelIndex &index) {
        Q_UNUSED(index);

        beginInsertRows(QModelIndex(), position, position + rows - 1);
        constexpr container_type::value_type EmptyMapping = {.to = 0, .from = 0};
        m_mappings.insert(m_mappings.begin() + position, rows, EmptyMapping);
        endInsertRows();
        return true;
    }

    bool MappingModel::removeRows(int position, int rows, const QModelIndex &index) {
        Q_UNUSED(index);

        beginRemoveRows(QModelIndex(), position, position + rows - 1);

        const auto eraseBegin = m_mappings.begin() + position;
        m_mappings.erase(eraseBegin, eraseBegin + rows);
        endRemoveRows();
        return true;
    }

    const MappingModel::container_type &MappingModel::getMappings() const {
        return m_mappings;
    }

    const std::optional<CompScanMap::ScanMapping> MappingModel::rawData(const QModelIndex &index) const {    
        if (!index.isValid()) {
            return std::nullopt;
        }

        if (index.row() < 0 || m_mappings.size() <= (size_t)(index.row())) {
            return std::nullopt;
        }

        return m_mappings.at(index.row());;
    }
};