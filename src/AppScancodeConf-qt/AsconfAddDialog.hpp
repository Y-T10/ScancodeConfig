#pragma once

#include <QGridLayout>
#include <QLineEdit>
#include <QPointer>
#include <QDialog>

#include "CsmViewer.hpp"

namespace AppSacnConf {
    class AddDialog : public QDialog {
        Q_OBJECT

        public:
            AddDialog(QWidget* parent = nullptr) noexcept;

            const CompScanMap::ScanMapping mapping() const;
            void editMapping(const CompScanMap::ScanMapping& mapping) noexcept;

        private:
            void placeButton(QGridLayout* parent) noexcept;

            QPointer<QLineEdit> m_formFrom;
            QPointer<QLineEdit> m_formTo;
    };
}