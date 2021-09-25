/*
 * main: used in the NutShhell Explorer

 * Author: VJ 140222,181001
 */

#include <QApplication>
#include "nutshellQt.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    nutshellqt w;

    w.setWindowTitle("NutShell (version 5.12 [25 Sep 2021])");
    w.showMaximized();
    w.show();
    a.setStyle("Fusion");

    return a.exec();
}
