#include "nutshellqt.h"


//---------------------------------------------------------------------------
nutshellOptions::nutshellOptions(QWidget *parent) :
		QDialog(parent)
{
	setupUi(this);
    this->setWindowTitle("Directories for the GIS software");
    this->setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint);

	connect(toolButton_1, SIGNAL(clicked()), this, SLOT(findPcrcalcDir()));
    connect(toolButton_2, SIGNAL(clicked()), this, SLOT(findCondaDir()));
//	connect(toolButton_3, SIGNAL(clicked()), this, SLOT(findMapeditDir()));
    connect(toolButton_4, SIGNAL(clicked()), this, SLOT(findGDALDir()));


	baseDirs.clear();
}
//---------------------------------------------------------------------------
nutshellOptions::~nutshellOptions()
{
}
//---------------------------------------------------------------------------
void nutshellOptions::setupOptions(QStringList SL, double dpi)
{
	baseDirs = SL;
	lineEdit_pcrcalcDir->setText(SL[0]);
//	lineEdit_aguilaDir->setText(SL[1]);
//	lineEdit_mapeditDir->setText(SL[2]);
    lineEdit_GDALDir->setText(SL[1]);
    lineEdit_CondaDir->setText(SL[2]);
    dpiScale->setValue(dpi);
}
//---------------------------------------------------------------------------
QStringList nutshellOptions::getOptions()
{
	baseDirs.clear();
	baseDirs << lineEdit_pcrcalcDir->text()
  //          << lineEdit_aguilaDir->text()
          //  << lineEdit_mapeditDir->text()
            << lineEdit_GDALDir->text()
            << lineEdit_CondaDir->text()
            << dpiScale->text();
    return baseDirs;
}
//---------------------------------------------------------------------------
void nutshellOptions::findCondaDir()
{
    QString dir;
    QString olddir = lineEdit_CondaDir->text();

    dir = setExistingDirectory("(min)Conda main dir",baseDirs[0]);

    if (!dir.endsWith("\\") && !dir.endsWith("/"))
        dir = dir + "/";

    if (QFileInfo(dir+"_conda.exe").exists())
    {
        QString pcrdir = QString(dir+"envs/pcraster37/Library/bin/");
        qDebug() << pcrdir;
        lineEdit_pcrcalcDir->setText(pcrdir);
        QString gdaldir = pcrdir;
        lineEdit_GDALDir->setText(gdaldir);
      //  baseDirs.clear();
        baseDirs[0] = pcrdir;
        baseDirs[1] = gdaldir;
        lineEdit_CondaDir->setText(dir);
    }
    else
    {
        lineEdit_CondaDir->setText(olddir);
        ErrorMsg("_conda.exe not found.");
    }
    qDebug() << dir;
}
//---------------------------------------------------------------------------
void nutshellOptions::findPcrcalcDir()
{
	QString dir;
	QString olddir = lineEdit_pcrcalcDir->text();

    dir = setExistingDirectory("pcrcalc main dir",baseDirs[0]);

    if (!dir.endsWith("\\") && !dir.endsWith("/"))
        dir = dir + "/";
    if (QFileInfo(dir+"bin/pcrcalc.exe").exists())
    {
		lineEdit_pcrcalcDir->setText(dir);
        baseDirs[0] = dir;
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
      if (QFileInfo(dir+"bin/gdal_translate.exe").exists())
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
