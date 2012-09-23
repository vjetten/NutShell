#include <QtGui/QApplication>
#include "nutshellqt.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    nutshellqt w;

    w.setWindowTitle("nutShell (version 3.7 [23 Sep 2012])");
    w.showMaximized();
    w.show();
	//a.setStyle("Cleanlooks");

    return a.exec();
}
