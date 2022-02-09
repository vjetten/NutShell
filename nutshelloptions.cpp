#include "nutshellqt.h"


//---------------------------------------------------------------------------
nutshellOptions::nutshellOptions(QWidget *parent) :
		QDialog(parent)
{
	setupUi(this);
    this->setWindowTitle("Directories for the GIS software");
    this->setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint);

  //  connect(toolButton_2, SIGNAL(clicked()), this, SLOT(findCondaDir()));

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
void nutshellOptions::setupOptions(QStringList SL)
{
	baseDirs = SL;
    CondaInstall = SL[4].toInt() == 1;
    dpiScale->setValue(SL[3].toDouble());
    if (!CondaInstall) {
        lineEdit_pcrcalcDir->setText(SL[0]);
        lineEdit_GDALDir->setText(SL[1]);
    } else
        combo_envs->insertItem(0,SL[2]);
    on_checkBoxInstallation_stateChanged(CondaInstall ? 1 : 0);
}
//---------------------------------------------------------------------------
QStringList nutshellOptions::getOptions()
{
    // after closing options get the results
    CondaInstall = findCondaDir();
	baseDirs.clear();
	baseDirs << lineEdit_pcrcalcDir->text()
            << lineEdit_GDALDir->text()
            << CondaDirName
            << dpiScale->text()
            << QString(CondaInstall ? "1" : "0");

    return baseDirs;
}
//---------------------------------------------------------------------------
bool nutshellOptions::findCondaDir()
{
    CondaInstall = GetCondaAllEnvs();
    //qDebug() << CondaInstall << CondaDirName;

    if (!CondaInstall) {
        QMessageBox msgBox;
        msgBox.setText("No anaconda or miniconda installation with Python, PCRaster and GDAL is found. Follow the instructions on the PCRaster installation website.");
        msgBox.exec();
        CondaDirName = "";
    } else
        CondaDirName = combo_envs->currentText();
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
bool nutshellOptions::GetCondaAllEnvs()
{
    QString CondaBaseDirName;
    QString name = qgetenv("USERPROFILE");
    QString name1 = qgetenv("ALLUSERSPROFILE");
    combo_envs->clear();

    if (name.isEmpty())
        name = qgetenv("USERNAME");
    name.replace("\\","/");
    name1.replace("\\","/");

    name = name+"/.conda/environments.txt";
    name1 = name1+"/.conda/environments.txt";
    QStringList folders;
    if (QFileInfo(name).exists()) {
        QFile fin(name);

        if (fin.open(QIODevice::ReadOnly | QIODevice::Text)){
            while (!fin.atEnd())
            {
                QString S = fin.readLine();
                if (!S.trimmed().isEmpty()) {
                    S.remove(QChar('\n'));
                    S.replace("\\","/");
                    S = S + "/";
                    folders << S;

                }
            }
        }
        fin.close();
    }
    for (int i = 0; i < folders.size(); i++) {
        QString str = folders.at(i)+"python.exe";
        QString str1 = folders.at(i)+"Library/bin/pcrcalc.exe";
        QString str2 = folders.at(i)+"Library/bin/gdalinfo.exe";
        bool pythonfound = QFileInfo(str).exists();
        bool pcrasterfound = QFileInfo(str1).exists();
        bool gdalfound = QFileInfo(str2).exists();

        if (pythonfound && pcrasterfound && gdalfound)
            combo_envs->addItem(folders.at(i));
        else {
            QString error;
            if (!pythonfound) error = QString("Python not found in Anaconda environment "+folders.at(i)+"\nThis environment is ignored.");
            else
                if (!pcrasterfound) error = QString("PCRaster not found in Anaconda environment "+folders.at(i)+"\nThis environment is ignored.");
                else
                    if (!gdalfound) error = QString("GDAL not found in Anaconda environment "+folders.at(i)+"\nThis environment is ignored.");
        }
    }
/*

    qDebug() << name << name1;
    for (int cda = 0; cda < 4; cda++) {
        if (cda == 0) {
            //CondaBaseDirName = QString("c:/Users/" +name + "/Miniconda3/envs");
            CondaBaseDirName = QString(name + "/Miniconda3/envs");
        }
        if (cda == 1) {
//            CondaBaseDirName = QString("c:/Users/" +name + "/Anaconda3/envs");
            CondaBaseDirName = QString(name + "/Anaconda3/envs");
        }
        if (cda == 2) {
            //CondaBaseDirName = QString("c:/ProgramData/Miniconda3/envs");
            CondaBaseDirName = QString(name1 + "/Miniconda3/envs");
        }
        if (cda == 3) {
            //CondaBaseDirName = QString("c:/ProgramData/Anaconda3/envs");
            CondaBaseDirName = QString(name1+"/Anaconda3/envs");
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
                }
            }
        }
    }
    */
    if (combo_envs->count() > 0)
        CondaDirName = combo_envs->currentText();
    return(combo_envs->count() > 0);
}

void nutshellOptions::on_toolButton_2_clicked()
{
    QString filter = "all (*.*)";
    QString CondaBaseDirName = "c:/";
    QStringList dirs = QFileDialog::getOpenFileNames(this, QString("Select the 'envs' folder in your conda install directory"),CondaBaseDirName,filter);
    if (dirs.count() == 0)
        return;
    CondaBaseDirName = dirs.at(0);

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

            }
        }
    }
}

