/*
 * main: used in the NutShhell Explorer

 * Author: VJ 140222,181001
 */

#include <QApplication>
#include "nutshellqt.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    nutshellqt w;

    w.setWindowTitle("NutShell (version 5.5 [12 Jan 2019])");
    w.showMaximized();
    w.show();
    //a.setStyle("Cleanlooks");

    return a.exec();
}
