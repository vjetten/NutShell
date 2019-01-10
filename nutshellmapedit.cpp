#include "nutshellqt.h"
#include "csf.h"


nutshellmapedit::nutshellmapedit(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    this->setWindowTitle("Directories for the GIS software");

}

nutshellmapedit::~nutshellmapedit()
{
}

void nutshellmapedit::loadmap(QString filename)
{
   MAP *_M = Mopen(filename.toLatin1(), M_READ_WRITE);




}
