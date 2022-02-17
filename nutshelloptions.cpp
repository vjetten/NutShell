#include "nutshellqt.h"


//---------------------------------------------------------------------------
nutshellOptions::nutshellOptions(QWidget *parent) :
		QDialog(parent)
{
	setupUi(this);
    this->setWindowTitle("Directories for the GIS software");
    this->setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint);

    connect(toolButton_1, SIGNAL(clicked()), this, SLOT(findPcrcalcDir()));
    connect(toolButton_4, SIGNAL(clicked()), this, SLOT(findGDALDir()));

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
    qDebug() << SL;
    CondaInstall = SL[0].toInt() == 1;
    if (!CondaInstall) {
        lineEdit_pcrcalcDir->setText(SL[2]);
        lineEdit_GDALDir->setText(SL[3]);
    } else {
        bool yes = false;
        for (int i = 0; i < combo_envs->count(); i++)
            if (combo_envs->itemText(i) == SL[1])
                yes = true;
        if (!yes)
            combo_envs->insertItem(0,SL[1]);
    }

    //on_checkBoxInstallation_stateChanged(CondaInstall ? 1 : 0);
    groupBox_Conda->setEnabled(CondaInstall);
    groupBox_PCR->setEnabled(!CondaInstall);
}
//---------------------------------------------------------------------------
QStringList nutshellOptions::getOptions()
{
    // after closing options get the results
    CondaInstall = findCondaDir();
	baseDirs.clear();
    baseDirs << QString(CondaInstall ? "1" : "0")
             << CondaDirName
            << lineEdit_pcrcalcDir->text()
            << lineEdit_GDALDir->text();
            //<< dpiScale->text()

    return baseDirs;
}
//---------------------------------------------------------------------------
bool nutshellOptions::findCondaDir()
{
    CondaInstall = GetCondaAllEnvs();

    if (!CondaInstall) {        
        WarningMsg("No valid anaconda or miniconda installation with Python, PCRaster and GDAL is found.\nFollow the instructions on https://pcraster.geo.uu.nl/.");
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
    QStringList folders;
    QString name = qgetenv("USERPROFILE");
  //  QString name1 = qgetenv("ALLUSERSPROFILE");
    combo_envs->clear();

    name.replace("\\","/");
    name = name+"/.conda/environments.txt";

    if (QFileInfo(name).exists()) {
        QFile fin(name);

        if (fin.open(QIODevice::ReadOnly | QIODevice::Text)){
            while (!fin.atEnd())
            {
                QString S = fin.readLine();
                if (S.contains("envs")) {
                    S.remove(QChar('\n'));
                    S.replace("\\","/");
                    S = S + "/";
                    folders << S;
                }
            }
        }
        fin.close();
    } else {
        WarningMsg("No conda environment found.");
        return false;
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
            QString error = "";
            if (!pythonfound) error = QString("Python not found in Anaconda environment "+folders.at(i)+"\nThis environment is ignored.");
            else
                if (!pcrasterfound) error = QString("PCRaster not found in Anaconda environment "+folders.at(i)+"\nThis environment is ignored.");
                else
                    if (!gdalfound) error = QString("GDAL not found in Anaconda environment "+folders.at(i)+"\nThis environment is ignored.");
//            if (!error.isEmpty())
//                WarningMsg(error);
        }
    }
    if (combo_envs->count() > 0)
        CondaDirName = combo_envs->currentText();

    return(combo_envs->count() > 0);

/*
THIS doesn't work when conda is not in users
    qDebug() << name << name1;
    for (int cda = 0; cda < 4; cda++) {
        if (cda == 0) {
            CondaBaseDirName = QString(name + "/Miniconda3/envs");
        }
        if (cda == 1) {
            CondaBaseDirName = QString(name + "/Anaconda3/envs");
        }
        if (cda == 2) {
            CondaBaseDirName = QString(name1 + "/Miniconda3/envs");
        }
        if (cda == 3) {
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
    if (combo_envs->count() > 0)
        CondaDirName = combo_envs->currentText();
    return(combo_envs->count() > 0);
    */
}

/*
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
*/

void nutshellOptions::on_groupBox_Conda_clicked(bool checked)
{
    CondaInstall = checked;
    groupBox_PCR->setEnabled(!CondaInstall);
    groupBox_PCR->setChecked(!CondaInstall);
}


void nutshellOptions::on_groupBox_PCR_clicked(bool checked)
{
    CondaInstall = !checked;
    groupBox_Conda->setEnabled(CondaInstall);
    groupBox_Conda->setChecked(CondaInstall);
}


void nutshellOptions::on_toolButton_1_clicked()
{
    QStringList filters({"Any files (*)"});
    QString S = lineEdit_pcrcalcDir->text();
    QString FileName = getFileorDir(S,"Select the PCRaster installation directory", filters, 0);
    if (!FileName.isEmpty()) {
        lineEdit_pcrcalcDir->setText(FileName);
        //setIniStart();
    }
}


void nutshellOptions::on_toolButton_4_clicked()
{
    QStringList filters({"Any files (*)"});
    QString S = lineEdit_GDALDir->text();
    QString FileName = getFileorDir(S,"Select the PCRaster installation directory", filters, 0);
    if (!FileName.isEmpty()) {
        lineEdit_GDALDir->setText(FileName);
        //setIniStart();
    }

}

// select a file or directory
// doFile = 0: select a directory;
// dofile = 1 select a file and return file name only;
// dofile = 2 return filename wioth full path
QString nutshellOptions::getFileorDir(QString inputdir,QString title, QStringList filters, int doFile)
{
    QFileDialog dialog;
    QString dirout = inputdir;
    qDebug() <<"dir" << inputdir << QFileInfo(inputdir).absoluteDir();
    if (doFile > 0) {
        dialog.setNameFilters(filters);
        dialog.setDirectory(QFileInfo(inputdir).absoluteDir());
        dialog.setFileMode(QFileDialog::ExistingFile);
    } else {
        // get a file
        filters.clear();
        dialog.setNameFilters(filters);
        dialog.setDirectory(QFileInfo(inputdir).absoluteDir());
        dialog.setFileMode(QFileDialog::DirectoryOnly);
    }

    dialog.setLabelText(QFileDialog::LookIn,title);
    dialog.exec();

    if (doFile > 0) {
        dirout = "";
        if (dialog.selectedFiles().count() > 0)
            dirout = dialog.selectedFiles().at(0);
        if (doFile == 1)
            dirout = QFileInfo(dirout).fileName();
        if (doFile == 2)
            dirout = QFileInfo(dirout).absoluteFilePath();
        qDebug() << dirout;
    } else {
        QString S = dialog.selectedUrls().at(0).path();
        S.remove(0,1);
        if (!S.isEmpty())
            dirout = S;
        if (!dirout.endsWith('/') && !dirout.endsWith('\\'))
            dirout = dirout + "/";
    }

    return dirout;
}
