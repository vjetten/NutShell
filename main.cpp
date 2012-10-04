#include <QtGui/QApplication>
#include "nutshellqt.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    nutshellqt w;

    w.setWindowTitle("NutShell (version 3.8 [04 Oct 2012])");
    w.showMaximized();
    w.show();
	//a.setStyle("Cleanlooks");

    return a.exec();
}
