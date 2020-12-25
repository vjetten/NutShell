/*
 * main: used in the NutShhell Explorer

 * Author: VJ 140222,181001
 */

#include <QApplication>
#include "nutshellQt.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    nutshellqt w;
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);


    w.setWindowTitle("NutShell (version 5.10 [9 Sep 2020])");
    w.showMaximized();
    w.show();
    a.setStyle("Fusion");

    return a.exec();
}
