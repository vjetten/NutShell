/*
 * NutShellExplorer,

 * adapted from the extended explorer example in Qt wiki:
 * http://www.qtcentre.org/wiki/index.php?title=Extended_Dir_View_example
 * Author: VJ 140222,181001
 */


#include "nutshellqt.h"

/*! the explorer part is an extended version of
 *  http://www.qtcentre.org/wiki/index.php?title=Extended_Dir_View_example
 */
//---------------------------------------------------------------
/*!
 * \brief nutshellqt::setupExplorer Setup explorer with TWO QFileSystemModel models for file and dir
 */

//---------------------------------------------------------------
/// SLOT triggered when the tableview is populated: resize the columns but keep user control of column width
void nutshellqt::initExplorer2(QString)
{
 //   qDebug() << "initexplorer";
    // cannot be done earlier because treeview is not populated yet
    treeView->hideColumn(1);
    treeView->hideColumn(2);
    treeView->setVisible(false);
    treeView->resizeColumnToContents(0);
    treeView->setVisible(true);

    // !!!!
//        fileView->setVisible(false);
//        fileView->resizeColumnsToContents();
//        fileView->resizeRowsToContents();
//        fileView->setVisible(true);

    changeFileFilter(_filternr);
}

void nutshellqt::initExplorer(QString)
{
 //   qDebug() << "initexplorer";
    // cannot be done earlier because treeview is not populated yet
    treeView->hideColumn(1);
    treeView->hideColumn(2);
    treeView->setVisible(false);
    treeView->resizeColumnToContents(0);
    treeView->setVisible(true);

    // !!!!
//        fileView->setVisible(false);
//        fileView->resizeColumnsToContents();
//        fileView->resizeRowsToContents();
//        fileView->setVisible(true);

    changeFileFilter(_filternr);
}
//---------------------------------------------------------------
void nutshellqt::setupExplorer()
{
    fns.clear();
    baseFilters.clear();
    baseFilters << QString("*.mod;*.map;*.csf;*.tbl;*.tss;*.txt;*.dat;*.csv;*.pcr;*.cmd;*.bat;*.tif");
    baseFilters << QString("*.map");
    baseFilters << QString("*.tss;*.tbl;*.txt;*.dat;*.csv;*.xls");
    baseFilters << QString("*.mod;*.cmd;*.bat");
    baseFilters << QString("Map Series");
    baseFilters << QString("*.*");
    baseFilters << QString("*.*");
    baseFilters << QString("*.tif");
    baseFilters << QString("*.*");
    _filternr = 0;
    // predefined filters to show PCRaster relevant files

    dirModel = new QFileSystemModel(this);
    dirModel->setReadOnly(false);
    dirModel->setFilter ( QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Drives );
    dirModel->setNameFilterDisables(false);
    // directory tree view model, show only dirs

    fileModel = new FSM(this);//QFileSystemModel(this); //
    fileModel->setReadOnly(false);
    fileModel->setFilter( QDir::Files | QDir::NoDotAndDotDot );
    fileModel->setNameFilterDisables(false);
    // file list view model, show only files


    treeView = new myTreeView();
    verticalLayout_tree->insertWidget(0, treeView);

    treeView->header()->setStretchLastSection(true);
    treeView->setUniformRowHeights(true);
    treeView->setIndentation(10);
    treeView->setHeaderHidden(false);
    treeView->setSortingEnabled(true);
    treeView->sortByColumn(0, Qt::AscendingOrder);
    treeView->sortByColumn(1, Qt::AscendingOrder);
    //   treeView->setDragEnabled(true);
    treeView->setAcceptDrops(true);
    treeView->setDropIndicatorShown(true);

    treeView->setModel(dirModel);

    treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(treeView,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(contextualMenu(const QPoint &)));

    treeView->setVisible(false);
    treeView->resizeColumnToContents(0);
    treeView->setVisible(true);
    treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    fileView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    fileView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //fileView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    fileView->horizontalHeader()->setStretchLastSection(true);
    QCoreApplication::sendPostedEvents(this, 0);
    fileView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    fileView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    fileView->setShowGrid(false);
    fileView->setWordWrap(false);
    fileView->sortByColumn(0, Qt::AscendingOrder);
    fileView->setSortingEnabled(true);
    fileView->setDragEnabled(true);
    fileView->setAcceptDrops(true);
    fileView->setDropIndicatorShown(true);
    fileView->setIconSize(QSize(0,0));
    fileView->setModel(fileModel);

    BDgate = new BlueDelegate();
    BDgate->setSeries(false);
    fileView->setItemDelegate(BDgate);
    // delegate paints filenames for mapseries blue

    selectionModel = new QItemSelectionModel(fileModel);

    fileView->setSelectionModel(selectionModel);
    selectionDirModel = new QItemSelectionModel(dirModel);
    treeView->setSelectionModel(selectionDirModel);

    connect(fileModel, SIGNAL(directoryLoaded(QString)),this, SLOT(initExplorer(QString)));
    connect(fileModel, SIGNAL(rootPathChanged(QString)),this, SLOT(initExplorer2(QString)));
    // link finished updating tableview to resizing columns

    treeView->setEditTriggers(QAbstractItemView::NoEditTriggers | QAbstractItemView::EditKeyPressed);
    connect(treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(setRootIndex(QModelIndex)));
    connect(treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(setWorkDirectoryIndex(QModelIndex)));
    // single click on tree sets also fileView
    connect(fileView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectFiles(QModelIndex)));
    // double clicked activate pcraster stuff

    // OBSOLETE:
    //connect(toolButton_dirprev, SIGNAL(clicked()), this, SLOT(goBack()));
    //connect(toolButton_dirnext, SIGNAL(clicked()), this, SLOT(goForward()));
    //    connect(toolButton_dirup, SIGNAL(clicked()), this, SLOT(goUp()));
    //    connect(toolButton_dirnew, SIGNAL(clicked()), this, SLOT(newDirectory()));
    //    connect(toolButton_dirRemove, SIGNAL(clicked()), this, SLOT(removeDirectory()));

    // vertical tool buttons to show selections of files
    connect(toolButton_showAll, SIGNAL(clicked()), this, SLOT(showAll()));
    connect(toolButton_showTIF, SIGNAL(clicked()), this, SLOT(showTIF()));
    connect(toolButton_showMaps, SIGNAL(clicked()), this, SLOT(showMaps()));
    connect(toolButton_showPCR, SIGNAL(clicked()), this, SLOT(showPCR()));
    connect(toolButton_showScript, SIGNAL(clicked()), this, SLOT(showScript()));
    connect(toolButton_showPlot, SIGNAL(clicked()), this, SLOT(showPlot()));
    connect(toolButton_showSeries, SIGNAL(clicked()), this, SLOT(showSeries()));
    connect(toolButton_showOutput, SIGNAL(clicked()), this, SLOT(showReport()));
    connect(toolButton_deletereport, SIGNAL(clicked()), this, SLOT(showDelReport())); //deleteScriptReport()));


    expToolBar = new QToolBar();
    expToolBar->setOrientation(Qt::Vertical);
        expToolBar->setIconSize(iSize);
    verticalLayout->insertWidget(0, expToolBar);
    expToolBar->addWidget(toolButton_showPCR);
    expToolBar->addWidget(toolButton_showMaps);
    expToolBar->addWidget(toolButton_showPlot);
    expToolBar->addWidget(toolButton_showScript);
    expToolBar->addWidget(toolButton_showSeries);
    expToolBar->addWidget(toolButton_showOutput);
    expToolBar->addWidget(toolButton_deletereport);
    expToolBar->addWidget(toolButton_showTIF);
    expToolBar->addWidget(toolButton_showAll);



    statusBarProgress.setMaximum(100);
    statusBarProgress.setValue(0);
    statusBarProgress.setMinimumHeight(16);
    statusBarProgress.setMaximumHeight(16);
    statusBarProgress.setMaximumWidth(250);
    statusBarProgress.setFormat ( "%v");

    fileView->installEventFilter(this);
    // react to single and double click for pcraster
    comboBox_cmdlist->lineEdit()->installEventFilter(this);
    // handle double click on field of commandlist
    treeView->installEventFilter(this);

    plus = "!+!";
    // plus is used in the creation of correct filename string for aguila
    // you cannot split on a space when the path name has a space in it!!!
    // but if you do not split aguila doesn't recognize two maps as one argument
    // so we use a character like ! to split and create the separate arguments

    ismapseries = true;
    history.clear();
    future.clear();

    expFont = 32;
}
//---------------------------------------------------------------
void nutshellqt::contextualMenu(const QPoint  &point)
{
    QMenu *menu = new QMenu;
    menu->addAction(setDirAct);
    menu->addAction(newDirAct);
    menu->addAction(delDirAct);
    //    QModelIndex index = treeView->currentIndex();
    //    QString fileName = dirModel->data(dirModel->index(index.row(), 0),0).toString();
    //    menu->addAction(QString("Import"), this, SLOT(test_slot()));
    //    menu->addAction(QString("Export"), this, SLOT(test_slot()));
    menu->exec(QCursor::pos());
}
//---------------------------------------------------------------
// function for double click or enter in file liset
// Getactiontype is in nutshellaction.cpp
void nutshellqt::selectFiles(const QModelIndex& index)
{
    PerformAction(GetActionType());
}
//---------------------------------------------------------------
void nutshellqt::setWorkDirectoryIndex(const QModelIndex& index)
{
    setWorkdirectory();
}
//---------------------------------------------------------------
//! simplified set rootindex of filelist based on treeview click
//! treeview click is always a dir so no need to check
void nutshellqt::setRootIndex(const QModelIndex& index)
{
    QString oldpath = currentPath;

    currentPath = dirModel->fileInfo(index).absoluteFilePath();
    // get current path from the dirmodel

    treeView->setCurrentIndex(index);
    dirModel->setRootPath(currentPath);
    // set the view to currentpath en also the model
    // set the treeview to the current index when a dir is selected as workdir

    fileView->setRootIndex(fileModel->setRootPath(currentPath));
    // set the root index of the listview so that is shows files of the new dir

    changeFileFilter(_filternr);
    // to update getMapSeries when changing dir with mouseclick

    if(oldpath != currentPath)
    {
        future.clear();
        history.push(currentPath);
    }

    toolButton_dirprev->setEnabled(!history.isEmpty());
    toolButton_dirnext->setEnabled(!future.isEmpty());

    QCoreApplication::sendPostedEvents(this, 0);
}
//---------------------------------------------------------------
// the following show actions trigger fileFilterChange(QString S)
void nutshellqt::showPCR()
{
    ismapseries = true;
    changeFileFilter(fPCR);
}
//---------------------------------------------------------------
void nutshellqt::showMaps()
{
    ismapseries = false;
    changeFileFilter(fMaps);
}
//---------------------------------------------------------------
void nutshellqt::showPlot()
{
    ismapseries = false;
    changeFileFilter(fPlot);
}
//---------------------------------------------------------------
void nutshellqt::showScript()
{
    ismapseries = false;
    changeFileFilter(fScript);
}
//---------------------------------------------------------------
void nutshellqt::showSeries()
{
    ismapseries = true;
    changeFileFilter(fSeries);
}
//---------------------------------------------------------------
void nutshellqt::showAll()
{
    ismapseries = false;
    changeFileFilter(fall);
}
//---------------------------------------------------------------
void nutshellqt::showReport()
{
    changeFileFilter(freport);
}
//---------------------------------------------------------------
void nutshellqt::showDelReport()
{
    changeFileFilter(fdelreport);
}
//---------------------------------------------------------------
void nutshellqt::showTIF()
{
    ismapseries = false;
    changeFileFilter(fTIFF);
}
//---------------------------------------------------------------
// get all filenames reported, series as first occurence: name0000.001
QStringList nutshellqt::getReportFilter()
{
    QStringList filter;


    filter.clear();
    ismapseries = false;

    if (!getScriptReport(false))
    {
        STATUS("No reported variables to show.");
        return filter;
    }

    QStringList seriesList = getMapSeries().split(";");
    for (int i = 0; i < reportNames.count(); i++)
    {
        if (!reportNames[i].isSeries)
        {
            filter << reportNames[i].fileName;
        }
        else
        {
            for(int j = 0; j < seriesList.count(); j++)
            {
                if (StripForName(seriesList[j]).compare(reportNames[i].fileName, Qt::CaseInsensitive) == 0)
                {
                    filter << seriesList[j];
                    ismapseries = true;
                }
            }
        }
    }

    return filter;
}
//---------------------------------------------------------------
void nutshellqt::changeFileFilter(int filterNr)
{
    _filternr = filterNr;
    // set the global file filter nr

    switch(filterNr){
        case fPCR: toolButton_showPCR->setChecked(true); break;
        case fMaps: toolButton_showMaps->setChecked(true); break;
        case fPlot: toolButton_showPlot->setChecked(true); break;
        case fScript: toolButton_showScript->setChecked(true); break;
        case fSeries: toolButton_showSeries->setChecked(true); break;
        case freport: toolButton_showOutput->setChecked(true); break;
        case fdelreport: toolButton_deletereport->setChecked(true); break;
        case fTIFF: toolButton_showTIF->setChecked(true); break;
        case fall: toolButton_showAll->setChecked(true); break;
    }

    currentFilter.clear();
    // clear the display file filter
    if (filterNr == freport)
        currentFilter << getReportFilter();
    //currentFilter is set here to script reported output
    //    else
    //        if (filterNr == fdelreport)
    //            deleteScriptReport();
    else {
        if (filterNr == fdelreport) {
            deleteScriptReport();
            filterNr = 0;
            _filternr = 0;
            toolButton_showPCR->setChecked(true);
        }
        currentFilter << baseFilters[filterNr].split(";");
        // get the right base filter from the hardcoded list
        if (ismapseries)
            currentFilter << getMapSeries().split(";");
        // if mapseries use created mapseries filter
        // is already done in showReport
        // becomes true for filter setPCR and setSeries and setReport
    }

    BDgate->setSeries(ismapseries);
    // paint series blue if they are included in the filter

    if (currentFilter.count() == 0)
        currentFilter << " ";
    //force display no possible match if empty

    fileModel->setNameFilters(currentFilter);
    // set the file model to the filtered output

//   fileView->setVisible(false);
   // fileView->resizeColumnsToContents();
   // fileView->resizeRowsToContents();
//  //  fileView->setVisible(true);

//    fileView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    fileView->resizeColumnsToContents();
//    QCoreApplication::sendPostedEvents(this, 0);

//    fileView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);


}
//---------------------------------------------------------------
//! OBSOLETE this function is not used anymore
void nutshellqt::deleteSeries()
{
    QString res;
    QDir dir = QDir(currentPath);

    if (calcProcess && calcProcess->state() == QProcess::Running)
    {
        toolButton_startrun->setChecked(false);
        ErrorMsg("pcrcalc is active, wait until it is finished or press stop first");
        return;
    }

    res = getMapSeries();
    if (res.isEmpty())
    {
        ErrorMsg(QString("No map series to delete."));
        return;
    }

    QMessageBox::StandardButton reply = WarningMsg(QString("Delete all mapseries:\n%1 Continue?").arg(res.replace(";","\n")));
    if (reply == QMessageBox::No)
        return;

    foreach (QString str, FNall)
    {
        bool ok = dir.remove(str);
        if (!ok)
            ErrorMsg(QString("Could not delete %1!").arg(str));
    }
}
//---------------------------------------------------------------
void nutshellqt::deleteScriptReport()
{
    QStringList list, series;
    QFile file;

    toolButton_deletereport->setChecked(true);

    if (calcProcess && calcProcess->state() == QProcess::Running)
    {
        toolButton_startrun->setChecked(false);
        ErrorMsg("pcrcalc is active, wait until it is finished or press stop first");
        return;
    }

    if (!getScriptReport(false))
    {
        ErrorMsg(QString("No reported variables to delete."));
        return;
    }

    for(int i = 0; i < reportNames.count(); i++)
        list << reportNames[i].fileName;

    for(int i = 0; i < reportNames.count(); i++)
    {
        if (reportNames[i].isSeries) // mapseries
            list[i] = "<i><font color=\"blue\">" + list[i] + "</font></i>";
        // list[i] = "<br>" + list[i] + "</br>";
    }

    QMessageBox::StandardButton reply =
            QuestionMsg(QString("<p>Delete all <b><font color=\"red\">reported</font></b> variables of script"
                                "<br><b>%1</b></br></p>"
                                "<p><i>WARNING: if you report INPUT variables these will ALSO be deleted!</i></p>"
                                "%2"
                                "<p>Continue?</p>").arg(ETfileName).arg(list.join("; ")));
    if (reply == QMessageBox::No)
        return;

    getMapSeries();
    for(int i = 0; i < reportNames.count(); i++)
    {
        if (reportNames[i].isSeries)
        {
            for(int j = 0; j < fns.count(); j++)
                if (reportNames[i].fileName.compare(fns[j].base, Qt::CaseInsensitive) == 0)
                {
                    series << fns[j].series;
                    // add all maps of a map series
                }
        }
    }

    list.clear();
    // rebuild the list from scratch

    for(int i = 0; i < reportNames.count(); i++)
        if (!reportNames[i].isSeries)
            list << reportNames[i].fileName;

    for(int i = 0; i < list.count(); i++)
    {
        list[i].insert(0, currentPath + QDir::separator());
        list[i] = QDir::fromNativeSeparators(list[i]);
    }
    // add pathname to non series
    list << series;
    // add full map series

    // check if there exists anything to delete
    bool nothing = true;
    foreach (QString str, list)
        if (QDir(currentPath).exists(str))
        {
            nothing = false;
            break;
        }

    if (nothing)
    {
        ErrorMsg(QString("Reported variables not found."));
        return;
    }

    statusLabel.setText("Deleting reported files: ");
    statusBarProgress.setMaximum(list.count());
    statusBar()->addWidget(&statusLabel);
    statusBar()->addWidget(&statusBarProgress);
    statusBarProgress.show();
    statusLabel.show();
    int k = 0;
    foreach (QString str, list)
    {
        file.setFileName(str);
        file.remove();
        statusBarProgress.setValue(k++);
    }
    statusBar()->removeWidget(&statusBarProgress);
    statusBar()->removeWidget(&statusLabel);
    toolButton_deletereport->setChecked(false);

    //   changeFileFilter(0);//_filternr);
    // update explorer
}
//---------------------------------------------------------------
// delete one or multiple files and file series
void nutshellqt::deleteFiles()
{
    QFile file;
    QModelIndexList indexes = selectionModel->selectedIndexes();
    QModelIndex index;

    // each file has 4 indexes (name, date, type, size)
    QMessageBox::StandardButton reply =
            WarningMsg(QString("Delete highlighted maps and map series ?\n"
                               "To delete single files form a series select the \"*.*\" speedbutton"));
    if (reply == QMessageBox::No)
        return;

    getMapSeries();
    // fill fns list

    foreach (index, indexes)
    {
        if (index.column() == 0)
        {
            // if not single files displayed treat a numbered file as a series
            if (isExtentionInt(fileModel->fileName(index)) && currentFilter[0] != "*.*")
            {
                QString base = StripForName(fileModel->filePath(index));
                for(int i = 0; i < fns.count(); i++)
                    if (base == fns[i].base)
                    {
                        statusLabel.setText("Deleting: ");
                        statusBar()->addWidget(&statusLabel);
                        statusBar()->addWidget(&statusBarProgress);
                        statusBarProgress.show();
                        statusLabel.show();
                        statusBarProgress.setMaximum(fns[i].series.count());
                        for (int k = 0; k < fns[i].series.count(); k++)
                        {
                            file.setFileName(fns[i].series[k]);
                            file.remove();
                            statusBarProgress.setValue(k);
                        }
                        statusBar()->removeWidget(&statusLabel);
                        statusBar()->removeWidget(&statusBarProgress);
                    }
            }
            else // not a series
            {
                file.setFileName(fileModel->filePath(index));
                file.remove();
            }
        }
    }
}
//---------------------------------------------------------------
void BlueDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    QStyleOptionViewItem hoi = option;

    if (ismapseries)
    {
        QString name = index.model()->data(index, Qt::DisplayRole).toString();
        bool ok;
        int val = QFileInfo(name).suffix().toInt(&ok, 10);
        if (val > 0 && ok)
            hoi.palette.setColor(QPalette::Text,QColor(0, 0, 255, 255) );
    }
    QStyledItemDelegate::paint(painter, hoi, index);
}
//---------------------------------------------------------------
//void nutshellqt::contextMenuEvent(QContextMenuEvent *event)
//{
//    QMenu menu(this);
//    //   menu.addAction(cutFileAct);
//    //   menu.addAction(copyFileAct);
//    //   menu.addAction(pasteFileAct);
//    menu.addAction(newDirAct);

//    menu.exec(event->globalPos());
//}
//---------------------------------------------------------------
void nutshellqt::cutFile()
{
    QString res = getMapSeries();
    fileNamesToCopy = getFileListString().replace(plus,";");
    fileCutCopy = true;
}
//---------------------------------------------------------------
void nutshellqt::copyFile()
{
    QString res = getMapSeries();
    fileNamesToCopy = getFileListString().replace(plus,";");
    fileCutCopy = false;
}
//---------------------------------------------------------------
void nutshellqt::pasteFile()
{
    /*
   QStringList sourceFiles = fileNamesToCopy.split(";");
   QString str;
   foreach(str, sourceFiles)
   {
      QStringList series;
      series.clear();
      QString oldFilePath = str;
      if (str.contains("+")) // is map series
      {
         str.remove(str.indexOf("+"), 10);
         for(int j = 0; j < fns.count(); j++)//nrseries; j++)
            if (StripForName(str) == fns[j].base)
               series << fns[j].series;
      }
      else
         series << str;

      statusLabel.setText("Copying files: ");
      statusBarProgress.setMaximum(series.count());
      statusBar()->addWidget(&statusLabel);
      statusBar()->addWidget(&statusBarProgress);
      statusBarProgress.show();
      statusLabel.show();

      QString stra;
      int k = 0;
      foreach(stra, series)
      {
         QString destDir = dirModel->fileInfo(selectionDirModel->currentIndex()).absoluteFilePath();
         QString sourceDir = QFileInfo(stra).dir().absolutePath();
         QString newFilePath = destDir + QDir::separator() + QFileInfo(stra).fileName();
         newFilePath = QDir::fromNativeSeparators(newFilePath);
         //same file, add _copy
         if (sourceDir.compare(destDir) == 0)
            newFilePath = destDir + QDir::separator() + QFileInfo(stra).baseName() + "_copy." + QFileInfo(stra).suffix();

         // if new file exists add _copy
         if (QDir(newFilePath).exists())
            newFilePath = destDir + QDir::separator() + QFileInfo(stra).baseName() + "_copy." + QFileInfo(stra).suffix();

         QFile oldFile(stra);
         oldFile.copy(newFilePath);

         if (fileCutCopy)
            oldFile.remove();
         statusBarProgress.setValue(k++);
      }
      statusBar()->removeWidget(&statusBarProgress);
      statusBar()->removeWidget(&statusLabel);
   }
   changeFileFilter(_filternr);
   */
}
//---------------------------------------------------------------
void nutshellqt::newDirectory()
{
    QString path;// = currentPath + QDir::separator() + "New folder";
    QModelIndex index = dirModel->index(currentPath);
    treeView->expand(index);

    int i = 1;
    while (dirModel->fileInfo(index).exists())
    {
        if (i == 1)
            path = "New folder";
        else
            path = QString("New folder(%2)").arg(i);
        index = dirModel->index(currentPath + QDir::separator() + path);
        i++;
    }

    index = dirModel->index(currentPath);
    dirModel->mkdir(index,QDir(path).dirName());
    treeView->setModel(dirModel);
    setRootIndex(index);
}
//---------------------------------------------------------------
//! from http://john.nachtimwald.com/2010/06/08/qt-remove-directory-and-its-contents/
//! called from deletedirectory
bool nutshellqt::removeDirectory(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDirectory(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }

    return result;
}
//---------------------------------------------------------------
//! called from eventfilter when del key is pressed
bool nutshellqt::deleteDirectory()
{
    QModelIndex index = selectionDirModel->currentIndex();
    QString dirName = dirModel->fileInfo(index).absoluteFilePath();

    QMessageBox::StandardButton reply = WarningMsg(QString("Delete directory and all its contents: %1\n Continue?").arg(dirName));
    if (reply == QMessageBox::No)
        return false;

    if (removeDirectory(dirName))
    {
        if (index.row() == 0)
            setRootIndex(index.parent());
        else
            setRootIndex(treeView->indexAbove(index));
        currentPath = dirModel->fileInfo(treeView->indexAbove(index)).absoluteFilePath();
        setRootIndex(dirModel->index(currentPath));
        return true;
    }
    else
    {
        ErrorMsg(QString("Could not delete %1").arg(dirName));
        return false;
    }

    return false;
}
//---------------------------------------------------------------
void nutshellqt::goBack()
{
    future.push(currentPath);
    currentPath = history.pop();
    setRootIndex(dirModel->index(currentPath));
    int place = comboBox_workdir->findText(currentPath);
    if(place > -1)
        comboBox_workdir->setCurrentIndex(place);
}
//---------------------------------------------------------------
void nutshellqt::goForward()
{
    history.push(currentPath);
    currentPath = future.pop();
    setRootIndex(dirModel->index(currentPath));
    int place = comboBox_workdir->findText(currentPath);
    if(place > -1)
        comboBox_workdir->setCurrentIndex(place);
}

//---------------------------------------------------------------
void nutshellqt::removeWorkdirectory()
{
   comboBox_workdir->removeItem(comboBox_workdir->currentIndex());
   setWorkdirectory();
    //   initExplorer();
}
//---------------------------------------------------------------
void nutshellqt::clearWorkdirectories()
{
   comboBox_workdir->clear();
   setWorkdirectory();
    //   initExplorer();
}//---------------------------------------------------------------
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
//void nutshellqt::returnToWorkdirectory()
//{
//    setWorkdirectoryNr(0);
//}
//---------------------------------------------------------------
void nutshellqt::setWorkdirectoryNr(int index)
{
    QDir dir;

    treeView->collapse(dirModel->index(currentPath));

    currentPath = comboBox_workdir->currentText();
    if (currentPath.isEmpty())
        currentPath = QDir::currentPath();
    // set current working path

    //    QString root = QDir(currentPath).rootPath();
    //    setRootIndex(dirModel->index(root));
    //    treeView->sortByColumn(0, Qt::AscendingOrder);

    setRootIndex(dirModel->index(currentPath));
    // set the explorer to this path
    dir.setCurrent(currentPath);
    // set the path in the operating system

    treeView->expand(dirModel->index(currentPath) );
}
//---------------------------------------------------------------
//OBSOLETE
void nutshellqt::goUp()
{
    history.push(currentPath);
    remember = true;
    QModelIndex  index = dirModel->index(currentPath).parent();
    if (index.column() >= 0 && index.row() >= 0)
        setRootIndex(index);
}
//---------------------------------------------------------------
//OBSOLETE
void nutshellqt::goHome()
{
    remember = true;
    setRootIndex(dirModel->index(QDir::homePath()));
}
//---------------------------------------------------------------
