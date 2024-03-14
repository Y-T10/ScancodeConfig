#include <QApplication>
#include <QPointer>

#include "AsconfMainWindow.hpp"

using namespace AppSacnConf;

int main(int argc, char* argv[]) {
    QPointer<QApplication> app (new QApplication(argc, argv));
    MainWindow window;
    window.show();
    return app->exec();
}