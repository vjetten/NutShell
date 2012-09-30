/*
 * nutshellqt.cp
 * main functions, key events, init of main app
 * registry and ini files
 *
 */

#include "nutshellqt.h"

//---------------------------------------------------------------
nutshellqt::nutshellqt(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    getRegPCRaster();

    currentPath = "C:/";

    setupActions();

    setupToolBar();

    setupMenu();

    setupCommandwindow();

    setupExplorer();

    setupEditor();

    setupModel();

    getNutshellIni();

    // setup help html adresses and keywords in StringList
    help.setuphelp(PCRasterDocDirName);

    // initialize alternative for aguila
    //mapDisplay.setupMapPlot();

    showPCR();

    // present but not used
    toolButton_globaloptions->setVisible(false);
    //toolButton_oldcalc->setVisible(false);
    checkBox_argsubst->setVisible(false);

    // OBSOLETE
    //   toolButton_deletemapseries->setVisible(false);
//       toolButton_dirRemove->setVisible(false);
//       toolButton_dirup->setVisible(false);
//       toolButton_dirnext->setVisible(false);
//       toolButton_dirprev->setVisible(false);
//       toolButton_dirnew->setVisible(false);

    dirModel->setRootPath(QDir(currentPath).rootPath());
    setRootIndex(dirModel->index(currentPath));
//    fileModel->setRootPath(QDir(currentPath).rootPath());
//    setRootIndex(fileModel->index(currentPath));

    setWorkdirectory();

    STATUS("");

    commandcounter = -1;

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

    oldmodelAct = new QAction(QIcon(":/resources/oldcalc1.png"), "&Use oldcalc...", this);
    oldmodelAct->setCheckable (true);
    oldmodelAct->setToolTip("Run model script with oldcalc");
    connect(oldmodelAct, SIGNAL(toggled(bool)), this, SLOT(toggleOldcalc(bool)));

//    argsubsAct = new QAction(QIcon(":/resources/argsubs2.png"), "", this);
//    argsubsAct->setCheckable (true);
//    argsubsAct->setToolTip("use argument subsitution in script");
//    connect(argsubsAct, SIGNAL(toggled(bool)), lineEdit_argsubst, SLOT(enabled(bool)));
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

    newfileAct = new QAction(QIcon(":/resources/filenew.png"), "&New script file...", this);
    newfileAct->setShortcuts(QKeySequence::New);
    connect(newfileAct, SIGNAL(triggered()), this, SLOT(makeNewFile()));

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


    helpAct = new QAction(QIcon(":/resources/help.png"), "&Help files", this);
    connect(helpAct, SIGNAL(triggered()), this, SLOT(showHelp()));

    helpWebAct = new QAction(QIcon(":/resources/help.png"), "&Help: latest on the web", this);
    helpWebAct->setShortcut(QKeySequence::HelpContents);
    connect(helpWebAct, SIGNAL(triggered()), this, SLOT(showWebHelp()));

    helpAguilaAct = new QAction(QIcon(":/resources/help.png"), "Aguila manual (pdf, web)", this);
    connect(helpAguilaAct, SIGNAL(triggered()), this, SLOT(showAguilaHelp()));

    helppcrcalcAct = new QAction(QIcon(":/resources/helpoperation.png"), "Context sensitive help (web)", this);
    helppcrcalcAct->setShortcut(Qt::CTRL+Qt::Key_F1);
    helppcrcalcAct->setToolTip("Context sensitive help");
    connect(helppcrcalcAct, SIGNAL(triggered()), this, SLOT(showHelpOperation()));

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

    // the rest is done here3
    syntaxAct = new QAction(QIcon(":/resources/syntax.png"), "&Show syntax", this);
    syntaxAct->setCheckable (true);
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

    decreaseIndentAct = new QAction(QIcon(":/resources/editnoindent.png"), "&Decrease block indent 1 space", this);
    decreaseIndentAct->setShortcut(Qt::CTRL+Qt::Key_U);
    connect(decreaseIndentAct, SIGNAL(triggered()), this, SLOT(decreaseIndent()));

    increaseIndentAct = new QAction(QIcon(":/resources/editindent.png"), "&Increase block indent 1 space", this);
    increaseIndentAct->setShortcut(Qt::CTRL+Qt::Key_I);
    connect(increaseIndentAct, SIGNAL(triggered()), this, SLOT(increaseIndent()));

    toggleHashAct = new QAction(QIcon(":/resources/edithash.png"), "&Toggle block comment ##...", this);
    toggleHashAct->setShortcut(Qt::ALT+Qt::Key_3);
    connect(toggleHashAct, SIGNAL(triggered()), this, SLOT(increaseHash()));

    toggleReportAct = new QAction(QIcon(":/resources/editreport.png"), "&Toggle report...", this);
    toggleReportAct->setShortcut(Qt::ALT+Qt::Key_5);
    connect(toggleReportAct, SIGNAL(triggered()), this, SLOT(increaseReport()));

}
//---------------------------------------------------------------
void nutshellqt::createExplorerActions()
{
    // explorer actions on toolbar

    aguilaplusAct   = new QAction(QIcon(":/resources/plus-button.png"), "", this);
    aguilaplusAct->setCheckable (true);
    aguilaplusAct->setChecked(true);
    aguilaplusAct->setToolTip ("Show maps on top of each other or in parallel windows");
    connect(aguilaplusAct,   SIGNAL(toggled(bool)), this, SLOT(actionplusaguila(bool)));

    aguila2DAct = new QAction(QIcon(":/resources/aguila2d.png"), "", this);
    connect(aguila2DAct,     SIGNAL(triggered()), this, SLOT(actionaguila2D()));

    aguila3DAct = new QAction(QIcon(":/resources/aguila.png"), "", this);
    connect(aguila3DAct,     SIGNAL(triggered()), this, SLOT(actionaquila3D()));

    aguila3DrapeAct = new QAction(QIcon(":/resources/aguila3d.png"), "", this);
    connect(aguila3DrapeAct, SIGNAL(triggered()), this, SLOT(actionaguila3Drape()));

    aguilaplotAct   = new QAction(QIcon(":/resources/timeplot.png"), "", this);
    connect(aguilaplotAct,   SIGNAL(triggered()), this, SLOT(actionaguilaplot()));

    legendAct   = new QAction(QIcon(":/resources/maplegend1.png"), "", this);
    connect(legendAct,       SIGNAL(triggered()), this, SLOT(actionlegend()));

    editorAct   = new QAction(QIcon(":/resources/editmodel.png"), "", this);
    connect(editorAct,       SIGNAL(triggered()), this, SLOT(actioneditor()));

    mapattributeAct = new QAction(QIcon(":/resources/header1.png"), "", this);
    connect(mapattributeAct, SIGNAL(triggered()), this, SLOT(actionmapattribute()));

    mapnewAct   = new QAction(QIcon(":/resources/mapnew.png"), "", this);
    connect(mapnewAct,       SIGNAL(triggered()), this, SLOT(actionmapnew()));

    mapeditAct  = new QAction(QIcon(":/resources/mapedit.png"), "", this);
    connect(mapeditAct,      SIGNAL(triggered()), this, SLOT(actionmapedit()));

    mapDisplayAct  = new QAction(QIcon(":/resources/aguila2d.png"), "", this);
    connect(mapDisplayAct,      SIGNAL(triggered()), this, SLOT(actionmapDisplay()));

}
//---------------------------------------------------------------
void nutshellqt::createContextMenuActions()
{
    //    cutFileAct = new QAction(tr("Cu&t"), this);
    //    copyFileAct = new QAction(tr("Copy"), this);
    //    pasteFileAct = new QAction(tr("Paste"), this);
    newDirAct = new QAction(QIcon(":/resources/dirnew.png"),QString("Create Directory"), this);

    //    connect(cutFileAct, SIGNAL(triggered()), this, SLOT(cutFile()));
    //    connect(copyFileAct, SIGNAL(triggered()), this, SLOT(copyFile()));
    //    connect(pasteFileAct, SIGNAL(triggered()), this, SLOT(pasteFile()));
    connect(newDirAct, SIGNAL(triggered()), this, SLOT(newDirectory()));
}
//---------------------------------------------------------------
void nutshellqt::setupToolBar()
{
   // toolBar->addAction(newDirAct);
    toolBar->addAction(newfileAct);
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
    toolBar->addSeparator ();
//    toolBar->addAction(runmodelAct);
//    toolBar->addAction(pausemodelAct);
//    toolBar->addAction(killmodelAct);
//    toolBar->addAction(oldmodelAct);
//    toolBar->addAction(argsubsAct);
//    toolBar->addSeparator ();
    toolBar->addAction(helppcrcalcAct);
    toolBar->addAction(helpWebAct);

    toolBar->setAllowedAreas(Qt::RightToolBarArea | Qt::TopToolBarArea);
  //  addToolBar(Qt::RightToolBarArea, toolBar);

    pcrToolBar = new QToolBar();
    horizontalLayout->insertWidget(0, pcrToolBar);
    pcrToolBar->setIconSize(QSize(16,16));
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
   // pcrToolBar->addAction(mapDisplayAct   );

//    dirToolBar = new QToolBar();
//    verticalLayout_6->insertWidget(0, dirToolBar);
//    dirToolBar->setIconSize(QSize(16,16));
//    dirToolBar->addAction(prevDirAct   );
//    dirToolBar->addAction(nextDirAct     );
}
//---------------------------------------------------------------
void nutshellqt::setupMenu( )
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newDirAct);
    fileMenu->addAction(newfileAct);
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
    runMenu->addAction(oldmodelAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
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

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpAct);
    helpMenu->addAction(helpWebAct);
    helpMenu->addAction(helpAguilaAct);
    helpMenu->addAction(helppcrcalcAct);
}
//---------------------------------------------------------------
void nutshellqt::removeWorkdirectory()
{
    int place = comboBox_workdir->findText(currentPath);
    comboBox_workdir->removeItem(place);
    if (place > comboBox_workdir->count()-1)
        comboBox_workdir->setCurrentIndex(place-1);
    else
        comboBox_workdir->setCurrentIndex(place);
}
//---------------------------------------------------------------
void nutshellqt::setWorkdirectory()
{
    int place = comboBox_workdir->findText(currentPath);
    //check if dir exists already, if not insert it on top, else select it
    if(place == -1)
    {
        comboBox_workdir->insertItem(0,currentPath);
        comboBox_workdir->setCurrentIndex(0);
    }
    else
        comboBox_workdir->setCurrentIndex(place);
}
//---------------------------------------------------------------
void nutshellqt::returnToWorkdirectory()
{
   //comboBox_workdir->setCurrentIndex(0);
   setWorkdirectoryNr(0);
//    setRootIndex(dirModel->index(currentPath));
}
//---------------------------------------------------------------
void nutshellqt::setWorkdirectoryNr(int index)
{
    QDir dir;
    currentPath = comboBox_workdir->currentText();
    // set current working path
    setRootIndex(dirModel->index(currentPath));
    // set the explorer to this path
    dir.setCurrent(currentPath);
    // set the path in the operating system
}
//---------------------------------------------------------------
// must return true is event stops in this function or false when it should continue
bool nutshellqt::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {

        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_F5)
        {
            changeFileFilter(_filternr);
            QCoreApplication::sendPostedEvents(this, 0);
            //fileModel->setRootPath(QDir(currentPath).rootPath());
            setRootIndex(dirModel->index(currentPath));
            return true;
        }
    }

//        if (event->type() == QEvent::DragEnter)
//        {
//            QModelIndex index = treeView->indexAt(QCursor::pos());
//            qDebug() << "hoi" << index;
//        }
    if (obj == treeView)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

            if (keyEvent->key() == Qt::Key_Delete)
            {
                return(deleteDirectory());
            }
        }
    }
    if (obj == fileView)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Return)
            {
                PerformAction(GetActionType());
                return true;
            }
            if (keyEvent->key() == Qt::Key_Delete)
            {
                deleteFiles();
                return true;
            }
        }
    }
    if (obj == commandWindow)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

            if (calcProcess && calcProcess->state() == QProcess::Running)
                return true;

            if (keyEvent->key() == Qt::Key_Return)
            {
                parseCommand();
                commandcounter = -1;
                return true;
            }
            if (keyEvent->key() == Qt::Key_Up)
            {
                prevCommand();
                return true;
            }
            if (keyEvent->key() == Qt::Key_Down)
            {
                nextCommand();
                return true;
            }
            if (keyEvent->key() == Qt::Key_Backspace)
            {
                int bsize = commandWindow->document()->lastBlock().text().count();
                if (bsize == 0)
                    return true;
            }
            if (keyEvent->key() == Qt::Key_Escape)
            {
                //delete last line
                commandWindow->moveCursor( QTextCursor::End, QTextCursor::MoveAnchor );
                commandWindow->moveCursor( QTextCursor::StartOfLine, QTextCursor::MoveAnchor );
                commandWindow->moveCursor( QTextCursor::End, QTextCursor::KeepAnchor );
                commandWindow->textCursor().removeSelectedText();
                return true;
            }
        }
    }
    if (obj == comboBox_cmdlist->lineEdit())
    {
        if (event->type() == QEvent::MouseButtonDblClick)
        {
            copyCommandList();
            return true;
        }
    }

    return QMainWindow::eventFilter(obj, event);
}
//---------------------------------------------------------------
void nutshellqt::setNutshellIni()
{
    QSettings settings(QSettings::IniFormat,QSettings::UserScope,"NutSHell","NutSHellQt");
    settings.clear();
    //   settings.setValue("workDirectory", currentPath);
    settings.setValue("PCRasterDirectory", PCRasterDirName);
    settings.setValue("pcrcalcDirectory", PCRasterAppDirName);
    settings.setValue("aguilaDirectory", AguilaDirName);
    settings.setValue("mapeditDirectory", MapeditDirName);

    //settings.setValue(QString("workdir/current"),comboBox_workdir->currentIndex());
    for (int i = 0; i < comboBox_workdir->count(); i++)
    {
        if (i == comboBox_workdir->currentIndex())
            settings.setValue(QString("workdir/workdir%1").arg(i),comboBox_workdir->itemText(i)+"<");
        else
            settings.setValue(QString("workdir/workdir%1").arg(i),comboBox_workdir->itemText(i));
    }

    //settings.setValue(QString("modelnr/active"),tabWidget->currentIndex());
    settings.setValue("models/current",tabWidget->currentIndex());
    //    settings.setValue("models/arg_substitute_do",checkBox_argsubst->isChecked());
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
        settings.setValue(QString("cmdlist/Command%1").arg(i),comboBox_cmdlist->itemText(i));
    }

    settings.setValue("layout/splitter1", splitter->saveState());
    settings.setValue("layout/splitter2", splitter_2->saveState());
    settings.setValue("layout/splitter3", splitter_3->saveState());

}
//---------------------------------------------------------------
void nutshellqt::clearNutshellIni()
{
    QSettings settings(QSettings::IniFormat,QSettings::UserScope,"NutSHell","NutSHellQt");
    settings.clear();
}
//---------------------------------------------------------------
void nutshellqt::getNutshellIni()
{

    QSettings settings(QSettings::IniFormat,QSettings::UserScope,"NutSHell","NutSHellQt");

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

    settings.beginGroup("workdir");
    QStringList keys = settings.childKeys();
    int currentworkdir = 0;
    comboBox_workdir->setInsertPolicy(QComboBox::InsertAtBottom);
    QStringList dirs;
    for (int i = 0; i < keys.count(); i++)
    {
        QString str = settings.value(keys[i]).toString();
        if (!str.isEmpty())
        {
            if(str.contains("<"))
            {
                str.remove(str.size()-1,1);
                currentworkdir = i;
            }
            dirs << str;
        }
    }
    comboBox_workdir->clear();
    comboBox_workdir->addItems(dirs);

    comboBox_workdir->setCurrentIndex(currentworkdir);
    currentPath = comboBox_workdir->itemText(currentworkdir);
    settings.endGroup();

    settings.beginGroup("layout");
    splitter->restoreState(settings.value("splitter1").toByteArray());
    splitter_2->restoreState(settings.value("splitter2").toByteArray());
    splitter_3->restoreState(settings.value("splitter3").toByteArray());
    settings.endGroup();

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
                    //            checkBox_argsubst->setChecked(settings.value(keys[i]).toBool());
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
    QSettings rsettings(QSettings::SystemScope,"PCRaster","PCRaster");
    //SystemScope = HKEY_LOCAL_MACHINE

    PCRasterDirName = rsettings.value("Default", "").toString();
    QDir dir(PCRasterDirName);

    if (!dir.exists() || PCRasterDirName.isEmpty())
    {
        QSettings settings(QSettings::SystemScope,"PCRaster","Install");
        //try old registry
        PCRasterDirName = settings.value("Default", "").toString();
    }

    dir.setPath(PCRasterDirName);

    if (!dir.exists() || PCRasterDirName.isEmpty())
    {
        ErrorMsg(QString("PCRaster directory not found: %1\nSet dirs in File->Options").arg(PCRasterDirName));
        return;
    }
    QChar cs = QDir::separator();
    PCRasterDirName = PCRasterDirName + cs;
    PCRasterAppDirName = PCRasterDirName + "apps" + cs;
    PCRasterDocDirName = PCRasterDirName + "doc" + cs + "pcrman" + cs;
    AguilaDirName = PCRasterAppDirName;
    MapeditDirName = PCRasterAppDirName;
}
//---------------------------------------------------------------
void nutshellqt::getDirectories()
{
    QStringList tempdirs;
    tempdirs.clear();
    tempdirs << PCRasterAppDirName << AguilaDirName << MapeditDirName;

    nutOptions.setupOptions(tempdirs);
    nutOptions.setModal(true);
    if (nutOptions.exec())
    {
        tempdirs = nutOptions.getOptions();

        PCRasterAppDirName = tempdirs[0];
        AguilaDirName = tempdirs[1];
        MapeditDirName = tempdirs[2];
    }
}
