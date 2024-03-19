#include "AsconfMainWindow.hpp"

#include <QMenuBar>
#include <QWidget>
#include <QMenu>

#include <type_traits>

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

        connect(m_mappingWidget, &MappingTableWidget::selectionChanged, this, &MainWindow::updateActions);
    }

    void MainWindow::applyMapping() noexcept {

    };
    
    void MainWindow::importMapping() noexcept {

    };
    
    void MainWindow::exportMapping() noexcept {

    };
}