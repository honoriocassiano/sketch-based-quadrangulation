#include "mainwindow.h"
#include "utils/debug.h"
#include <QApplication>

int main(int argc, char *argv[]) {

    qInstallMessageHandler(myMessageOutput);

    QApplication a(argc, argv);
    MainWindow w;

    setlocale(LC_NUMERIC, "C");

    w.show();

    return a.exec();
}
