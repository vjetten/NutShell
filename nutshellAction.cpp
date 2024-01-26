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
//void nutshellqt::actionmapDisplay()
//{
//    PerformAction(ACTIONTYPEMAPDISPLAY);
//}
//---------------------------------------------------------------------------
void nutshellqt::actionmapMap2Tiff()
{
    PerformAction(ACTIONTYPEMAP2TIFF);
}
//---------------------------------------------------------------------------
void nutshellqt::actionmapMap2Ilwis()
{
   // PerformAction(ACTIONTYPEMAP2ILWIS);
}
//---------------------------------------------------------------------------
void nutshellqt::createBatch(QString sss, QString args)
{
    QFile efout(MapeditDirName+"_nutshell_batchjob.cmd");
    if (efout.exists())
        efout.remove();
    efout.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream eout(&efout);
    eout << "@echo off\n";
    eout << "CD "+currentPath +"\n";
    eout << QString("PATH=") + PCRasterAppDirName + "\n";
    eout << "@echo on\n";
    eout << "call \"" + sss + "\" " + args;

    efout.flush();
    efout.close();
}
//---------------------------------------------------------------------------
void nutshellqt::deleteBatch()
{
  //  QFile efout(MapeditDirName+"_nutshell_batchjob.cmd");
 //   efout.remove();
}
//---------------------------------------------------------------------------
// get actiontype if double click or <enter> keypress in fileView
int nutshellqt::GetActionType()
{
    int at;

    QString ext = SelectedSuffix; //QFileInfo(SelectedPathName).suffix();
    MAP *m = Mopen(SelectedPathName.toLatin1(),M_READ_WRITE);

    if (m != nullptr || ext.toUpper() == "TIF")
    {
        at = ACTIONTYPEAGUILA2D;
        Mclose(m);
        m=nullptr;
    }
    else
        if( isTSSfile(SelectedPathName))
            at = ACTIONTYPETIMEPLOT;
        else
            if (ext.toUpper() == "BAT" || ext.toUpper() == "CMD")
                at = ACTIONTYPEWINDOWSCMD;
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

    // hack to force a map tio have the correct projection, that is depreciated anyway
    m = Mopen(SelectedPathName.toLatin1().data(),M_READ_WRITE);
    if (m == nullptr)
        fileIsMap = false;
    else {
        MputProjection(m,PT_YDECT2B);
        Mclose(m);
    }

    isTIFF = SelectedSuffix.toUpper() == "TIF";

    // check if selection is a pcraster map
    if (!fileIsMap && !isTIFF && (
        actiontype == ACTIONTYPEAGUILA3D ||
        actiontype == ACTIONTYPEAGUILA2D ||
        actiontype == ACTIONTYPEDRAPE ||
        actiontype == ACTIONTYPEMAPEDIT ||
        actiontype == ACTIONTYPEMAP2TIFF ||
        actiontype == ACTIONTYPELEGEND ))
    {
        ErrorMsg(QString("%1 is not a raster map.").arg(SelectedPathName));
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

    switch (actiontype)
    {
    case ACTIONTYPEAGUILA2D :
        args << "aguila" << "-2" << cmdl.split("!");
        // you cannot split on a space when the path name has a space in it!!!
        // but if you do not split aguila doesn't recognize two maps as one argument
        // so we use a character like ! to split and create the separate arguments

        //prog = AguilaDirName + PCRxtr + "aguila.exe";
        isAguila = true;
        break;
    case ACTIONTYPEAGUILA3D :
        args << "aguila" << "-3" << cmdl.split(plus);
        // a split on plus will always show as a single 3D surface,
        // two maps as two seperate surfaces
        //prog = AguilaDirName + PCRxtr + "aguila.exe";
        isAguila = true;
        break;
    case ACTIONTYPEDRAPE :
        if (cmdl.contains("!+!"))
            args << "aguila" << "-3" << cmdl.split("!");
        // cmdl already contains more maps with +, first map will be base
        else
        {
            // cmdl contains one or more maps without +
            args = cmdl.split("!");
            // splt first if possible

            if (args.count() == 1)
                args << cmdl;
            // if one map is chosen double it to construct e.g. dem.map + dem.map

            cmdl = args.join("!+!");
            // join allways with a + for drape effect in aguila
            args.clear();
            args << "aguila" << "-3" << cmdl.split("!");
        }
        isAguila = true;
        break;
    case ACTIONTYPETIMEPLOT :
        args << "aguila" << "-t" << cmdl.split("!");
        isAguila = true;
        break;
    case ACTIONTYPEMODEL :
            AddModel(SelectedPathName,1);
            actiontype = ACTIONTYPENONE;
        break;
    case ACTIONTYPEMAPEDIT :
        if (!QFileInfo(MapeditDirName + "mapedit.exe").exists()) {
            WarningMsg("Mapedit not found in Nutshell install directory.");
            actiontype = ACTIONTYPENONE;
        } else {
            cmdl.replace("!", "");
            if (cmdl.contains("+"))
                args << "mapedit" << cmdl.split("+");
            else
                args << "mapedit" << cmdl;
        }
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
                ErrorMsg("Only the legend of nominal, ordinal or boolean PCRaster maps can be read.");
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
        actiontype = ACTIONTYPENONE;
        break;
    case ACTIONTYPEATTRIBUTE :
        if (isTIFF) {
            args << "gdalinfo" << SelectedPathName;
            //prog = GDALAppDirName + "gdalinfo.exe";
        } else
        if (fileIsMap) {
            if (mapattribute.fill(SelectedPathName, false) == 0) {
                mapattribute.show();
                mapattribute.raise();
                actiontype = ACTIONTYPENONE;
            }
        } else {
            statusBar()->removeWidget(&statusLabel);
             ErrorMsg("File is not recognised as PCRaster or GeoTIFF map.");
        }
        break;
    case ACTIONTYPEMAP2TIFF:
        nameout = QFileInfo(SelectedPathName).baseName() + ".tif";
        namein =  QFileInfo(SelectedPathName).baseName() + "." + SelectedSuffix;//QFileInfo(SelectedPathName).suffix();
        args << "gdal_translate" << namein << nameout;
        commandWindow->appendPlainText(args.join(" "));
        break;
    case ACTIONTYPEWINDOWSCMD:
        args << cmdl;
        break;
    default:
        STATUS("Opening file in operating system");
        QDesktopServices::openUrl(QUrl("file:///" + cmdl));
        //open process in its standard OS application
        actiontype = ACTIONTYPENONE;
    }

    if (actiontype != ACTIONTYPENONE) {
        //qDebug() << args;
        executeCommand(args);
    }

    actiontype = ACTIONTYPENONE;
    // reset actiontype for the next command

}
//---------------------------------------------------------------------------
