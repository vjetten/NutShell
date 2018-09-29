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

    setupMenu();

    setupModel();

    setupEditor();

    setupExplorer();

    showPCR();
    // set display filter to all PCR type files

    setupCommandwindow();

    //getRegPCRaster();
    // Obsolete

    currentPath = "C:/";

    getNutshellIni();

    setPCRasterDirectories();

    // present but not used
    toolButton_globaloptions->setVisible(false);
    //toolButton_oldcalc->setVisible(false);

    // OBSOLETE
    //   toolButton_deletemapseries->setVisible(false);
    //       toolButton_dirRemove->setVisible(false);
    //       toolButton_dirup->setVisible(false);
    //       toolButton_dirnext->setVisible(false);
    //       toolButton_dirprev->setVisible(false);
    //       toolButton_dirnew->setVisible(false);

//    dirModel->setRootPath(QDir(currentPath).rootPath());
//    setRootIndex(dirModel->index(currentPath));

    setWorkdirectory();

    STATUS("");

    commandcounter = -1;

    changeName = false;

    fileView->horizontalHeader()->moveSection(3, 2);


    QRect rect = qApp->desktop()->screenGeometry();
    int _H = rect.height();
    QFont font = qApp->font();
    genfontsize=font.pointSize();
    dpiscale = 1;

    // do a bit of size teaking for large displays becvause QT 5.5.0 does not have that yet
    toolBar->setIconSize(QSize(16, 16));
    QSize iSize = QSize(16,16);
// the "20" margin is because not all mon itors are exactly the pixel size, e.g. 1210
    int mar = 0;
    if (_H > 800) {
        toolBar->setIconSize(QSize(32, 32));
        genfontsize = 12;
    }
    if (_H > 1080+mar) {
        genfontsize = 14;
        this->setStyleSheet(QString("QToolButton * {icon-size: 32px 32px}"));
        iSize = QSize(24,24);
    }
    if (_H > 1200+mar) {
        dpiscale = 2;
        genfontsize = 12;
        toolBar->setIconSize(QSize(48, 48));
        this->setStyleSheet(QString("QToolButton * {icon-size: 48px 48px}"));
        iSize = QSize(32,32);
    }
toolBar->setIconSize(iSize);
    setfontSize(genfontsize);
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

    nextDirAct = new QAction(QIcon(":/resources/forward.png"),QString("Next Directory"), this);
    prevDirAct = new QAction(QIcon(":/resources/back.png"),QString("Prev Directory"), this);

}
//---------------------------------------------------------------
void nutshellqt::createModelActions()
{
    // run model actions
    runmodelAct = new QAction(QIcon(":/resources/start1.png"), "&Run active model...", this);
    runmodelAct->setCheckable (true);
    runmodelAct->setShortcut(Qt::CTRL+Qt::Key_R);
    runmodelAct->setToolTip("Run model script");
    connect(runmodelAct, SIGNAL(triggered()), this, SLOT(runModel()));

    pausemodelAct = new QAction(QIcon(":/resources/pause2.png"), "&Pause model...", this);
    pausemodelAct->setCheckable (true);
    pausemodelAct->setToolTip("Pause model run");
    connect(pausemodelAct, SIGNAL(toggled(bool)), this, SLOT(suspendModel(bool)));

    killmodelAct = new QAction(QIcon(":/resources/stop1.png"), "&Stop model...", this);
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
    // main actions
    connect(toolButton_workdir, SIGNAL(clicked()), this, SLOT(setWorkdirectory()));
    connect(toolButton_delWorkdir, SIGNAL(clicked()), this, SLOT(removeWorkdirectory()));
    connect(toolButton_returnWorkdir, SIGNAL(clicked()), this, SLOT(returnToWorkdirectory()));
    connect(comboBox_workdir, SIGNAL(currentIndexChanged(int)), this, SLOT(setWorkdirectoryNr(int)));
    //	connect(tabWidget, SIGNAL(currentChanged(int)),this, SLOT(changeSyntax(int)));

    newfileAct = new QAction(QIcon(":/resources/filenew.png"), "&New empty file...", this);
    newfileAct->setShortcuts(QKeySequence::New);
    connect(newfileAct, SIGNAL(triggered()), this, SLOT(makeNewFile()));

    newfileScriptAct = new QAction(QIcon(":/resources/filenewscript.png"), "&New script file...", this);
    newfileScriptAct->setShortcuts(QKeySequence::New);
    connect(newfileScriptAct, SIGNAL(triggered()), this, SLOT(makeNewScriptFile()));

    openfileAct = new QAction(QIcon(":/resources/fileopen.png"), "&Open script file...", this);
    openfileAct->setShortcuts(QKeySequence::Open);
    connect(openfileAct, SIGNAL(triggered()), this, SLOT(openFile()));

    savefileAct = new QAction(QIcon(":/resources/filesave.png"), "&Save script file...", this);
    savefileAct->setShortcuts(QKeySequence::Save);
    connect(savefileAct, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveasfileAct = new QAction(QIcon(":/resources/filesaveas.png"), "S&ave script file as...", this);
    saveasfileAct->setShortcuts(QKeySequence::SaveAs);
    connect(saveasfileAct , SIGNAL(triggered()), this, SLOT(saveasFile()));

    closefileAct = new QAction(QIcon(":/resources/fileclose.png"), "&Close script file...", this);
    //closefileAct->setShortcut(Qt::CTRL+Qt::Key_W);
    connect(closefileAct, SIGNAL(triggered()), this, SLOT(closeFile()));


    helpAct = new QAction(QIcon(":/resources/help.png"), "&PCRaster help", this);
    helpAct->setShortcut(Qt::Key_F1);
    connect(helpAct, SIGNAL(triggered()), this, SLOT(showHelp()));

    //    helpWebAct = new QAction(QIcon(":/resources/help.png"), "&Help: latest on the web", this);
    //    helpWebAct->setShortcut(QKeySequence::HelpContents);
    //    helpWebAct->setToolTip("Help, latest PCRaster webpages (F1)");
    //    connect(helpWebAct, SIGNAL(triggered()), this, SLOT(showWebHelp()));

    helpAguilaAct = new QAction(QIcon(":/resources/help.png"), "Aguila manual (pdf, web)", this);
    connect(helpAguilaAct, SIGNAL(triggered()), this, SLOT(showAguilaHelp()));

    helppcrcalcAct = new QAction(QIcon(":/resources/helpoperation.png"), "Context sensitive help (web)", this);
    helppcrcalcAct->setShortcut(Qt::CTRL+Qt::Key_F1);
    helppcrcalcAct->setToolTip("Context sensitive help (Ctrl-F1)");
    connect(helppcrcalcAct, SIGNAL(triggered()), this, SLOT(showHelpOperation()));

    helpNutshellAct = new QAction(QIcon(":/resources/helpnutshell.png"), "Nutshell main commands", this);
    helpNutshellAct->setShortcut(Qt::ALT+Qt::Key_F1);
    helpNutshellAct->setToolTip("Nutshell main commands (Alt-F1)");
    connect(helpNutshellAct, SIGNAL(triggered()), this, SLOT(showNutshellHelp()));

    clearOptionsAct = new QAction(QIcon(""), "&Clear nutshell.ini...", this);
    connect(clearOptionsAct, SIGNAL(triggered()), this, SLOT(clearNutshellIni()));

    optionsAct = new QAction(QIcon(""), "&Options...", this);
    connect(optionsAct, SIGNAL(triggered()), this, SLOT(getDirectories()));

}
//---------------------------------------------------------------
void nutshellqt::createEditorActions()
{
    // text edit options
    // text actions are defined here but connected with slot in myeditor construction
    cutAct = new QAction(QIcon(":/resources/Cut.png"), tr("Cu&t text"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut selection"));

    copyAct = new QAction(QIcon(":/resources/editCopy.png"), tr("&Copy text"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy selection"));

    pasteAct = new QAction(QIcon(":/resources/editPaste.png"), tr("&Paste text"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste selection"));

    undoAct = new QAction(QIcon(":/resources/editUndo.png"), tr("&Undo edit"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo"));

    redoAct = new QAction(QIcon(":/resources/editRedo.png"), tr("&Redo edit"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo"));

    wheelAct = new QAction(this);

    deleteLineAct = new QAction(QIcon(":/resources/table_delete_row.png"), "&Delete Line", this);
    deleteLineAct->setShortcut(Qt::CTRL+Qt::Key_D);
    connect(deleteLineAct, SIGNAL(triggered()), this, SLOT(deleteLine()));

    // the rest is done here3
    syntaxAct = new QAction(QIcon(":/resources/syntax.png"), "&Show syntax", this);
    syntaxAct->setCheckable(true);
    syntaxAct->setChecked(true);
    connect(syntaxAct , SIGNAL(toggled(bool)), this, SLOT(showsyntax(bool)));

    actionFind = new QAction(QIcon(":/resources/editFind.png"), "&Find", this);
    actionFind->setShortcuts(QKeySequence::Find);
    connect(actionFind, SIGNAL(triggered()), this, SLOT(findDialog()));

    actionReplace = new QAction(QIcon(":/resources/editreplace.png"), "&Replace", this);
    actionReplace->setShortcuts(QKeySequence::Replace);
    connect(actionReplace, SIGNAL(triggered()), this, SLOT(findReplaceDialog()));

    actionFindNext = new QAction(QIcon(":/resources/editfindnext.png"), "&Find Next", this);
    actionFindNext->setShortcut(Qt::Key_F3);
    connect(actionFindNext, SIGNAL(triggered()), this, SLOT(findNextfind()));

    actionFindPrev = new QAction(QIcon(":/resources/editfindprev.png"), "&Find Prev", this);
    actionFindPrev->setShortcut(Qt::SHIFT+Qt::Key_F3);
    connect(actionFindPrev, SIGNAL(triggered()), this, SLOT(findPrevfind()));

    fontAct = new QAction(QIcon(":/resources/fontselect.png"), "&Select font", this);
    connect(fontAct, SIGNAL(triggered()), this, SLOT(fontSelect()));

    fontIncreaseAct = new QAction(QIcon(":/resources/fontbigger.png"), "&Increase font size", this);
    connect(fontIncreaseAct, SIGNAL(triggered()), this, SLOT(fontIncrease()));

    fontDecreaseAct = new QAction(QIcon(":/resources/fontsmaller.png"), "&Decrease font size", this);
    connect(fontDecreaseAct, SIGNAL(triggered()), this, SLOT(fontDecrease()));

    decreaseIndentAct = new QAction(QIcon(":/resources/editnoindent.png"), "&Decrease block indent 1 space (Ctrl-U)", this);
    decreaseIndentAct->setShortcut(Qt::CTRL+Qt::Key_U);
    connect(decreaseIndentAct, SIGNAL(triggered()), this, SLOT(decreaseIndent()));

    increaseIndentAct = new QAction(QIcon(":/resources/editindent.png"), "&Increase block indent 1 space (Ctrl-I)", this);
    increaseIndentAct->setShortcut(Qt::CTRL+Qt::Key_I);
    connect(increaseIndentAct, SIGNAL(triggered()), this, SLOT(increaseIndent()));

    toggleHashAct = new QAction(QIcon(":/resources/edithash.png"), "&Toggle block comment ## (Alt-3)", this);
    toggleHashAct->setShortcut(Qt::ALT+Qt::Key_3);
    connect(toggleHashAct, SIGNAL(triggered()), this, SLOT(increaseHash()));

    toggleReportAct = new QAction(QIcon(":/resources/editreport.png"), "&Toggle report (Alt-4)", this);
    toggleReportAct->setShortcut(Qt::ALT+Qt::Key_4);
    connect(toggleReportAct, SIGNAL(triggered()), this, SLOT(increaseReport()));

    displayvarAct = new QAction(QIcon(":/resources/aguilareport.png"), "&Show selected script variable (F4)", this);
    displayvarAct->setShortcut(Qt::Key_F4);
    connect(displayvarAct, SIGNAL(triggered()), this, SLOT(displayVar()));

    getdisplayvarAct = new QAction(QIcon(":/resources/allreport.png"), "&Show all reported variables in script (F6)", this);
    getdisplayvarAct->setShortcut(Qt::Key_F6);
    getdisplayvarAct->setCheckable(true);
    connect(getdisplayvarAct, SIGNAL(triggered()), this, SLOT(getScriptLinks()));

}
//---------------------------------------------------------------
void nutshellqt::createExplorerActions()
{
    // explorer actions on toolbar

    aguilaplusAct   = new QAction(QIcon(":/resources/add_16.png"), "Aguila +: show maps stacked or in seperate windows", this);
    aguilaplusAct->setCheckable (true);
    aguilaplusAct->setChecked(true);
    connect(aguilaplusAct,   SIGNAL(toggled(bool)), this, SLOT(actionplusaguila(bool)));

    aguila2DAct = new QAction(QIcon(":/resources/aguila2d.png"), "Aguila 2D", this);
    connect(aguila2DAct,     SIGNAL(triggered()), this, SLOT(actionaguila2D()));

    aguila3DAct = new QAction(QIcon(":/resources/aguila.png"), "Aguila 3D single map", this);
    connect(aguila3DAct,     SIGNAL(triggered()), this, SLOT(actionaquila3D()));

    aguila3DrapeAct = new QAction(QIcon(":/resources/aguila3d.png"), "Aguila 3D stacked maps", this);
    connect(aguila3DrapeAct, SIGNAL(triggered()), this, SLOT(actionaguila3Drape()));

    aguilaplotAct   = new QAction(QIcon(":/resources/timeplot.png"), "Aguila timeplot", this);
    connect(aguilaplotAct,   SIGNAL(triggered()), this, SLOT(actionaguilaplot()));

    legendAct   = new QAction(QIcon(":/resources/maplegend1.png"), "CLassified map legend", this);
    connect(legendAct,       SIGNAL(triggered()), this, SLOT(actionlegend()));

    editorAct   = new QAction(QIcon(":/resources/editmodel.png"), "Script editor", this);
    connect(editorAct,       SIGNAL(triggered()), this, SLOT(actioneditor()));

    mapattributeAct = new QAction(QIcon(":/resources/header1.png"), "Look at map attributes", this);
    connect(mapattributeAct, SIGNAL(triggered()), this, SLOT(actionmapattribute()));

    mapnewAct   = new QAction(QIcon(":/resources/mapnew.png"), "Make a new empty map", this);
    connect(mapnewAct,       SIGNAL(triggered()), this, SLOT(actionmapnew()));

    mapeditAct  = new QAction(QIcon(":/resources/mapedit.png"), "Map edit", this);
    connect(mapeditAct,      SIGNAL(triggered()), this, SLOT(actionmapedit()));

    mapDisplayAct  = new QAction(QIcon(":/resources/aguila2d.png"), "", this);
    connect(mapDisplayAct,      SIGNAL(triggered()), this, SLOT(actionmapDisplay()));

    map2TiffAct  = new QAction(QIcon(":/resources/map2tiff.png"), "Covert Map to GeoTiff", this);
    connect(map2TiffAct,      SIGNAL(triggered()), this, SLOT(actionmapMap2Tiff()));

//    map2IlwisAct  = new QAction(QIcon(":/resources/map2ilwis.png"), "Covert Map to Ilwis mpr", this);
//    connect(map2IlwisAct,      SIGNAL(triggered()), this, SLOT(actionmapMap2Ilwis()));

}
//---------------------------------------------------------------
void nutshellqt::createContextMenuActions()
{
    //    cutFileAct = new QAction(tr("Cu&t"), this);
    //    copyFileAct = new QAction(tr("Copy"), this);
    //    pasteFileAct = new QAction(tr("Paste"), this);
    newDirAct = new QAction(QIcon(":/resources/dirnew.png"),QString("Create Directory"), this);
    delDirAct = new QAction(QIcon(":/resources/dir_delete.png"),QString("Delete Directory"), this);

    //    connect(cutFileAct, SIGNAL(triggered()), this, SLOT(cutFile()));
    //    connect(copyFileAct, SIGNAL(triggered()), this, SLOT(copyFile()));
    //    connect(pasteFileAct, SIGNAL(triggered()), this, SLOT(pasteFile()));
    connect(newDirAct, SIGNAL(triggered()), this, SLOT(newDirectory()));
    connect(delDirAct, SIGNAL(triggered()), this, SLOT(deleteDirectory()));
}
//---------------------------------------------------------------
void nutshellqt::setupToolBars()
{
    // toolBar->addAction(newDirAct);
    toolBar->addAction(newfileAct);
    toolBar->addAction(newfileScriptAct);
    toolBar->addAction(openfileAct);
    toolBar->addAction(savefileAct);
    toolBar->addAction(saveasfileAct);
    //toolBar->addAction(closefileAct);
    toolBar->addSeparator ();
    toolBar->addAction(cutAct);
    toolBar->addAction(copyAct);
    toolBar->addAction(pasteAct);
    toolBar->addAction(undoAct);
    toolBar->addAction(redoAct);
    toolBar->addSeparator ();
    toolBar->addAction(actionFind);
    toolBar->addAction(actionReplace);
    toolBar->addAction(actionFindNext);
    toolBar->addAction(actionFindPrev);
    toolBar->addSeparator ();

    //toolBar->addAction(syntaxAct);

    toolBar->addAction(fontAct);
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
    pcrToolBar->setIconSize(QSize(32,32));
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
    pcrToolBar->addAction(map2IlwisAct      );

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

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addAction(wheelAct);

    editMenu->addSeparator ();

    findMenu = menuBar()->addMenu(tr("&Find"));
    findMenu->addAction(actionFind);
    findMenu->addAction(actionReplace);
    findMenu->addAction(actionFindNext);
    findMenu->addAction(actionFindPrev);

    formatMenu = menuBar()->addMenu(tr("F&ormat"));
    formatMenu->addAction(syntaxAct);
    formatMenu->addSeparator ();
    formatMenu->addAction(fontAct);
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
void nutshellqt::getDirectories()
{
    QStringList tempdirs;
    tempdirs.clear();
    tempdirs << PCRasterAppDirName << AguilaDirName << MapeditDirName << GDALDirName;

    nutOptions.setupOptions(tempdirs);
    nutOptions.setModal(true);
    if (nutOptions.exec())
    {
        tempdirs = nutOptions.getOptions();

        PCRasterAppDirName = tempdirs[0];
        AguilaDirName = tempdirs[1];
        MapeditDirName = tempdirs[2];
        GDALDirName = tempdirs[3];

        PCRasterDirName = PCRasterAppDirName + "../";
        PCRasterDirName = QDir(PCRasterDirName).absolutePath() + "/";

        PCRasterDocDirName = PCRasterDirName + "doc/pcrman/";
        if (!QFileInfo(PCRasterDocDirName+"index.html").exists())
            PCRasterDocDirName = PCRasterDirName + "doc/manual/";
    }
}
//---------------------------------------------------------------
void nutshellqt::setfontSize(int fs)
{
    qDebug() << "fs" << fs << dpiscale;
    QFont font = qApp->font();
    font.setPixelSize(fs*dpiscale);
    qApp->setFont(font);

    qApp->setStyleSheet(QString("QComboBox {font: %1px; padding: 1px 0px 1px 3px;}").arg(fs*dpiscale));
  //  qApp->setStyleSheet(QString("QToolButton {iconSize: %1px %1px; }").arg(fs*dpiscale));

    qApp->setStyleSheet(QString("QCheckBox::indicator {width: %1px; height: %1px;}\
                                QRadioButton::indicator {width: %1px; height: %1px;}").arg(fs*dpiscale));

}
//---------------------------------------------------------------
