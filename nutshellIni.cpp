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
    if (CondaInstall) {
        PCRasterAppDirName = CondaDirName+"Library/bin/";
        AguilaDirName = PCRasterAppDirName;
        GDALAppDirName =PCRasterAppDirName;
    }
    else
    if (PCRasterInstall){
        if(!PCRasterDirName.isEmpty())
        {
            PCRasterDirName.replace("\\","/");
            if (!PCRasterDirName.endsWith("/"))
                PCRasterDirName = PCRasterDirName + "/";
            PCRasterAppDirName = PCRasterDirName+"bin/";

            GDALDirName.replace("\\","/");
            if (!GDALDirName.endsWith("/"))
                GDALDirName = GDALDirName + "/";
            GDALAppDirName = GDALDirName + "bin/gdal/apps/";

            AguilaDirName = PCRasterAppDirName;
        }
        else {
            WarningMsg("Set the path to a PCRaster installation directory in File->options");
        }
    }

    MapeditDirName = QCoreApplication::applicationDirPath() + "/";
}
//---------------------------------------------------------------
void nutshellqt::setNutshellIni()
{
    QSettings settings(qApp->applicationDirPath()+"/NutShell.ini",QSettings::IniFormat);
    settings.clear();

    settings.setValue("PCRasterDirectory", PCRasterDirName);
    settings.setValue("GDALDirectory", GDALDirName);
   // settings.setValue("CondaDirectory", CondaDirName);
    settings.setValue("DPI",dpiscale);
    settings.setValue("CondaInstall",CondaInstall);
    settings.setValue("PCRasterInstall",PCRasterInstall);
    settings.setValue("workdir",currentPath);

    //settings.setValue(QString("workdir/current"),comboBox_workdir->currentIndex());
//    for (int i = 0; i < comboBox_workdir->count(); i++)
//    {
//    //    qDebug() << comboBox_workdir->itemText(i);
//        if (i == comboBox_workdir->currentIndex())
//            settings.setValue(QString("workdir/workdir%1").arg(i),comboBox_workdir->itemText(i)+"<");
//        else
//            settings.setValue(QString("workdir/workdir%1").arg(i),comboBox_workdir->itemText(i));
//    }

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

    QString str = settings.value("PCRasterDirectory").toString();
    if (!str.isEmpty())
        if (!str.endsWith("\\") && !str.endsWith("/"))
            str = str + "/";
    PCRasterDirName = str;
    str = settings.value("GDALDirectory").toString();
    if (!str.isEmpty())
        if (!str.endsWith("\\") && !str.endsWith("/"))
            str = str + "/";
    GDALDirName = str;
//    str = settings.value("CondaDirectory").toString();
//    if (!str.isEmpty())
//        if (!str.endsWith("\\") && !str.endsWith("/"))
//            str = str + "/";
//    CondaDirName = str;

    dpiscale = settings.value("DPI").toDouble();

    CondaInstall = settings.value("CondaInstall").toString() == "true";
    PCRasterInstall = settings.value("PCRasterInstall").toString() == "true";

    str = settings.value("workdir").toString();
    if (!str.isEmpty())
        if (!str.endsWith("\\") && !str.endsWith("/"))
            str = str + "/";
    currentPath = str;
    comboBox_workdir->addItem(currentPath);

    settings.beginGroup("models");
    QStringList keys = settings.childKeys();
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
