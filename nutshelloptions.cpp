#include "nutshellqt.h"


//---------------------------------------------------------------------------
nutshellOptions::nutshellOptions(QWidget *parent) :
		QDialog(parent)
{
	setupUi(this);
    this->setWindowTitle("Conda install environment");
    this->setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
}
//---------------------------------------------------------------------------
nutshellOptions::~nutshellOptions()
{
}

//---------------------------------------------------------------------------
void nutshellOptions::setOptions(QString S)
{
    CondaInstall = findCondaDir();
    combo_envs->setCurrentText(S);
}
//---------------------------------------------------------------------------
QString nutshellOptions::getOptions()
{
    // after closing options get the results
    CondaInstall = findCondaDir();
    QFileInfo inff(CondaDirName);
    if (CondaDirName.isEmpty() || !inff.exists() || !inff.isDir()) {
        ErrorMsg("Invalid conda install. NutShell will not work!");
        CondaInstall = false;
    }

    return CondaDirName;
}
//---------------------------------------------------------------------------
bool nutshellOptions::findCondaDir()
{
    CondaInstall = GetCondaAllEnvs();

    if (!CondaInstall) {        
        WarningMsg("No valid Anaconda or Miniconda installation with Python, PCRaster and GDAL is found.\nFollow the instructions on https://pcraster.geo.uu.nl/.");
        CondaDirName = "";
    } else
        CondaDirName = combo_envs->currentText();
    return(CondaInstall);
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
        }
    }
    if (combo_envs->count() > 0)
        CondaDirName = combo_envs->currentText();

    return(combo_envs->count() > 0);
}
