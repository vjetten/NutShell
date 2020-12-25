/*
 * nutshellAction: used in the NutSHhell Explorer

 * perform an action on the explorer selection (aguia2D, aguila 3D etc)
 * valid strings are made in nutshellfilenames.cpp
 * Author: VJ 140222,181001
 */


#include "nutshellqt.h"

QStringList nutshellqt::setEnvironment()
{
    QStringList env;
    env << QString("PATH=") + GDALDirName+QString("bin;")+ GDALDirName+QString("bin/gdal/apps;")+ PCRasterAppDirName;
    env << QString("set GDAL_DATA=") + GDALDirName + QString("bin/gdal-data");
    return env;
}
//---------------------------------------------------------------------------
// the following functions determine which action to take
// when a toolbutton is pressed or enter or doubleclick on the fileView is executed
void nutshellqt::actionaguila2D()
{
    PerformAction(ACTIONTYPEAGUILA2D);
}
//---------------------------------------------------------------------------
void nutshellqt::actionaquila3D()
{
    PerformAction(ACTIONTYPEAGUILA3D);
}
//---------------------------------------------------------------------------
void nutshellqt::actionaguila3Drape()
{
    PerformAction(ACTIONTYPEDRAPE);
}
//---------------------------------------------------------------------------
void nutshellqt::actionaguilaplot()
{
    PerformAction(ACTIONTYPETIMEPLOT);
}
//---------------------------------------------------------------------------
void nutshellqt::actionplusaguila(bool check)
{
    if (check)
        plus = "!+!";
    else
        plus = "!";
}
//---------------------------------------------------------------------------
void nutshellqt::actionlegend()
{
    PerformAction(ACTIONTYPELEGEND);
}
//---------------------------------------------------------------------------
void nutshellqt::actionmapattribute()
{
    PerformAction(ACTIONTYPEATTRIBUTE);
}
//---------------------------------------------------------------------------
void nutshellqt::actionmapnew()
{
    PerformAction(ACTIONTYPEATTRIBUTENEW);
}
//---------------------------------------------------------------------------
void nutshellqt::actioneditor()
{
    PerformAction(ACTIONTYPEMODEL);
}
//---------------------------------------------------------------------------
void nutshellqt::actionmapedit()
{
    PerformAction(ACTIONTYPEMAPEDIT);
}
//---------------------------------------------------------------------------
void nutshellqt::actionmapDisplay()
{
    PerformAction(ACTIONTYPEMAPDISPLAY);
}
//---------------------------------------------------------------------------
void nutshellqt::actionmapMap2Tiff()
{
    PerformAction(ACTIONTYPEMAP2TIFF);
}
//---------------------------------------------------------------------------
void nutshellqt::actionmapMap2Ilwis()
{
    PerformAction(ACTIONTYPEMAP2ILWIS);
}
//---------------------------------------------------------------------------
void nutshellqt::createBatch(QString sss, QString args)
{
    QFile efout(MapeditDirName+"_nutshell_batchjob.cmd");
    efout.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream eout(&efout);
    eout << "@echo off\n";
    eout << "CD "+currentPath +"\n";
    if (!CondaInstall) {
        eout << QString("PATH=") + GDALDirName+QString("bin;")+ GDALDirName+QString("bin/gdal/apps;")+ PCRasterAppDirName + "\n";
        eout << QString("set GDAL_DATA=") + GDALDirName + QString("bin/gdal-data\n");
    } else
        eout << QString("PATH=") + PCRasterAppDirName + "\n";
    eout << "@echo on\n";
    eout << "call \"" + sss + "\" " + args;

    efout.flush();
    efout.close();
}
//---------------------------------------------------------------------------
void nutshellqt::deleteBatch()
{
    QFile efout(MapeditDirName+"_nutshell_batchjob.cmd");
 //   efout.remove();
}
//---------------------------------------------------------------------------
// get actiontype if double click or <enter> keypress in fileView
int nutshellqt::GetActionType()
{
    int at;

    QString ext = SelectedSuffix; //QFileInfo(SelectedPathName).suffix();
    MAP *m = Mopen(SelectedPathName.toLatin1(),M_READ);

    if (m != nullptr || ext.toUpper() == "TIF" || ext.toUpper() == "MPR")
    {
        at = ACTIONTYPEAGUILA2D; //ACTIONTYPEDISPLAY;
        Mclose(m);
        m=nullptr;
    }
    else
        //		if (ext.toUpper() == "TSS")
        if( isTSSfile(SelectedPathName))
            at = ACTIONTYPETIMEPLOT;
        else
            if (ext.toUpper() == "BAT" || ext.toUpper() == "CMD")
                at = ACTIONTYPEWINDOWSCMD;
        else
            if (ext.toUpper() == "PCR")
                at = ACTIONTYPEPCRASTER;
            else
                if (ext.toUpper() == "MOD")
                {
                    at = ACTIONTYPEMODEL;
                    syntaxAct->setChecked(true);
                }
                else
                    if (ext.toUpper() == "TXT" ||
                            ext.toUpper() == "TBL" ||
                            ext.toUpper() == "DAT" ||
                            ext.toUpper() == "INI")
                    {
                        at = ACTIONTYPEMODEL;
                        syntaxAct->setChecked(false);
                    }
                    else
                        if (ext.toUpper() == "GST")
                            at = ACTIONTYPEGSTAT;
                        else
                            at = ACTIONTYPEUNDEFINED;

    return at;
}
//---------------------------------------------------------------------------
void nutshellqt::PerformAction(int actiontype)
{
    QString prog;
    QString cmdl;
    QStringList args;
    QString nameout;
    QString namein;
    MAP *m = nullptr;
    bool fileIsMap = true;
    bool isAguila = false;
    bool isTIFF = false;

    changeFileFilter(_filternr);
    args.clear();
    if(!selectionModel->currentIndex().isValid() && actiontype != ACTIONTYPEATTRIBUTENEW)
    {
        ErrorMsg("No file selected.");
        return;
    }
    // make a filelist string for aguila related actions
    if (actiontype != ACTIONTYPEATTRIBUTENEW &&
            actiontype != ACTIONTYPEATTRIBUTE &&
            actiontype != ACTIONTYPELEGEND &&
            actiontype != ACTIONTYPENONE)
        cmdl = getFileListString();
    // also makes mapseries if needed

    m = Mopen(SelectedPathName.toLatin1().data(),M_READ);
    if (m == nullptr)
        fileIsMap = false;
    else
        Mclose(m);

    isTIFF = SelectedSuffix.toUpper() == "TIF";// ||
      //      QFileInfo(SelectedPathName).suffix().toUpper() == "MPR";
    // geotiff or ILWIS file only from extension

    // check if selection is a pcraster map
    if (!fileIsMap && !isTIFF && (
                actiontype == ACTIONTYPEDISPLAY ||
                actiontype == ACTIONTYPEAGUILA3D ||
                actiontype == ACTIONTYPEAGUILA2D ||
                actiontype == ACTIONTYPEDRAPE ||
                actiontype == ACTIONTYPEMAPEDIT ||
                actiontype == ACTIONTYPEMAP2TIFF ||
              //  actiontype == ACTIONTYPEMAP2ILWIS ||
                actiontype == ACTIONTYPELEGEND ))
        //|| actiontype == ACTIONTYPEATTRIBUTE))
    {
        ErrorMsg(QString("%1 is not a PCRaster map.").arg(SelectedPathName));
        actiontype = ACTIONTYPENONE;
    }

    if (fileIsMap)
    {
        QString mapatts;
        mapatts = mapattribute.getMapAttributes(SelectedPathName);
        statusLabel.setText("<b>Map Attributes</b> - " + mapatts);
        statusBar()->addWidget(&statusLabel);
        statusLabel.show();
    }

//    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
//    QString str = GDALDirName+QString("bin;")+ GDALDirName+QString("bin/gdal/apps;")+ PCRasterAppDirName;
//    env.insert("PATH",str);
//    str = GDALDirName + QString("bin/gdal-data");
//    env.insert("GDAL_DATA",str);
//    PCRProcess->setProcessEnvironment(env);

    switch (actiontype)
    {
    case ACTIONTYPEAGUILA2D :
        args << "-2" << cmdl.split("!");
        //you cannot split on a space when the path name has a space in it!!!
        // but if you do not split aguila doesn't recognize two maps as one argument
        // so we use a character like ! to split and create the separate arguments

        prog = AguilaDirName + PCRxtr + "aguila.exe";
        isAguila = true;
        break;
    case ACTIONTYPEAGUILA3D :
        args << "-3" << cmdl.split(plus);
        // a split on plus will always show as a single 3D surface,
        // two maps as two seperate surfaces
        prog = AguilaDirName + PCRxtr + "aguila.exe";
        isAguila = true;
        break;
    case ACTIONTYPEDRAPE :
        if (cmdl.contains("!+!"))
            args << "-3" << cmdl.split("!");
        // cmdl already contains more maps with +, first map will be base
        else
        {
            // cmdl contains one or moremaps without +
            args = cmdl.split("!");
            // splt first if possible

            if (args.count() == 1)
                args << cmdl;
            // if one map is chosen double it to construct e.g. dem.map + dem.map

            cmdl = args.join("!+!");
            // join allways with a + for drape effect in aguila
            args.clear();
            args << "-3" << cmdl.split("!");
        }
        prog = AguilaDirName + "aguila.exe";
        isAguila = true;
        break;
    case ACTIONTYPETIMEPLOT :
        args << "-t" << cmdl.split("!");
        prog = AguilaDirName + "aguila.exe";
        isAguila = true;
        break;
    case ACTIONTYPEPCRASTER :
        args << cmdl;
        prog = PCRasterAppDirName + "pcraster.exe";
        break;
    case ACTIONTYPEMODEL :
        if (fileIsMap)
        {
            ErrorMsg(QString("%1 is a PCRaster map and cannot be loaded in the editor. Use MapEdit.").arg(SelectedFileName));
            actiontype = ACTIONTYPENONE;
        }
        else
        {
            AddModel(SelectedPathName,1);// dosyntax ? 1 : 0);
            actiontype = ACTIONTYPENONE;
        }
        break;
    case ACTIONTYPEMAPEDIT :
        if (!fileIsMap) {
            ErrorMsg(QString("%1 is not a PCRaster map and cannot be loaded in MapEdit.").arg(SelectedFileName));
            actiontype = ACTIONTYPENONE;
            break;
        }
//        mapedit.loadmap(SelectedPathName);
//        mapedit.show();
        args << cmdl;
        prog = MapeditDirName + "mapedit.exe";
        break;
    case ACTIONTYPELEGEND:
        if (fileIsMap)
        {
            m = Mopen(SelectedPathName.toLatin1().data(),M_READ);
            if (RgetValueScale(m) == VS_NOMINAL ||
                    RgetValueScale(m) == VS_ORDINAL ||
                    RgetValueScale(m) == VS_BOOLEAN)
            {
                maplegend.makelegend(SelectedPathName);
                maplegend.show();
                //maplegend.raise();
            }
            else
                ErrorMsg("Only nominal, ordinal or boolean maps can have a legend.");
            Mclose(m);
        }
        else
            ErrorMsg("Error opening file as PCRaster map.");
        m = nullptr;
        actiontype = ACTIONTYPENONE;
        break;
    case ACTIONTYPEATTRIBUTENEW :
        if (fileIsMap)
        {
            if (mapattribute.fill(SelectedPathName, true) == 0)
            {
                mapattribute.show();
                mapattribute.raise();
            }
        }
        else
            if (mapattribute.fill(QString(currentPath+"/new.map_vj"), true) == 0)
        {
            mapattribute.show();
            mapattribute.raise();
        }
        //         ErrorMsg("Error opening file as PCRaster map.");
        actiontype = ACTIONTYPENONE;
        break;
    case ACTIONTYPEATTRIBUTE :
        if (isTIFF) {
            args << SelectedPathName;
            prog = GDALAppDirName + "gdalinfo.exe";
            qDebug() << prog << args;
        } else
        if (fileIsMap) {
            if (mapattribute.fill(SelectedPathName, false) == 0) {
                mapattribute.show();
                mapattribute.raise();
                actiontype = ACTIONTYPENONE;
            }
        } else {
            statusBar()->removeWidget(&statusLabel);
             ErrorMsg("File is not recognised as PCRaster or GeoTIFF map.p");
        }
        break;
    case ACTIONTYPEGSTAT :
        args << cmdl;
        prog = PCRasterAppDirName + "gstat.exe";
        //actiontype = ACTIONTYPENONE;
        break;
    case ACTIONTYPEMAP2ILWIS:
        nameout = QFileInfo(SelectedPathName).baseName() + ".mpr";
        namein =  QFileInfo(SelectedPathName).baseName() + "." + SelectedSuffix;//QFileInfo(SelectedPathName).suffix();
        args << "-of" << "ILWIS" << namein << nameout;
        prog = GDALAppDirName + "gdal_translate.exe";
        if (!QFileInfo(prog).exists())
        {
            ErrorMsg("Cannot find GDAL tools, have you installed GDAL \nand set file->options?");
            actiontype = ACTIONTYPENONE;
        }
        commandWindow->appendPlainText("gdal_translate "+args.join(" "));
        break;
    case ACTIONTYPEMAP2TIFF:
        nameout = QFileInfo(SelectedPathName).baseName() + ".tif";
        namein =  QFileInfo(SelectedPathName).baseName() + "." + SelectedSuffix;//QFileInfo(SelectedPathName).suffix();
        args << namein << nameout;
        prog = GDALAppDirName + "gdal_translate.exe";
        if (!QFileInfo(prog).exists())
        {
            ErrorMsg("Cannot find GDAL tools, have you installed GDAL \nand set file->options?");
            actiontype = ACTIONTYPENONE;
        }
        commandWindow->appendPlainText("gdal_translate "+args.join(" "));
        break;
//    case ACTIONTYPEMAPDISPLAY :
//        //      args << cmdl;
//        mapDisplay.mapDisplayNames = SelectedPathName;
//        mapDisplay.initMapPlot();
//        mapDisplay.show();
//        mapDisplay.ShowMap();
//        break;
    case ACTIONTYPEWINDOWSCMD:
  QDesktopServices::openUrl(QUrl("file:///" + cmdl));
//        if (cmdl.contains("_nutshell_batchjob"))
//            break;

//        deleteBatch();
//        createBatch(cmdl,"");
//        prog = "cmd.exe";
//        args << QString("/C " + MapeditDirName + "_nutshell_batchjob");
//        CMDProcess->startDetached(prog,args);

        actiontype = ACTIONTYPENONE;
        break;
    default:
        STATUS("Opening file in operating system");
        QDesktopServices::openUrl(QUrl("file:///" + cmdl));
        //open process in its standard OS application
        actiontype = ACTIONTYPENONE;
    }

    if (actiontype != ACTIONTYPENONE)
    {
        if (isAguila)
            PCRProcess->startDetached(prog,args);
        else
        {
            PCRProcess->start(prog,args);
        }
    }

    actiontype = ACTIONTYPENONE;
    // reset actiontype for the next command

}
//---------------------------------------------------------------------------
