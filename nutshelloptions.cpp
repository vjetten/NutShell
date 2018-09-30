#include "nutshellqt.h"


//---------------------------------------------------------------------------
nutshellOptions::nutshellOptions(QWidget *parent) :
		QDialog(parent)
{
	setupUi(this);
    this->setWindowTitle("Directories for the GIS software");
   this->setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
	connect(toolButton_1, SIGNAL(clicked()), this, SLOT(findPcrcalcDir()));
//	connect(toolButton_2, SIGNAL(clicked()), this, SLOT(findAguilaDir()));
//	connect(toolButton_3, SIGNAL(clicked()), this, SLOT(findMapeditDir()));
    connect(toolButton_4, SIGNAL(clicked()), this, SLOT(findGDALDir()));
    //	connect(pushButton_ok, SIGNAL(clicked()), this, SLOT(closew()));
	//	connect(pushButton_cancel, SIGNAL(clicked()), this, SLOT(close()));

	baseDirs.clear();
}
//---------------------------------------------------------------------------
nutshellOptions::~nutshellOptions()
{
}
//---------------------------------------------------------------------------
void nutshellOptions::setupOptions(QStringList SL)
{
	baseDirs = SL;
	lineEdit_pcrcalcDir->setText(SL[0]);
//	lineEdit_aguilaDir->setText(SL[1]);
//	lineEdit_mapeditDir->setText(SL[2]);
    lineEdit_GDALDir->setText(SL[1]);
}
//---------------------------------------------------------------------------
QStringList nutshellOptions::getOptions()
{
	baseDirs.clear();
	baseDirs << lineEdit_pcrcalcDir->text()
  //          << lineEdit_aguilaDir->text()
          //  << lineEdit_mapeditDir->text()
            << lineEdit_GDALDir->text();
	return baseDirs;
}
//---------------------------------------------------------------------------
void nutshellOptions::findPcrcalcDir()
{
	QString dir;
	QString olddir = lineEdit_pcrcalcDir->text();

    dir = setExistingDirectory("pcrcalc main dir",baseDirs[0]);

    if (!dir.endsWith("\\") && !dir.endsWith("/"))
        dir = dir + "/";// QDir::separator();
    if (QFileInfo(dir+"bin/pcrcalc.exe").exists())
    {
//        if (!dir.endsWith(QDir::separator()))
//            dir = dir + QDir::separator();
		lineEdit_pcrcalcDir->setText(dir);
        baseDirs[0] = dir;

//        if (QFileInfo(dir+"aguila.exe").exists())
//        {
//            lineEdit_aguilaDir->setText(dir);
//            baseDirs[1] = dir;
//        }
    }
	else
	{
		lineEdit_pcrcalcDir->setText(olddir);
		ErrorMsg("pcrcalc.exe not found.");
	}
}
//---------------------------------------------------------------------------
void nutshellOptions::findAguilaDir()
{
    /*
	QString dir;
	QString olddir = lineEdit_pcrcalcDir->text();
    dir = setExistingDirectory("aguila dir (bin)",baseDirs[1]);
	if (!dir.endsWith(QDir::separator()))
		dir = dir + QDir::separator();
    qDebug() << baseDirs << dir+"aguila.exe";
	if (QFileInfo(dir+"aguila.exe").exists())
    {
        if (!dir.endsWith(QDir::separator()))
            dir = dir + QDir::separator();
        lineEdit_aguilaDir->setText(dir);
        baseDirs[1] = dir;
    }
	else
	{
		lineEdit_aguilaDir->setText(olddir);
		ErrorMsg("aguila.exe not found.");
	}
    */
}
//---------------------------------------------------------------------------
void nutshellOptions::findMapeditDir()
{
    /*
    QString dir = baseDirs[0];
	QString olddir = lineEdit_pcrcalcDir->text();
    dir = setExistingDirectory("mapedit dir",dir);//baseDirs[2]);
	if (!dir.endsWith(QDir::separator()))
		dir = dir + QDir::separator();
	if (QFileInfo(dir+"mapedit.exe").exists())
    {
        if (!dir.endsWith(QDir::separator()))
            dir = dir + QDir::separator();
		lineEdit_mapeditDir->setText(dir);
    }
	else
	{
		lineEdit_mapeditDir->setText(olddir);
		ErrorMsg("mapedit.exe not found.");
	}
    */
}
//---------------------------------------------------------------------------
void nutshellOptions::findGDALDir()
{
    QString dir;
    QString olddir = lineEdit_GDALDir->text();
    dir = setExistingDirectory("GDAL main dir",baseDirs[1]);
    if (!dir.endsWith("\\") && !dir.endsWith("/"))
        dir = dir + "/";
    if (QFileInfo(dir+"bin/gdal/apps/gdal_translate.exe").exists())
    {
        lineEdit_GDALDir->setText(dir);
    }
    else
    {
        lineEdit_GDALDir->setText(olddir);
        ErrorMsg("GDAL executables not found.");
    }
}
//---------------------------------------------------------------------------
QString nutshellOptions::setExistingDirectory(QString title, QString bd)
{
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
	QString directory =
            QFileDialog::getExistingDirectory(this, title, bd);//, options);
	if (!directory.isEmpty())
		return(directory);
	else
		return "";
}
//---------------------------------------------------------------------------
