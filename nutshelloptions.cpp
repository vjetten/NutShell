#include "nutshellqt.h"


//---------------------------------------------------------------------------
nutshellOptions::nutshellOptions(QWidget *parent) :
		QDialog(parent)
{
	setupUi(this);
    this->setWindowTitle("Directories for the GIS software");
    this->setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint);

    connect(toolButton_2, SIGNAL(clicked()), this, SLOT(findCondaDir()));

    connect(toolButton_1, SIGNAL(clicked()), this, SLOT(findPcrcalcDir()));
    connect(toolButton_4, SIGNAL(clicked()), this, SLOT(findGDALDir()));

	baseDirs.clear();
}
//---------------------------------------------------------------------------
nutshellOptions::~nutshellOptions()
{
}
//---------------------------------------------------------------------------
void nutshellOptions::on_checkBoxInstallation_stateChanged(int state)
{
    CondaInstall = state > 0;

    lineEdit_pcrcalcDir->setEnabled(!CondaInstall);
    lineEdit_GDALDir->setEnabled(!CondaInstall);
    toolButton_1->setEnabled(!CondaInstall);
    toolButton_4->setEnabled(!CondaInstall);
    label_3->setEnabled(!CondaInstall);
    label_4->setEnabled(!CondaInstall);
    label_5->setEnabled(!CondaInstall);

    lineEdit_CondaDir->setEnabled(CondaInstall);
    label_3->setEnabled(CondaInstall);
    toolButton_2->setEnabled(CondaInstall);
}
//---------------------------------------------------------------------------
void nutshellOptions::setupOptions(QStringList SL, double dpi, bool CI)
{
	baseDirs = SL;
	lineEdit_pcrcalcDir->setText(SL[0]);
    lineEdit_GDALDir->setText(SL[1]);
    lineEdit_CondaDir->setText(SL[2]);
    dpiScale->setValue(dpi);
    CondaInstall = CI;

    on_checkBoxInstallation_stateChanged(CI ? 1 : 0);
}
//---------------------------------------------------------------------------
QStringList nutshellOptions::getOptions()
{
    // after closing options get the results

	baseDirs.clear();
	baseDirs << lineEdit_pcrcalcDir->text()
            << lineEdit_GDALDir->text()
            << lineEdit_CondaDir->text()
            << dpiScale->text()
            << QString(CondaInstall ? "1" : "0");

    if (lineEdit_pcrcalcDir->text().isEmpty() && lineEdit_CondaDir->text().isEmpty()) {
        WarningMsg("You have not specified a valid PCRaster directory, GIS operations will not work!");
    }

    return baseDirs;
}
//---------------------------------------------------------------------------
void nutshellOptions::findCondaDir()
{
    QString dir;
    QString olddir = lineEdit_CondaDir->text();

    dir = setExistingDirectory("Select (Mini)Conda environment folder that has PCRaster",baseDirs[2]);

    QString str = dir+"\\Library\\bin\\pcrcalc.exe";
    if (!QFileInfo(str).exists()) {
        WarningMsg(QString("The selected environment %1 does not contain a PCRaster installation").arg(str));
    } else {
        baseDirs[2] = dir;//+"/Library/bin/";
        lineEdit_CondaDir->setText(dir);
    }
    qDebug()<<  "miniconda found: "<< dir;
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
void nutshellOptions::findGDALDir()
{
    QString dir;
    QString olddir = lineEdit_GDALDir->text();
    dir = setExistingDirectory("GDAL main dir",baseDirs[1]);

    dir.replace("\\","/");
    if (!dir.endsWith("/"))
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
   QString directory =
            QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, title, bd, QFileDialog::ShowDirsOnly));

	if (!directory.isEmpty())
		return(directory);
	else
		return "";
}
//---------------------------------------------------------------------------
