#include <QItemSelection>
#include <QProgressBar>
#include <QApplication>
#include <QPushButton>
#include <QMainWindow>
#include <QMenuBar>
#include <QPointer>
#include <QWidget>
#include <QSlider>

class MainWindow : public QMainWindow {
    public:
        explicit MainWindow() noexcept;

    private slots:
        void updateActions(const QItemSelection &selection) noexcept;
        void applyMapping() noexcept;
        void importMapping() noexcept;
        void exportMapping() noexcept;
    
    private:
        void createMenu() noexcept;
};

int main(int argc, char* argv[]) {
    QPointer<QApplication> app (new QApplication(argc, argv));
    MainWindow window;
    window.show();
    return app->exec();
}

MainWindow::MainWindow() noexcept:
QMainWindow(){
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

}

void MainWindow::applyMapping() noexcept {

};
void MainWindow::importMapping() noexcept {

};
void MainWindow::exportMapping() noexcept {

};