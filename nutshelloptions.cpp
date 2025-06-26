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
   // CondaInstall = findCondaDir(S);
    combo_envs->setCurrentText(S);
}
//---------------------------------------------------------------------------
QString nutshellOptions::getOptions()
{
    // after closing options get the results    
    CondaInstall = findCondaDir(CondaDirName);
    QFileInfo inff(CondaDirName);
    if (CondaDirName.isEmpty() || !inff.exists() || !inff.isDir()) {
        ErrorMsg("Invalid conda install. NutShell will not work!");
        CondaInstall = false;
    }

    return CondaDirName;
}
//---------------------------------------------------------------------------
bool nutshellOptions::findCondaDir(QString dirS)
{
    CondaInstall = GetCondaAllEnvs();
    if (CondaInstall) {
        for (int i; i < combo_envs->count(); i++) {
             if (dirS == combo_envs->itemText(i))
                 combo_envs->setCurrentText(dirS);
        }
    } else {
        WarningMsg("No valid Anaconda or Miniconda installation with Python, PCRaster and GDAL is found.\nFollow the instructions on https://pcraster.geo.uu.nl/.");
        CondaDirName = "";        
    }
    return(CondaInstall);
}
//---------------------------------------------------------------------------
bool nutshellOptions::GetCondaAllEnvs()
{
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
                    allCondaDirNames << S;
                }
            }
        }
        fin.close();
    } else {
        WarningMsg("No conda environment found.");
        return false;
    }

    for (int i = 0; i < allCondaDirNames.size(); i++) {
        QString str = allCondaDirNames.at(i)+"python" + exeSuffix;
        QString str1 = allCondaDirNames.at(i)+"Library/bin/pcrcalc" + exeSuffix;
        QString str2 = allCondaDirNames.at(i)+"Library/bin/gdalinfo" + exeSuffix;
        bool pythonfound = QFileInfo(str).exists();
        bool pcrasterfound = QFileInfo(str1).exists();
        bool gdalfound = QFileInfo(str2).exists();

        if (pythonfound && pcrasterfound && gdalfound)
            combo_envs->addItem(allCondaDirNames.at(i));
        else {
            QString error = "";
            if (!pythonfound) error = QString("Python not found in Anaconda environment "+allCondaDirNames.at(i)+"\nThis environment is ignored.");
            else
                if (!pcrasterfound) error = QString("PCRaster not found in Anaconda environment "+allCondaDirNames.at(i)+"\nThis environment is ignored.");
                else
                    if (!gdalfound) error = QString("GDAL not found in Anaconda environment "+allCondaDirNames.at(i)+"\nThis environment is ignored.");
        }
    }
    allCondaDirNames.clear();
    condalist->clear();
    condalist->appendPlainText("Conda environments with GDAL and PCRaster:");
    for (int i = 0; i < combo_envs->count(); i++) {
        allCondaDirNames << combo_envs->itemText(i);
        condalist->appendPlainText(">" + combo_envs->itemText(i));
    }

    return(combo_envs->count() > 0);
}

void nutshellOptions::on_combo_envs_currentTextChanged(const QString &arg1)
{
    CondaDirName = arg1;
}

