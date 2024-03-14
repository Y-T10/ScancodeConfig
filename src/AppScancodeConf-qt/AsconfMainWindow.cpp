#include "AsconfMainWindow.hpp"

#include <QMenuBar>
#include <QWidget>

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

    const QList<CompScanMap::ScanMapping> CurrentScancodeMap() noexcept {
        const auto CurrentMap = ReadScancodeMap();
        return QList<CompScanMap::ScanMapping>(CurrentMap.begin(), CurrentMap.end());
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
    
        QAction* importAct = new QAction(tr("&Import Mapping"));
        fileMenu->addAction(importAct);
        connect(importAct, &QAction::triggered, this, &MainWindow::importMapping);
    
        QAction* exportAct = new QAction(tr("&Export Mapping"));
        fileMenu->addAction(exportAct);
        connect(exportAct, &QAction::triggered, this, &MainWindow::exportMapping);
    
        auto applyMenu = menuBar()->addMenu(tr("&Apply"));
        QAction* applyAct = new QAction(tr("&Apply mapping"));
        applyMenu->addAction(applyAct);
        connect(applyAct, &QAction::triggered, this, &MainWindow::applyMapping);
    }

    void updateActions(const QItemSelection &selection) noexcept {
        QModelIndexList indexes = selection.indexes();
    }

    void MainWindow::applyMapping() noexcept {

    };
    
    void MainWindow::importMapping() noexcept {

    };
    
    void MainWindow::exportMapping() noexcept {

    };
}