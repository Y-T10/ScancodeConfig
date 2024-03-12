#include "AsconfAddDialog.hpp"
#include "CsmViewer.hpp"

#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <cassert>
#include <limits>
#include <optional>

namespace {
    const QPointer<QLineEdit> CreateTextForm(QWidget* parent = nullptr) noexcept {
        QPointer<QLineEdit> form = new QLineEdit(parent);
        // フォームの入力を制限する
        form->setInputMask(QObject::tr("HHHH; "));
        // フォームを右詰にする
        form->setText("");
        return form;
    }

    void PlaceTextbox(const int row, const QString& formName, const QPointer<QLineEdit>& inputForm, QGridLayout* layoutParent) noexcept {
        layoutParent->addWidget(new QLabel(formName), row, 0, Qt::AlignLeft | Qt::AlignTop);
        layoutParent->addWidget(inputForm, row , 1, Qt::AlignLeft);
    }

    const std::optional<uint> ConvertToHex(const QString& str) noexcept {
        const auto RemoveSuffix = [](const QString& str) {
            if (!str.startsWith("0x")) {
                return str;
            };

            auto newStr = str;
            newStr.erase(newStr.begin(), newStr.begin() + 2);
            return newStr;
        };

        bool result = false;
        const auto value = RemoveSuffix(str).toUInt(&result, 16);

        return result ? std::make_optional(value): std::nullopt;
    };
}

namespace AppSacnConf {
    AddDialog::AddDialog(QWidget* parent) noexcept:
    QDialog(parent),
    m_formFrom(CreateTextForm(this)),
    m_formTo(CreateTextForm(this))
    {
        // ウィジェットレイアウトオブジェクトを作成する
        auto tableLayout = new QGridLayout;
        tableLayout->setColumnStretch(1, 2);

        // ボタンを配置
        placeButton(tableLayout);

        // 入力フォームを配置
        PlaceTextbox(0, tr("Mapping from"), m_formFrom, tableLayout);
        PlaceTextbox(1, tr("Mapping to"), m_formTo, tableLayout);

        // レイアウトをダイアログに設定する
        auto layoutMain = new QVBoxLayout;
        layoutMain->addLayout(tableLayout);
        setLayout(layoutMain);
    }

    const CompScanMap::ScanMapping AddDialog::mapping() const {
        const auto ValueFrom = ConvertToHex(m_formFrom->text());
        const auto ValueTo = ConvertToHex(m_formTo->text());
        assert(ValueFrom && ValueTo);
        assert(ValueFrom.value() <= std::numeric_limits<CompScanMap::Scancode>::max());
        assert(ValueTo.value() <= std::numeric_limits<CompScanMap::Scancode>::max());

        return {
            .to = (CompScanMap::Scancode)(ValueTo.value()),
            .from = (CompScanMap::Scancode)(ValueFrom.value())
        };
    }

    void AddDialog::editMapping(const CompScanMap::ScanMapping& mapping) noexcept {
        m_formTo->setText(QString("0x%1").arg(mapping.to, 4, u'0'));
        m_formFrom->setText(QString("0x%1").arg(mapping.from, 4, u'0'));
    }

    void AddDialog::placeButton(QGridLayout* parent) noexcept {
        // ボタンを作成する
        auto buttonOK = new QPushButton(tr("OK"));
        auto buttonCancel = new QPushButton(tr("Cancel"));
        connect(buttonOK, &QAbstractButton::clicked, this, &QDialog::accept);
        connect(buttonCancel, &QAbstractButton::clicked, this, &QDialog::reject);

        // ボタンを配置
        auto buttonLayout = new QVBoxLayout;
        buttonLayout->addWidget(buttonOK);
        buttonLayout->addWidget(buttonCancel);
        parent->addLayout(buttonLayout, 2, 1, Qt::AlignRight);
    }
}