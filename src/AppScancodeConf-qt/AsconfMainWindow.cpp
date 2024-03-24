#include "AsconfMainWindow.hpp"

#include <QFileDialog>
#include <QPointer>
#include <QMenuBar>
#include <QWidget>
#include <QMenu>

#include <type_traits>
#include <cassert>
#include <ostream>
#include <fstream>
#include <string>

#include "CregHandler.hpp"
#include "CsmCodec.hpp"

namespace {
    const CompScanMap::MappingList ReadScancodeMap() noexcept {
        const auto RegKey = CompReg::OpenRegKey(
            HKEY_LOCAL_MACHINE,
            TEXT("SYSTEM\\CurrentControlSet\\Control\\Keyboard Layout"),
            KEY_READ
        );
        if (!RegKey) {
            return {};
        }

        const auto RegValue = ReadKeyValueBin(RegKey, TEXT("Scancode Map"));
        if (!RegValue) {
            return {};
        }

        const auto MappingList = CompScanMap::DecodeScancodeMap(*RegValue);
        return MappingList.has_value() ? *MappingList : CompScanMap::MappingList{};
    }

    const AppSacnConf::MappingModel::container_type CurrentScancodeMap() noexcept {
        return ReadScancodeMap();
    }

    const QPointer<QFileDialog> MappingFileDialog(QWidget* parent, const std::u8string& caption, const QFileDialog::FileMode& fileMode) noexcept {
        QPointer<QFileDialog> dialog(new QFileDialog(
            parent, QString(caption.data()),
            QString(), QString(u8"TOML Files (*.toml)")
        ));

        dialog->setFileMode(fileMode);

        return dialog;
    }
}

namespace AppSacnConf {
    MainWindow::MainWindow() noexcept:
    QMainWindow(),
    m_mappingWidget(new MappingTableWidget(CurrentScancodeMap(), this)){
        setFixedSize(300, 300);

        setCentralWidget(m_mappingWidget);
        m_mappingWidget->setFixedWidth(width());

        createMenu();
        setWindowTitle(tr("Scancode Map Viewer"));
    }

    void MainWindow::createMenu() noexcept {
        auto fileMenu = menuBar()->addMenu(tr("&File"));

        const auto addMenuItem = []<class class_type>(QMenu* menu, const QString& text, class_type* obj, void (class_type::*func)()){
            static_assert(std::is_base_of_v<QObject, class_type>);
            QAction* act = new QAction(text);
            menu->addAction(act);
            connect(act, &QAction::triggered, obj, func);
        };
    
        addMenuItem(fileMenu, tr("&Import Mapping"), this, &MainWindow::importMapping);
        addMenuItem(fileMenu, tr("&Export Mapping"), this, &MainWindow::exportMapping);
    
        auto applyMenu = menuBar()->addMenu(tr("&Apply"));
        addMenuItem(applyMenu, tr("&Apply mapping"), this, &MainWindow::applyMapping);
    }

    void MainWindow::applyMapping() noexcept {

    };
    
    void MainWindow::importMapping() noexcept {

    };
    
    void MainWindow::exportMapping() noexcept {
        const auto fileDialog = MappingFileDialog(this, u8"Export mappings", QFileDialog::AnyFile);

        if (!fileDialog->exec()) {
            return;
        }

        const auto SelectedFiles = fileDialog->selectedFiles();

        if (SelectedFiles.empty()) {
            return;
        }

        assert(SelectedFiles.size() == 1);
        const auto FilePath = SelectedFiles.front();

        std::ofstream(std::filesystem::path(FilePath.toStdString())) << "" << std::endl;
        return;
    };
}