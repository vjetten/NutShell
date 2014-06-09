/*
 * nutshellAction: used in the NutSHhell Explorer

 * perform an action on the explorer selection (aguia2D, aguila 3D etc)
 * valid strings are made in nutshellfilenames.cpp
 * Author: VJ 140222
 */


#include "nutshellqt.h"


//---------------------------------------------------------------


void nutshellqt::setPCRasterDirectories()
{
    if(!PCRasterDirName.isEmpty())
    {
        PCRasterDirName = QDir(PCRasterDirName).absolutePath()+ "/";
        PCRasterDocDirName = PCRasterDirName + "doc/pcrman/";
        PCRasterAppDirName = PCRasterDirName + "apps" + "/";

        // assume verssion 4.0.0
        if (!QFileInfo(PCRasterDocDirName+"index.html").exists())
        {
            PCRasterDocDirName = PCRasterDirName + "doc/manual/";
            PCRasterAppDirName = PCRasterDirName + "bin" + "/";
        }
        AguilaDirName = PCRasterAppDirName;
        MapeditDirName = PCRasterAppDirName + "nutshell/";
    }
    else
    {
        //check if the install has pcrcalc in the dir ../bin
        PCRasterAppDirName = QDir(qApp->applicationDirPath()+"/..").absolutePath() +"/";
        if (QFileInfo(PCRasterAppDirName+"/pcrcalc.exe").exists())
        {
            PCRasterDirName = QDir(PCRasterAppDirName+"/..").absolutePath()+"/";
            AguilaDirName = PCRasterAppDirName;
            MapeditDirName = PCRasterAppDirName + "nutshell/";
            PCRasterDocDirName = PCRasterDirName + "doc/manual/";
        }
        else
        {
            PCRasterAppDirName = "";
        }
     //   qDebug() << PCRasterDirName << PCRasterAppDirName << PCRasterDocDirName << MapeditDirName;
    }
}
//---------------------------------------------------------------
void nutshellqt::setNutshellIni()
{
    QSettings settings(qApp->applicationDirPath()+"/NutShell.ini",QSettings::IniFormat);
    settings.clear();
    //   settings.setValue("workDirectory", currentPath);
    settings.setValue("PCRasterDirectory", PCRasterDirName);
    settings.setValue("pcrcalcDirectory", PCRasterAppDirName);
    settings.setValue("aguilaDirectory", AguilaDirName);
    settings.setValue("mapeditDirectory", MapeditDirName);
    settings.setValue("docDirectory", PCRasterDocDirName);

    //settings.setValue(QString("workdir/current"),comboBox_workdir->currentIndex());
    for (int i = 0; i < comboBox_workdir->count(); i++)
    {
    //    qDebug() << comboBox_workdir->itemText(i);
        if (i == comboBox_workdir->currentIndex())
            settings.setValue(QString("workdir/workdir%1").arg(i),comboBox_workdir->itemText(i)+"<");
        else
            settings.setValue(QString("workdir/workdir%1").arg(i),comboBox_workdir->itemText(i));
    }

    //settings.setValue(QString("modelnr/active"),tabWidget->currentIndex());
    settings.setValue("models/current",tabWidget->currentIndex());
    settings.setValue("models/arg_substitute_do",toolButton_argSubs->isChecked());
    settings.setValue("models/arg_substitute",lineEdit_argsubst->text());
    for (int i = 0; i < ET.count(); i++)
    {
        if (!ETfileName.contains("empty"))
            settings.setValue(QString("models/Script%1").arg(i),
                              QString("%1<%2").arg(ET[i].filePath).arg("1"));//.arg(ET[i].syntax));
    }

    for (int i = 0; i < comboBox_cmdlist->count(); i++)
    {
       // qDebug() << comboBox_cmdlist->itemText(i);
        settings.setValue(QString("cmdlist/Command%1").arg(i),comboBox_cmdlist->itemText(i));
    }

    settings.setValue("layout/splitter1", splitterExplorer->saveState());
    settings.setValue("layout/splitter2", splitterCommandwindow->saveState());
    settings.setValue("layout/splitter3", splitterEditor->saveState());
    settings.sync();
}
//---------------------------------------------------------------
void nutshellqt::clearNutshellIni()
{
    //    QSettings settings(QSettings::IniFormat,QSettings::UserScope,"NutSHell","NutSHellQt");
    //    settings.clear();
}
//---------------------------------------------------------------
void nutshellqt::getNutshellIni()
{
    //QSettings settings(QSettings::IniFormat,QSettings::UserScope,"NutSHell","NutShell");
    QSettings settings(qApp->applicationDirPath()+"/NutShell.ini",QSettings::IniFormat);
    //   currentPath = settings.value("workDirectory").toString();

    QChar cs = QDir::separator();
    QString str = settings.value("PCRasterDirectory").toString();
    if (!str.isEmpty())
        PCRasterDirName = str;
    str = settings.value("pcrcalcDirectory").toString();
    if (!str.isEmpty())
        PCRasterAppDirName = str;
    str = settings.value("aguilaDirectory").toString();
    if (!str.isEmpty())
        AguilaDirName = str;
    str = settings.value("mapeditDirectory").toString();
    if (!str.isEmpty())
        MapeditDirName = str;
    str = settings.value("docDirectory").toString();
    if (!str.isEmpty())
        PCRasterDocDirName = str;
    if (!PCRasterAppDirName.isEmpty() && !QFileInfo(PCRasterAppDirName + "/pcrcalc.exe").exists())
    {
        PCRasterDirName = "";
        PCRasterAppDirName = "";
        AguilaDirName = "";
        MapeditDirName = "";
        PCRasterDocDirName = "";
    }


    settings.beginGroup("workdir");
    QStringList keys = settings.childKeys();
    int currentworkdir = 0;
    comboBox_workdir->setInsertPolicy(QComboBox::InsertAtBottom);
    QStringList dirs;
    for (int i = 0; i < keys.count(); i++)
    {
        QString str = settings.value(keys[i]).toString().simplified();

        if (!str.isEmpty())
        {
            if(str.contains("<"))
            {
                str.remove(str.size()-1,1);
                currentworkdir = i;
            }
            //            if (str.simplified() == "")
            //                currentworkdir = 0;
            //            else
            if (str.simplified() != "")
                dirs << str;
        }
    }
    comboBox_workdir->clear();
    if (dirs.isEmpty())
        dirs << "C:/";

    comboBox_workdir->addItems(dirs);
    comboBox_workdir->setCurrentIndex(currentworkdir);
    currentPath = comboBox_workdir->itemText(currentworkdir);
    settings.endGroup();

    //    settings.beginGroup("layout");
    //    splitter->restoreState(settings.value("splitter1").toByteArray());
    //    splitter_2->restoreState(settings.value("splitter2").toByteArray());
    //    splitter_3->restoreState(settings.value("splitter3").toByteArray());
    //    settings.endGroup();

    settings.beginGroup("models");
    keys = settings.childKeys();
    int currentmodel = 0;
    for (int i = 0; i < keys.count(); i++)
    {
        if (keys[i] == "current")
        {
            int nr = settings.value(keys[i]).toInt();
            if (nr >= 0)
                currentmodel = nr;
        }
        else
            if (keys[i] == "arg_substitute")
                lineEdit_argsubst->setText(settings.value(keys[i]).toString());
            else
                if (keys[i] == "arg_substitute_do")
                    toolButton_argSubs->setChecked(settings.value(keys[i]).toBool());
                else
                {
                    QString name = settings.value(keys[i]).toString();
                    QStringList list = name.split("<");
                    AddModel(list[0],1);// list[1].toInt());
                }
    }
    tabWidget->setCurrentIndex(currentmodel);
    settings.endGroup();

    settings.beginGroup("cmdlist");
    keys = settings.childKeys();
    for (int i = 0; i < keys.count(); i++)
    {
        comboBox_cmdlist->addItem(settings.value(keys[i]).toString());
    }
    settings.endGroup();
}
//---------------------------------------------------------------
void nutshellqt::getRegPCRaster()
{

    PCRasterDirName = "";
    // for pcraster iunstall previous to version 4.0.0

    bool regv3 = false;
    bool regv2 = false;

    QSettings settings(QSettings::SystemScope,"PCRaster","Install");
    QSettings rsettings("HKEY_CURRENT_USER\\Software\\PCRaster\\PCRaster", QSettings::NativeFormat);

    if (rsettings.contains("Default"))
    {
        regv3 = true;
        PCRasterDirName = rsettings.value("Default", "").toString();
    }

    if (!regv3 && settings.contains("Default"))
    {
        regv2 = true;
        PCRasterDirName = settings.value("Default", "").toString();
    }

    if (!regv3 && !regv2)
        return;

    WarningMsg(QString("Old PCRaster version found. This will probably work but you should upgrade to version 4:? pcraster.geo.uu.nl"));

    QDir dir(PCRasterDirName);

    if (!dir.exists() || PCRasterDirName.isEmpty())
    {
        ErrorMsg(QString("PCRaster directory not found: %1\nSet dirs in File->Options").arg(PCRasterDirName));
        return;
    }
}
//---------------------------------------------------------------
