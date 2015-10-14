#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("PElord");
    MainWindow w;
    w.show();

    return app.exec();
}
