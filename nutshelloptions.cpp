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
        for (int i = 0; i < combo_envs->count(); i++) {
             if (dirS == combo_envs->itemText(i))
                 combo_envs->setCurrentText(dirS);
        }
    } else {
        // QString S =
        //     "No valid Anaconda or Miniconda installation with Python, PCRaster and GDAL is found.\n"
        //     "Do the following:\n"
        //     "Download and install miniforge from https://conda-forge.org/download/\n"
        //     "Open and make a new environment (e.g. \"nutshell\" or any name you want):\n"
        //     "conda create -n nutshell\n"
        //     "conda activate nutshell\n"
        //     "conda install -c conda-forge pcraster owslib scipy gdal soilgrids\n"
        //     "After that open NutShell again.";


        // WarningMsg(S);
        CondaDirName = "";
    }
    return(CondaInstall);
}
//---------------------------------------------------------------------------
bool nutshellOptions::GetCondaAllEnvs()
{
    combo_envs->clear();

    // Get home directory in a cross-platform way
    QString homeDir = QDir::homePath();
    QString envsFile = homeDir + "/.conda/environments.txt";

    if (!QFileInfo(envsFile).exists()) {
        WarningMsg("No conda environment found.");
        return false;
    }

    QFile fin(envsFile);
    if (fin.open(QIODevice::ReadOnly | QIODevice::Text)){
        while (!fin.atEnd())
        {
            QString S = fin.readLine();
            if (S.contains("envs")) {
                S.remove(QChar('\n'));
                S.replace("\\","/");
                if (!S.endsWith("/")) S += "/";
                allCondaDirNames << S;
            }
        }
        fin.close();
    } else {
        WarningMsg("Warrning: No valid conda environment found.");
        return false;
    }

    for (int i = 0; i < allCondaDirNames.size(); i++) {
        QString base = allCondaDirNames.at(i);

        // On Windows, executables are in Library/bin; on Linux, in bin
#ifdef Q_OS_WIN
        QString pythonPath = base + "python" + exeSuffix;
#else
        QString pythonPath = base + "bin/python" + exeSuffix;
#endif
       QString pcrPath = base + libPath + "/pcrcalc" + exeSuffix;
       QString gdalPath = base + libPath + "/gdalinfo" + exeSuffix;


        bool pythonfound = QFileInfo(pythonPath).exists();
        bool pcrasterfound = QFileInfo(pcrPath).exists();
        bool gdalfound = QFileInfo(gdalPath).exists();

        if (pythonfound && pcrasterfound && gdalfound)
        //if (pcrasterfound)
            combo_envs->addItem(base);
        else {
            QString error = "";
            if (!pythonfound) error = QString("Python not found in Anaconda environment "+base+"\nThis environment is ignored.");
            else if (!pcrasterfound) error = QString("PCRaster not found in Anaconda environment "+base+"\nThis environment is ignored.");
            else if (!gdalfound) error = QString("GDAL not found in Anaconda environment "+base+"\nThis environment is ignored.");
            // Optionally show or log error
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

