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

    w.setWindowTitle("NutShell (version 5.152 [13 Feb 2024])");
    w.showMaximized();
    w.show();
    a.setStyle("Fusion");

    return a.exec();
}
