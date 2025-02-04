/*
 * main: used in the NutShhell Explorer

 * Author: VJ 140222,181001
 */

#include <QApplication>
#include "nutshellQt.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::addLibraryPath(QDir::currentPath() + "/platforms");

    nutshellqt w;

    w.setWindowTitle("NutShell (version 5.18 - 250126)");
    w.showMaximized();
    w.show();
    a.setStyle("Fusion");

    return a.exec();
}
