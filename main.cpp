#include <QtGui/QApplication>
#include "nutshellqt.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    nutshellqt w;

    //a.addLibraryPath("nutslibs");

    w.setWindowTitle("nutShell Qt (version 3.5 beta [13 Mar 2012])");
    w.showMaximized();
    w.show();
	// a.setStyle("Cleanlooks");

    return a.exec();
}
