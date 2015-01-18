/*
 * main: used in the NutShhell Explorer

 * Author: VJ 140222
 */

#include <QtGui/QApplication>
#include "nutshellqt.h"
//qlineargradient(x1:0, y1:0, x2:1, y2:1,stop:0 #eee, stop:1 #ccc);
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    nutshellqt w;

//    qApp->setStyleSheet(
//                "QSplitter::handle {background-color : #fff;\
//                border: 1px solid #aaa;\
//              width: 1px;\
//              height: 1px;\
//    margin-top: 1px;\
//    margin-bottom: 1px;\
//    border-radius: 2px;}");

    w.setWindowTitle("NutShell (version 4.86 [18 Jan 2015])");
    w.showMaximized();
    w.show();
    //a.setStyle("Cleanlooks");

    return a.exec();
}
