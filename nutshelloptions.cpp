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

    //lineEdit_CondaDir->setEnabled(CondaInstall);
    label_3->setEnabled(CondaInstall);
    toolButton_2->setEnabled(CondaInstall);
}
//---------------------------------------------------------------------------
void nutshellOptions::setupOptions(QStringList SL, double dpi, bool CI)
{
	baseDirs = SL;
	lineEdit_pcrcalcDir->setText(SL[0]);
    lineEdit_GDALDir->setText(SL[1]);
  //  lineEdit_CondaDir->setText(SL[2]);
    combo_envs->insertItem(0,SL[2]);
    dpiScale->setValue(dpi);
    CondaInstall = CI;

    on_checkBoxInstallation_stateChanged(CI ? 1 : 0);
}
//---------------------------------------------------------------------------
QStringList nutshellOptions::getOptions()
{
    // after closing options get the results
    CondaInstall = findCondaDir();
	baseDirs.clear();
	baseDirs << lineEdit_pcrcalcDir->text()
            << lineEdit_GDALDir->text()
            << CondaBaseDirName
            << dpiScale->text()
            << QString(CondaInstall ? "1" : "0");

    return baseDirs;
}
//---------------------------------------------------------------------------
bool nutshellOptions::findCondaDir()
{
    CondaInstall = GetCondaAllEnvs(0);
    CondaInstall = CondaInstall || GetCondaAllEnvs(1);
    CondaInstall = CondaInstall || GetCondaAllEnvs(2);
    CondaInstall = CondaInstall || GetCondaAllEnvs(3);
    qDebug() << CondaInstall;

    if (!CondaInstall) {
        QMessageBox msgBox;
        msgBox.setText("No valid anaconda or miniconda installation is found. Follow the instructions on the PCRaster installation website.");
        msgBox.exec();

    }

    CondaBaseDirName = combo_envs->currentText();
    return(CondaInstall);
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
bool nutshellOptions::GetCondaAllEnvs(int cda)
{
    bool install = false;
    QString name = qgetenv("USER");
    if (name.isEmpty())
        name = qgetenv("USERNAME");

    if (cda == 0) {
        CondaBaseDirName = QString("c:/Users/" +name + "/Miniconda3/envs");
    }
    if (cda == 1) {
        CondaBaseDirName = QString("c:/Users/" +name + "/Anaconda3/envs");
    }
    if (cda == 2) {
        CondaBaseDirName = QString("c:/ProgramData/Miniconda3/envs");
    }
    if (cda == 3) {
        CondaBaseDirName = QString("c:/ProgramData/Anaconda3/envs");
    }

    if (QFileInfo(CondaBaseDirName).dir().exists()) {
        QDir const source(CondaBaseDirName);
        QStringList const folders = source.entryList(QDir::NoDot | QDir::NoDotDot | QDir::Dirs);
        for (int i = 0; i < folders.size(); i++) {
            QString str = CondaBaseDirName+"/"+folders.at(i)+"/python.exe";
            QString str1 = CondaBaseDirName+"/"+folders.at(i)+"/Library/bin/pcrcalc.exe";
            QString str2 = CondaBaseDirName+"/"+folders.at(i)+"/Library/bin/gdalinfo.exe";
            bool pythonfound = QFileInfo(str).exists();
            bool pcrasterfound = QFileInfo(str1).exists();
            bool gdalfound = QFileInfo(str2).exists();

            if (pythonfound && pcrasterfound && gdalfound)
                combo_envs->addItem(CondaBaseDirName+"/"+folders.at(i));
            else {
                QString error;
                if (!pythonfound) error = QString("Python not found in Anaconda environment "+folders.at(i)+"\nThis environment is ignored.");
                else
                if (!pcrasterfound) error = QString("PCRaster not found in Anaconda environment "+folders.at(i)+"\nThis environment is ignored.");
                else
                if (!gdalfound) error = QString("GDAL not found in Anaconda environment "+folders.at(i)+"\nThis environment is ignored.");

                QMessageBox msgBox;
                msgBox.setText(error);
                msgBox.exec();
            }
        }
        install = combo_envs->count() > 0;
    }
    return(install);
}
