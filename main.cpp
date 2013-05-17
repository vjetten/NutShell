/*
 * main: used in the NutShhell Explorer
 * v 1.x
 * Author: VJ 121012
 */

#include <QtGui/QApplication>
#include "nutshellqt.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    nutshellqt w;

    w.setWindowTitle("NutShell (version 4.8 [15 May 2013])");
    w.showMaximized();
    w.show();
    //a.setStyle("Cleanlooks");

    return a.exec();
}
