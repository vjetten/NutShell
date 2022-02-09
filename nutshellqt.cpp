/*
 * nutshellqt.cp
 * main functions, actions, key events, init of main app
 * registry and ini files
 *
 * Author: VJ 140222
 */

#include "nutshellqt.h"

//---------------------------------------------------------------
nutshellqt::nutshellqt(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    setupActions();

    setupToolBars();

    //setupMenu();

    setupModel();

    setupEditor();

    setupExplorer();

    showPCR();
    // set display filter to all PCR type files

    setupCommandwindow();
  //  QString name = qgetenv("USERPROFILE");
  //  QString name1 = qgetenv("ALLUSERSPROFILE");
 //commandWindow->appendPlainText(name);
 //commandWindow->appendPlainText(name1);
    currentPath = "C:/";

    comboBox_workdir->clear();
    comboBox_workdir->setInsertPolicy(QComboBox::InsertAtBottom);
    comboBox_workdir->addItem(currentPath);

    getNutshellIni();

    //if (CondaDirName.isEmpty() || !QFileInfo(CondaDirName).exists()) {
        CondaInstall = nutOptions.findCondaDir();
        QStringList sss = nutOptions.getOptions();
        CondaDirName = sss[2];
       // qDebug() << CondaDirName;
       // commandWindow->appendPlainText(CondaDirName);
  //  }
    // find conda installations

    setPCRasterDirectories();

    setWorkdirectory();

    STATUS("");

    commandcounter = -1;

    changeName = false;

    findDPIscale(true);

}
//---------------------------------------------------------------
nutshellqt::~nutshellqt()
{
    if (PCRProcess && PCRProcess->state() == QProcess::Running) {
        PCRProcess->kill();
    }
    if (calcProcess && calcProcess->state() == QProcess::Running) {
        calcProcess->kill();
    }
    if (CMDProcess && CMDProcess->state() == QProcess::Running) {
        CMDProcess->kill();
    }
    deleteBatch();
    setNutshellIni();
}
//---------------------------------------------------------------
void nutshellqt::setupActions()
{
    createMainActions();
    createModelActions();
    createEditorActions();
    createExplorerActions();
    createContextMenuActions();
    // not needed

    //    nextDirAct = new QAction(QIcon(":/resources/2X/forward.png"),QString("Next Directory"), this);
    //    prevDirAct = new QAction(QIcon(":/resources/2X/back.png"),QString("Prev Directory"), this);
    //   nextDirAct = new QAction(QString("Next Directory"), this);
    //    prevDirAct = new QAction(QString("Prev Directory"), this);

}
//---------------------------------------------------------------
void nutshellqt::createModelActions()
{
    // run model actions
    runmodelAct = new QAction(QIcon(":/resources/2X/start1.png"), "&Run active model...", this);
    runmodelAct->setCheckable (true);
    runmodelAct->setShortcut(Qt::CTRL+Qt::Key_R);
    runmodelAct->setToolTip("Run model script");
    connect(runmodelAct, SIGNAL(triggered()), this, SLOT(runModel()));

    pausemodelAct = new QAction(QIcon(":/resources/2X/pause2.png"), "&Pause model...", this);
    pausemodelAct->setCheckable (true);
    pausemodelAct->setToolTip("Pause model run");
    connect(pausemodelAct, SIGNAL(toggled(bool)), this, SLOT(suspendModel(bool)));

    killmodelAct = new QAction(QIcon(":/resources/2X/stop1.png"), "&Stop model...", this);
    killmodelAct->setCheckable (true);
    killmodelAct->setChecked (true);
    killmodelAct->setToolTip("Stop running");
    connect(killmodelAct, SIGNAL(triggered()), this, SLOT(killModel()));

    //    oldmodelAct = new QAction(QIcon(":/resources/oldcalc1.png"), "&Use oldcalc...", this);
    //    oldmodelAct->setCheckable (true);
    //    oldmodelAct->setToolTip("Run model script with oldcalc");
    //    connect(oldmodelAct, SIGNAL(toggled(bool)), this, SLOT(toggleOldcalc(bool)));

}
//---------------------------------------------------------------
void nutshellqt::createMainActions()
{
 //   toolButton_workdir->setVisible(false);
//    toolButton_returnWorkdir->setVisible(false);
    toolButton_dirprev->setVisible(false);
    toolButton_dirnext->setVisible(false);
   // widget_workDir->setVisible(false);

    // main actions
    connect(toolButton_workdir, SIGNAL(clicked()), this, SLOT(setWorkdirectory()));
    connect(toolButton_delWorkdir, SIGNAL(clicked()), this, SLOT(removeWorkdirectory()));
    connect(toolButton_clearWorkdirs, SIGNAL(clicked()), this, SLOT(clearWorkdirectories()));
   // connect(toolButton_returnWorkdir, SIGNAL(clicked()), this, SLOT(returnToWorkdirectory()));
    connect(comboBox_workdir, SIGNAL(currentIndexChanged(int)), this, SLOT(setWorkdirectoryNr(int)));
    //	connect(tabWidget, SIGNAL(currentChanged(int)),this, SLOT(changeSyntax(int)));

    optionsAct = new QAction(QIcon(":/resources/2X/options2.png"), "&Options...", this);
    connect(optionsAct, SIGNAL(triggered()), this, SLOT(getOptions()));

    newfileAct = new QAction(QIcon(":/resources/2X/filenew.png"), "&New empty file...", this);
    newfileAct->setShortcuts(QKeySequence::New);
    connect(newfileAct, SIGNAL(triggered()), this, SLOT(makeNewFile()));

    newfileScriptAct = new QAction(QIcon(":/resources/2X/filenewscript.png"), "&New script file...", this);
    newfileScriptAct->setShortcuts(QKeySequence::New);
    connect(newfileScriptAct, SIGNAL(triggered()), this, SLOT(makeNewScriptFile()));

    openfileAct = new QAction(QIcon(":/resources/2X/fileopen.png"), "&Open script file...", this);
    openfileAct->setShortcuts(QKeySequence::Open);
    connect(openfileAct, SIGNAL(triggered()), this, SLOT(openFile()));

    savefileAct = new QAction(QIcon(":/resources/2X/filesave.png"), "&Save script file...", this);
    savefileAct->setShortcuts(QKeySequence::Save);
    connect(savefileAct, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveasfileAct = new QAction(QIcon(":/resources/2X/filesaveas.png"), "S&ave script file as...", this);
    saveasfileAct->setShortcuts(QKeySequence::SaveAs);
    connect(saveasfileAct , SIGNAL(triggered()), this, SLOT(saveasFile()));

    closefileAct = new QAction(QIcon(":/resources/2X/fileclose.png"), "&Close script file...", this);
    //closefileAct->setShortcut(Qt::CTRL+Qt::Key_W);
    connect(closefileAct, SIGNAL(triggered()), this, SLOT(closeFile()));


    helpAct = new QAction(QIcon(":/resources/2X/help.png"), "&PCRaster help (web)", this);
    helpAct->setShortcut(Qt::Key_F1);
    connect(helpAct, SIGNAL(triggered()), this, SLOT(showHelp()));

    //    helpWebAct = new QAction(QIcon(":/resources/help.png"), "&Help: latest on the web", this);
    //    helpWebAct->setShortcut(QKeySequence::HelpContents);
    //    helpWebAct->setToolTip("Help, latest PCRaster webpages (F1)");
    //    connect(helpWebAct, SIGNAL(triggered()), this, SLOT(showWebHelp()));

    helpAguilaAct = new QAction(QIcon(":/resources/2X/help.png"), "Aguila manual (pdf, web)", this);
    connect(helpAguilaAct, SIGNAL(triggered()), this, SLOT(showAguilaHelp()));

    helppcrcalcAct = new QAction(QIcon(":/resources/2X/helpoperation.png"), "Context sensitive help (web)", this);
    helppcrcalcAct->setShortcut(Qt::CTRL+Qt::Key_F1);
    helppcrcalcAct->setToolTip("Context sensitive help (Ctrl-F1)");
    connect(helppcrcalcAct, SIGNAL(triggered()), this, SLOT(showHelpOperation()));

    helpNutshellAct = new QAction(QIcon(":/resources/2X/helpnutshell.png"), "Nutshell main commands", this);
    helpNutshellAct->setShortcut(Qt::ALT+Qt::Key_F1);
    helpNutshellAct->setToolTip("Nutshell main commands (Alt-F1)");
    connect(helpNutshellAct, SIGNAL(triggered()), this, SLOT(showNutshellHelp()));

    clearOptionsAct = new QAction(QIcon(""), "&Clear nutshell.ini...", this);
    connect(clearOptionsAct, SIGNAL(triggered()), this, SLOT(clearNutshellIni()));

}
//---------------------------------------------------------------
void nutshellqt::createEditorActions()
{
    // text edit options
    // text actions are defined here but connected with slot in myeditor construction
    //    cutAct = new QAction(QIcon(":/resources/2X/Cut.png"), tr("Cu&t text"), this);
    //    cutAct->setShortcuts(QKeySequence::Cut);
    //    cutAct->setStatusTip(tr("Cut selection"));

    //    copyAct = new QAction(QIcon(":/resources/2X/editCopy.png"), tr("&Copy text"), this);
    //    copyAct->setShortcuts(QKeySequence::Copy);
    //    copyAct->setStatusTip(tr("Copy selection"));

    //    pasteAct = new QAction(QIcon(":/resources/2X/editPaste.png"), tr("&Paste text"), this);
    //    pasteAct->setShortcuts(QKeySequence::Paste);
    //    pasteAct->setStatusTip(tr("Paste selection"));

    //    undoAct = new QAction(QIcon(":/resources/2X/editUndo.png"), tr("&Undo edit"), this);
    //    undoAct->setShortcuts(QKeySequence::Undo);
    //    undoAct->setStatusTip(tr("Undo"));

    //    redoAct = new QAction(QIcon(":/resources/2X/editRedo.png"), tr("&Redo edit"), this);
    //    redoAct->setShortcuts(QKeySequence::Redo);
    //    redoAct->setStatusTip(tr("Redo"));

    wheelAct = new QAction(this);

    //    deleteLineAct = new QAction(QIcon(":/resources/2X/table_delete_row.png"), "&Delete Line", this);
    deleteLineAct = new QAction("&Delete Line", this);
    deleteLineAct->setShortcut(Qt::CTRL+Qt::Key_D);
    connect(deleteLineAct, SIGNAL(triggered()), this, SLOT(deleteLine()));

    // the rest is done here3
    syntaxAct = new QAction(QIcon(":/resources/2X/syntax.png"), "&Show syntax", this);
    syntaxAct->setCheckable(true);
    syntaxAct->setChecked(true);
    connect(syntaxAct , SIGNAL(toggled(bool)), this, SLOT(showsyntax(bool)));

    actionFind = new QAction(QIcon(":/resources/2X/editFind.png"), "&Find", this);
    actionFind->setShortcuts(QKeySequence::Find);
    connect(actionFind, SIGNAL(triggered()), this, SLOT(findDialog()));

    actionReplace = new QAction(QIcon(":/resources/2X/editreplace.png"), "&Replace", this);
    actionReplace->setShortcuts(QKeySequence::Replace);
    connect(actionReplace, SIGNAL(triggered()), this, SLOT(findReplaceDialog()));

    actionFindNext = new QAction(QIcon(":/resources/2X/editfindnext.png"), "&Find Next", this);
    actionFindNext->setShortcut(Qt::Key_F3);
    connect(actionFindNext, SIGNAL(triggered()), this, SLOT(findNextfind()));

    actionFindPrev = new QAction(QIcon(":/resources/2X/editfindprev.png"), "&Find Prev", this);
    actionFindPrev->setShortcut(Qt::SHIFT+Qt::Key_F3);
    connect(actionFindPrev, SIGNAL(triggered()), this, SLOT(findPrevfind()));

 //   fontAct = new QAction(QIcon(":/resources/2X/fontselect.png"), "&Select font", this);
 //   connect(fontAct, SIGNAL(triggered()), this, SLOT(fontSelect()));

    fontIncreaseAct = new QAction(QIcon(":/resources/2X/fontbigger.png"), "&Increase font size", this);
    connect(fontIncreaseAct, SIGNAL(triggered()), this, SLOT(fontIncrease()));

    fontDecreaseAct = new QAction(QIcon(":/resources/2X/fontsmaller.png"), "&Decrease font size", this);
    connect(fontDecreaseAct, SIGNAL(triggered()), this, SLOT(fontDecrease()));

    decreaseIndentAct = new QAction(QIcon(":/resources/2X/editnoindent.png"), "&Decrease block indent 1 space (Ctrl-U)", this);
    decreaseIndentAct->setShortcut(Qt::CTRL+Qt::Key_U);
    connect(decreaseIndentAct, SIGNAL(triggered()), this, SLOT(decreaseIndent()));

    increaseIndentAct = new QAction(QIcon(":/resources/2X/editindent.png"), "&Increase block indent 1 space (Ctrl-I)", this);
    increaseIndentAct->setShortcut(Qt::CTRL+Qt::Key_I);
    connect(increaseIndentAct, SIGNAL(triggered()), this, SLOT(increaseIndent()));

    toggleHashAct = new QAction(QIcon(":/resources/2X/edithash.png"), "&Toggle block comment ## (Alt-3)", this);
    toggleHashAct->setShortcut(Qt::ALT+Qt::Key_3);
    connect(toggleHashAct, SIGNAL(triggered()), this, SLOT(increaseHash()));

    toggleReportAct = new QAction(QIcon(":/resources/2X/editreport.png"), "&Toggle report (Alt-4)", this);
    toggleReportAct->setShortcut(Qt::ALT+Qt::Key_4);
    connect(toggleReportAct, SIGNAL(triggered()), this, SLOT(increaseReport()));

    displayvarAct = new QAction(QIcon(":/resources/2X/aguilareport.png"), "&Show selected script variable (F4)", this);
    displayvarAct->setShortcut(Qt::Key_F4);
    connect(displayvarAct, SIGNAL(triggered()), this, SLOT(displayVar()));

    getdisplayvarAct = new QAction(QIcon(":/resources/2X/allreport.png"), "&Show all reported variables in script (F6)", this);
    getdisplayvarAct->setShortcut(Qt::Key_F6);
    getdisplayvarAct->setCheckable(true);
    connect(getdisplayvarAct, SIGNAL(triggered()), this, SLOT(getScriptLinks()));

}
//---------------------------------------------------------------
void nutshellqt::createExplorerActions()
{
    // explorer actions on toolbar

    aguilaplusAct   = new QAction(QIcon(":/resources/2X/add_16.png"), "Aguila +: show maps stacked or in seperate windows", this);
    aguilaplusAct->setCheckable (true);
    aguilaplusAct->setChecked(true);
    connect(aguilaplusAct,   SIGNAL(toggled(bool)), this, SLOT(actionplusaguila(bool)));

    aguila2DAct = new QAction(QIcon(":/resources/2X/aguila2d.png"), "Aguila 2D", this);
    connect(aguila2DAct,     SIGNAL(triggered()), this, SLOT(actionaguila2D()));

    aguila3DAct = new QAction(QIcon(":/resources/2X/aguila.png"), "Aguila 3D single map", this);
    connect(aguila3DAct,     SIGNAL(triggered()), this, SLOT(actionaquila3D()));

    aguila3DrapeAct = new QAction(QIcon(":/resources/2X/aguila3d.png"), "Aguila 3D stacked maps", this);
    connect(aguila3DrapeAct, SIGNAL(triggered()), this, SLOT(actionaguila3Drape()));

    aguilaplotAct   = new QAction(QIcon(":/resources/2X/timeplot.png"), "Aguila timeplot", this);
    connect(aguilaplotAct,   SIGNAL(triggered()), this, SLOT(actionaguilaplot()));

    legendAct   = new QAction(QIcon(":/resources/2X/maplegend1.png"), "CLassified map legend", this);
    connect(legendAct,       SIGNAL(triggered()), this, SLOT(actionlegend()));

    editorAct   = new QAction(QIcon(":/resources/2X/editmodel.png"), "Script editor", this);
    connect(editorAct,       SIGNAL(triggered()), this, SLOT(actioneditor()));

    mapattributeAct = new QAction(QIcon(":/resources/2X/header1.png"), "Look at map attributes", this);
    connect(mapattributeAct, SIGNAL(triggered()), this, SLOT(actionmapattribute()));

    mapnewAct   = new QAction(QIcon(":/resources/2X/mapnew.png"), "Make a new empty map", this);
    connect(mapnewAct,       SIGNAL(triggered()), this, SLOT(actionmapnew()));

    mapeditAct  = new QAction(QIcon(":/resources/2X/mapedit.png"), "Map edit", this);
    connect(mapeditAct,      SIGNAL(triggered()), this, SLOT(actionmapedit()));

    mapDisplayAct  = new QAction(QIcon(":/resources/2X/aguila2d.png"), "", this);
    connect(mapDisplayAct,      SIGNAL(triggered()), this, SLOT(actionmapDisplay()));

    map2TiffAct  = new QAction(QIcon(":/resources/2X/tifconvert.png"), "Covert Map to Tif (without projection)", this);
    connect(map2TiffAct,      SIGNAL(triggered()), this, SLOT(actionmapMap2Tiff()));

    //  map2IlwisAct  = new QAction(QIcon(":/resources/2X/map2ilwis.png"), "Covert Map to Ilwis mpr", this);
    //  connect(map2IlwisAct,      SIGNAL(triggered()), this, SLOT(actionmapMap2Ilwis()));

}
//---------------------------------------------------------------
void nutshellqt::createContextMenuActions()
{
    //    cutFileAct = new QAction(tr("Cu&t"), this);
    //    copyFileAct = new QAction(tr("Copy"), this);
    //    pasteFileAct = new QAction(tr("Paste"), this);
    setDirAct = new QAction(QIcon(":/resources/2X/dir_add.png"),QString("Set as Working Directory"), this);
    newDirAct = new QAction(QIcon(":/resources/2X/dirnew.png"),QString("Create Directory"), this);
    delDirAct = new QAction(QIcon(":/resources/2X/dir_delete.png"),QString("Delete Directory"), this);

    //    connect(cutFileAct, SIGNAL(triggered()), this, SLOT(cutFile()));
    //    connect(copyFileAct, SIGNAL(triggered()), this, SLOT(copyFile()));
    //    connect(pasteFileAct, SIGNAL(triggered()), this, SLOT(pasteFile()));
    //connect(setDirAct, SIGNAL(triggered()), this, SLOT(setWorkdirectory()));
    connect(newDirAct, SIGNAL(triggered()), this, SLOT(newDirectory()));
    connect(delDirAct, SIGNAL(triggered()), this, SLOT(deleteDirectory()));
}
//---------------------------------------------------------------
void nutshellqt::setupToolBars()
{
    // toolBar->addAction(newDirAct);
    toolBar->addAction(optionsAct);
    toolBar->addAction(newfileAct);
    toolBar->addAction(newfileScriptAct);
    toolBar->addAction(openfileAct);
    toolBar->addAction(savefileAct);
    toolBar->addAction(saveasfileAct);
    //toolBar->addAction(closefileAct);
    //    toolBar->addSeparator ();
    //    toolBar->addAction(cutAct);
    //    toolBar->addAction(copyAct);
    //    toolBar->addAction(pasteAct);
    //    toolBar->addAction(undoAct);
    //    toolBar->addAction(redoAct);
    toolBar->addSeparator ();
    toolBar->addAction(actionFind);
    toolBar->addAction(actionReplace);
    toolBar->addAction(actionFindNext);
    toolBar->addAction(actionFindPrev);
    toolBar->addSeparator ();

    //toolBar->addAction(syntaxAct);

  //  toolBar->addAction(fontAct);
    toolBar->addAction(fontIncreaseAct);
    toolBar->addAction(fontDecreaseAct);
    toolBar->addSeparator ();
    toolBar->addAction(decreaseIndentAct);
    toolBar->addAction(increaseIndentAct);
    toolBar->addAction(toggleHashAct);
    toolBar->addAction(toggleReportAct);
    toolBar->addAction(displayvarAct);
    toolBar->addAction(getdisplayvarAct);

    toolBar->addSeparator ();
    toolBar->addAction(helpAct);
    toolBar->addAction(helppcrcalcAct);
    toolBar->addAction(helpNutshellAct);

    toolBar->setAllowedAreas(Qt::RightToolBarArea | Qt::TopToolBarArea);
    //  addToolBar(Qt::RightToolBarArea, toolBar);

    pcrToolBar = new QToolBar();
    horizontalLayoutPCRbuttons->insertWidget(0, pcrToolBar);
    pcrToolBar->setIconSize(iSize);
    pcrToolBar->addAction(aguilaplusAct   );
    //pcrToolBar->addWidget(fileMaskBox);
    pcrToolBar->addAction(aguila2DAct     );
    pcrToolBar->addAction(aguila3DAct     );
    pcrToolBar->addAction(aguila3DrapeAct );
    pcrToolBar->addAction(aguilaplotAct   );
    pcrToolBar->addAction(legendAct       );
    pcrToolBar->addAction(editorAct       );
    pcrToolBar->addAction(mapattributeAct );
    pcrToolBar->addAction(mapnewAct       );
    pcrToolBar->addAction(mapeditAct      );
    pcrToolBar->addAction(map2TiffAct      );


    pcrToolBar->setSizePolicy (QSizePolicy::Expanding,QSizePolicy::Fixed);

    //        dirToolBar = new QToolBar();
    //        verticalLayout_tree->insertWidget(0, dirToolBar);
    //        dirToolBar->setIconSize(QSize(16,16));
    //        dirToolBar->addAction(prevDirAct   );
    //        dirToolBar->addAction(nextDirAct     );


}
//---------------------------------------------------------------
void nutshellqt::setupMenu( )
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newDirAct);
    fileMenu->addAction(newfileAct);
    fileMenu->addAction(newfileScriptAct);
    fileMenu->addAction(openfileAct);
    fileMenu->addAction(savefileAct);
    fileMenu->addAction(saveasfileAct);
    fileMenu->addAction(closefileAct);
    fileMenu->addSeparator();
    fileMenu->addAction(optionsAct);
    //fileMenu->addAction(clearOptionsAct);
    //fileMenu->addSeparator();
    //	fileMenu->addAction(exitAct);

    runMenu = menuBar()->addMenu(tr("&Run"));
    runMenu->addAction(runmodelAct);
    runMenu->addAction(pausemodelAct);
    runMenu->addAction(killmodelAct);
    //   runMenu->addAction(oldmodelAct);

    //editMenu = menuBar()->addMenu(tr("&Edit"));
    //    editMenu->addAction(undoAct);
    //    editMenu->addAction(redoAct);
    //    editMenu->addSeparator();
    //    editMenu->addAction(cutAct);
    //    editMenu->addAction(copyAct);
    //    editMenu->addAction(pasteAct);
    //    editMenu->addAction(wheelAct);

    //    editMenu->addSeparator ();

    findMenu = menuBar()->addMenu(tr("&Find"));
    findMenu->addAction(actionFind);
    findMenu->addAction(actionReplace);
    findMenu->addAction(actionFindNext);
    findMenu->addAction(actionFindPrev);

    formatMenu = menuBar()->addMenu(tr("F&ormat"));
    formatMenu->addAction(syntaxAct);
    formatMenu->addSeparator ();
  //  formatMenu->addAction(fontAct);
    formatMenu->addAction(fontIncreaseAct);
    formatMenu->addAction(fontDecreaseAct);
    formatMenu->addSeparator ();
    formatMenu->addAction(decreaseIndentAct);
    formatMenu->addAction(increaseIndentAct);
    formatMenu->addAction(toggleHashAct);
    formatMenu->addAction(toggleReportAct);
    formatMenu->addAction(deleteLineAct);


    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpAct);
    //  helpMenu->addAction(helpWebAct);
    helpMenu->addAction(helpAguilaAct);
    helpMenu->addAction(helppcrcalcAct);
    helpMenu->addAction(helpNutshellAct);
}
//---------------------------------------------------------------
void nutshellqt::getOptions()
{
    QStringList tempdirs;
    tempdirs.clear();

    tempdirs << PCRasterDirName  << GDALDirName << CondaDirName << QString::number(dpiscale) << (CondaInstall ? "1" : "0");
    //qDebug() << "into options" << tempdirs << CondaInstall;

    // push options to window
    nutOptions.setupOptions(tempdirs);
    //nutOptions.findCondaDir();
    nutOptions.setModal(true);

    if (nutOptions.exec())
    {
        // after closing options window get vars
        tempdirs = nutOptions.getOptions();

        CondaInstall = tempdirs[4] == "0" ? false : true;

        opDPIscale = tempdirs[3].toDouble();

        if(!CondaInstall) {

            PCRasterDirName = tempdirs[0];
            if (!PCRasterDirName.isEmpty()) {
                PCRasterDirName.replace("\\","/");
                if (!PCRasterDirName.endsWith("/"))
                    PCRasterDirName = PCRasterDirName + "/";
                PCRasterAppDirName = PCRasterDirName+ "bin/";

                GDALDirName = tempdirs[1];
                GDALDirName.replace("\\","/");
                if (!GDALDirName.endsWith("/"))
                    GDALDirName = GDALDirName + "/";
                GDALAppDirName = GDALDirName + "bin/gdal/apps/";

                AguilaDirName = PCRasterAppDirName;
            }
        } else {

            CondaDirName = tempdirs[2]+"/";
            PCRasterAppDirName = CondaDirName+"Library/bin/";
            AguilaDirName = PCRasterAppDirName;
            GDALAppDirName =PCRasterAppDirName;// CondaDirName;
        }

        findDPIscale(false);
    }
}
//---------------------------------------------------------------
void nutshellqt::setfontSize()
{
    qDebug() << "fs" << genfontsize << dpiscale;
    int fs = genfontsize;

    int fsdpi = int(fs*dpiscale);
    QFont font = qApp->font();
    font.setPixelSize((int)(fs*dpiscale));
    qApp->setFont(font);
    qApp->setStyleSheet(QString("QComboBox {font: %1px; padding: 1px 0px 1px 3px;}").arg(fsdpi));
    qApp->setStyleSheet(QString("QLineEdit {font: %1px;}").arg(fsdpi));
    qApp->setStyleSheet(QString("QLabel {font: %1px;}").arg(fsdpi));
    qApp->setStyleSheet(QString("QToolButton {font: %1px;}").arg(fsdpi));
    treeView->setStyleSheet(QString("font: %1px").arg(fsdpi));
    fileView->setStyleSheet(QString("font: %1px").arg(fsdpi));
    fileMenu->setStyleSheet(QString("font: %1px").arg(fsdpi));
    runMenu->setStyleSheet(QString("font: %1px").arg(fsdpi));
    editMenu->setStyleSheet(QString("font: %1px").arg(fsdpi));
    helpMenu->setStyleSheet(QString("font: %1px").arg(fsdpi));
    findMenu->setStyleSheet(QString("font: %1px").arg(fsdpi));
    formatMenu->setStyleSheet(QString("font: %1px").arg(fsdpi));
    menuBar()->setStyleSheet(QString("font: %1px").arg(fsdpi));

    qApp->setStyleSheet(QString("QCheckBox::indicator {width: %1px; height: %1px;}\
                                QRadioButton::indicator {width: %1px; height: %1px;}").arg(fsdpi));

}
//---------------------------------------------------------------
void nutshellqt::checkDPIscale()
{
    findDPIscale(true);
}

void nutshellqt::findDPIscale(bool check)
{
    //QRect rect = qApp->screens();
    QRect rect = QGuiApplication::primaryScreen()->availableGeometry();
    //->screenGeometry();
    int _H = rect.height();

//    qDebug() << _H;
//    QFont font = qApp->font();
//    genfontsize=font.pointSize();

    // do a bit of size teaking for large displays becvause QT 5.5.0 does not have that yet
    iSize = QSize(16,16);
    // the "20" margin is because not all mon itors are exactly the pixel size, e.g. 1210
    if (_H > 800) {
        genfontsize = 0;//12;
        QSize(24,24);
    }
    if (_H > 1080-5) {
        genfontsize = 1;// 14;
        iSize = QSize(24,24);
    }
    if (_H > 1200-5) {
        dpiscale = 1.2;
        genfontsize = 2;//14;
    }
    if (_H > 1440) {
        dpiscale = 2.0;
        genfontsize = 3;//12;
        iSize = QSize(48,48);
    }


    if (!check)
        dpiscale = opDPIscale;

    toolBar->setIconSize(iSize);
  //  setfontSize();

    const QWidgetList allWidgets = QApplication::allWidgets();
    for (QWidget *widget : allWidgets) {
        QFont font = widget->font();
        int ps = 8 + genfontsize;
        font.setPointSize(ps);
        widget->setFont(font);
        widget->update();
    }
    //qDebug() << "dpi" << _H << dpiscale;
}
