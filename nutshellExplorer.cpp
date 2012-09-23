/*
 * NutShellExplorer,
 * v 1.0
 * adapted from the extended explorer example in Qt wiki:
 * http://www.qtcentre.org/wiki/index.php?title=Extended_Dir_View_example
 * Author: VJ 100814
 */


#include "nutshellqt.h"


//---------------------------------------------------------------
void nutshellqt::setupExplorer()
{
    fns.clear();

    PCRProcess = new QProcess(this);
    PCRProcess->setProcessChannelMode(QProcess::MergedChannels);
    connect(PCRProcess, SIGNAL(readyReadStandardOutput()),this, SLOT(outputCommand()) );
    // process for comamndwindow actions
    //??WHY IS THIS DECLARED HERE ?? NOT LOGICAL

    baseFilters.clear();
    baseFilters << QString("*.mod;*.map;*.csf;*.tbl;*.tss;*.txt;*.dat;*.csv;*.pcr");
    baseFilters << QString("*.map");
    baseFilters << QString("*.tss;*.tbl;*.txt;*.dat;*.csv;*.xls");
    baseFilters << QString("*.mod");
    baseFilters << QString("Map Series");
    baseFilters << QString("*.*");
    _filternr = 0;
    // fileter list for speed button selcetion

    dirModel = new QFileSystemModel(this);
    dirModel->setReadOnly(false); //true
    dirModel->setFilter ( QDir::AllDirs | QDir::NoDotAndDotDot);// | QDir::Drives );
    dirModel->setNameFilterDisables(false);
    dirModel->setReadOnly(false);

    fileModel = new QFileSystemModel(this);
    fileModel->setReadOnly(false);
    fileModel->setFilter( QDir::Files | QDir::NoDotAndDotDot);
    fileModel->setNameFilterDisables(false);

    treeView->setModel(dirModel);
    treeView->header()->setStretchLastSection(true);
    treeView->header()->setResizeMode(QHeaderView::ResizeToContents);
    treeView->setUniformRowHeights(true);

    treeView->setDragEnabled(true);
    treeView->setAcceptDrops(true);
    treeView->setDropIndicatorShown(true);

    treeView->setIndentation(10);
    treeView->setHeaderHidden(false);
    treeView->hideColumn(1);
    treeView->setSortingEnabled(true);
    treeView->sortByColumn(1, Qt::AscendingOrder);
    treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(treeView,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(contextualMenu(const QPoint &)));
  //  connect(selectionDirModel, SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(treeSelectionChanged(QModelIndex, QModelIndex)));

    fileView->setModel(fileModel);
    fileView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    fileView->setSelectionBehavior(QAbstractItemView::SelectRows);
    fileView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    // fileView->horizontalHeader()->setDefaultSectionSize(112);
    //fileView->horizontalHeader()->setStretchLastSection(true);
    fileView->resizeColumnToContents(0);
    fileView->verticalHeader()->hide();
    fileView->setShowGrid(false);
    fileView->setSortingEnabled(true);
    fileView->sortByColumn(0, Qt::AscendingOrder);
    fileView->setWordWrap(false);
    fileView->setColumnWidth (0, 156);
    fileView->setColumnWidth (1, 72);
    fileView->setColumnWidth (2, 72);
    fileView->setColumnWidth (3, 128);
    //fileView->setIconSize(QSize(0,0));
    fileView->verticalHeader()->setDefaultSectionSize(16);
    fileView->setDragEnabled(true);
    fileView->setAcceptDrops(true);
    fileView->setDropIndicatorShown(true);

    BDgate = new BlueDelegate();
    BDgate->setSeries(false);
    fileView->setItemDelegate(BDgate);
    // delegate paints filenames for mapseries blue

    selectionModel = new QItemSelectionModel(fileModel);
    fileView->setSelectionModel(selectionModel);
    selectionDirModel = new QItemSelectionModel(dirModel);
    treeView->setSelectionModel(selectionDirModel);

   // connect(treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(setRootIndex(QModelIndex)));
    connect(treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(setRootIndex(QModelIndex)));
    connect(fileView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectFiles(QModelIndex)));
    //connect(fileView, SIGNAL(clicked(QModelIndex)), this, SLOT(showAttributes(QModelIndex)));
    // double clicked activate pcraster stuff

    // OBSOLETE:
    //connect(toolButton_dirprev, SIGNAL(clicked()), this, SLOT(goBack()));
    //connect(toolButton_dirnext, SIGNAL(clicked()), this, SLOT(goForward()));
    //connect(toolButton_dirup, SIGNAL(clicked()), this, SLOT(goUp()));
    //connect(toolButton_deletemapseries, SIGNAL(clicked()), this, SLOT(deleteSeries()));
    //connect(toolButton_dirnew, SIGNAL(clicked()), this, SLOT(newDirectory()));
    //connect(toolButton_dirRemove, SIGNAL(clicked()), this, SLOT(removeDirectory()));
    //remember = true;

    connect(toolButton_showAll, SIGNAL(clicked()), this, SLOT(showAll()));
    connect(toolButton_showMaps, SIGNAL(clicked()), this, SLOT(showMaps()));
    connect(toolButton_showPCR, SIGNAL(clicked()), this, SLOT(showPCR()));
    connect(toolButton_showScript, SIGNAL(clicked()), this, SLOT(showScript()));
    connect(toolButton_showPlot, SIGNAL(clicked()), this, SLOT(showPlot()));
    connect(toolButton_showSeries, SIGNAL(clicked()), this, SLOT(showSeries()));
    connect(toolButton_showOutput, SIGNAL(clicked()), this, SLOT(showReport()));
    connect(toolButton_deletereport, SIGNAL(clicked()), this, SLOT(deleteScriptReport()));

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
}
//---------------------------------------------------------------

void nutshellqt::treeSelectionChanged(QModelIndex current, QModelIndex previous)
{
    QFileInfo fileInfo(fileModel->fileInfo(fileSystemProxyModel->mapToSource(current)));
    if(!fileInfo.exists())
        return;
    //getActivePane()->moveTo(fileInfo.filePath());
}

//---------------------------------------------------------------
void nutshellqt::dragEnterEvent(QDragEnterEvent *event)
{
    QModelIndex index = treeView->indexAt(event->pos());
//    QModelIndex index1 = treeView->indexAbove(event->pos());

    qDebug() << index;
    //event->acceptProposedAction();
}
//---------------------------------------------------------------
void nutshellqt::contextualMenu(const QPoint& point)
{
    QMenu *menu = new QMenu;
    menu->addAction(newDirAct);
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
//void nutshellqt::showAttributes(const QModelIndex& index)
//{
//    PerformAction(ACTIONTYPEATTRIBUTE);
//}
//---------------------------------------------------------------
void nutshellqt::setRootPath1(const QString& path)
{
    setRootIndex(dirModel->index(path));
    //	setRootIndex(fileModel->index(path));
}
//---------------------------------------------------------------
void nutshellqt::setRootIndex(const QModelIndex& index)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QModelIndex dir = index.sibling(index.row(), 0);
    if (dir != fileView->rootIndex())// && dirModel->isDir(dir))
    {
        //      if (remember)
        //      {
        //         future.clear();
        //         history.push(currentPath);
        //      }
        currentPath = dirModel->filePath(dir);
        treeView->setCurrentIndex(index);
        fileModel->setRootPath(currentPath);
        fileView->setRootIndex(fileModel->index(currentPath));

        changeFileFilter(_filternr);
        // to update getmapseries when changing dir with mouseclick
    }

    //toolButton_dirprev->setEnabled(!history.isEmpty());
    //toolButton_dirnext->setEnabled(!future.isEmpty());
    //toolButton_dirup->setEnabled(index.column() >= 0 && index.row() >= 0);

    QApplication::restoreOverrideCursor();
}
//---------------------------------------------------------------
//OBSOLETE
void nutshellqt::goBack()
{
    future.push(currentPath);
    if (!history.isEmpty())
    {
        currentPath = history.pop();
        remember = false;
        setRootPath1(currentPath);
    }
}
//---------------------------------------------------------------
//OBSOLETE
void nutshellqt::goForward()
{
    history.push(currentPath);
    if (!future.isEmpty())
    {
        currentPath = future.pop();
        remember = false;
        setRootPath1(currentPath);
    }
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
    setRootPath1(QDir::homePath());
}
//---------------------------------------------------------------
// the following show actions trigger fileFilterChange(QString S)
void nutshellqt::showPCR()
{
    ismapseries = true;
    changeFileFilter(0);
}
//---------------------------------------------------------------
void nutshellqt::showMaps()
{
    ismapseries = false;
    changeFileFilter(1);
}
//---------------------------------------------------------------
void nutshellqt::showPlot()
{
    ismapseries = false;
    changeFileFilter(2);
}
//---------------------------------------------------------------
void nutshellqt::showScript()
{
    ismapseries = false;
    changeFileFilter(3);
}
//---------------------------------------------------------------
void nutshellqt::showSeries()
{
    ismapseries = true;
    changeFileFilter(4);
}
//---------------------------------------------------------------
void nutshellqt::showReport()
{
    QString res, resa;
    QStringList list;

    _filternr = 6;

    ismapseries = false;

    res = getScriptReport();
    // get all filenames reported,
    // swap binding names with real names,
    // series only as base name

    if (res.isEmpty())
    {
        // ErrorMsg(QString("No reported variables to show."));
        STATUS("No reported variables to show.")
                return;
    }

    resa = GetMapSeries();
    // get mapseries

    list = res.split("\n");

    for(int i = 0; i < list.count(); i++)
    {
        if (!list[i].contains(".")) // mapseries
        {
            list.removeAt(i);
            ismapseries = true;
            BDgate->setSeries(true);
        }
    }
    // find and remove 1st name of mapseries (001)

    list << resa.split(";");
    // append allmapseries to the list

    fileModel->setNameFilters(list);
}
//---------------------------------------------------------------
void nutshellqt::showAll()
{
    ismapseries = false;
    changeFileFilter(5);
}
//---------------------------------------------------------------
void nutshellqt::changeFileFilter(int filterNr)
{
    if (filterNr == 6)
    {
        showReport();
        return;
    }

    QString S = baseFilters[filterNr];
    // get the right base filter from the hardcoded list
    _filternr = filterNr;
    // set the global file filter

    currentFilter.clear();

    // if mapseries use created mapseries filter
    // true for filter setPCR and setSeries and setReport
    if (ismapseries)
        S = GetMapSeries();
    // qDebug() << "filter" << S;

    BDgate->setSeries(ismapseries);
    // paint series blue idf they are included

    // set current filter to base filter or mapseries
    currentFilter << S.split(";");

    // add basefilter 0 (PCR files) to mapseries
    if (filterNr == 0)
        currentFilter << baseFilters[0].split(";");

    fileModel->setNameFilters(currentFilter);

    switch(filterNr){
    case 0: toolButton_showPCR->setChecked(true); break;
    case 1: toolButton_showMaps->setChecked(true); break;
    case 2: toolButton_showPlot->setChecked(true); break;
    case 3: toolButton_showScript->setChecked(true); break;
    case 4: toolButton_showSeries->setChecked(true); break;
    case 5: toolButton_showAll->setChecked(true); break;
    }
}
//---------------------------------------------------------------
// this function is not used anymore
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

    res = GetMapSeries();
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
    QString res;
    QStringList list, series;
    QDir dir = QDir(currentPath);
    QFile file;

    if (calcProcess && calcProcess->state() == QProcess::Running)
    {
        toolButton_startrun->setChecked(false);
        ErrorMsg("pcrcalc is active, wait until it is finished or press stop first");
        return;
    }

    res = getScriptReport();

    if (res.isEmpty())
    {
        ErrorMsg(QString("No reported variables to delete."));
        return;
    }
    list = res.split("\n");

    int j = 0;
    for(int i = 0; i < list.count(); i++)
    {
        if (!list[i].contains('.'))
            list[i] = "<i><font color=\"blue\">" + list[i] + "</font></i>";
        if (j == 0)
            list[i] = "<br>" + list[i];
        j++;
        if (j == 3)
        {
            list[i] = list[i] + "</br>";
            j = 0;
        }
    }

    QMessageBox::StandardButton reply =
            QuestionMsg(QString("<p>Delete all <b><font color=\"red\">reported</font></b> variables of script"
                                "<br><b>%1</b></br></p>"
                                "<p><i>WARNING: if you report INPUT variables these will be ALSO deleted!</i></p>"
                                "%2"
                                "<p>Continue?</p>").arg(ETfileName).arg(list.join("; ")));
    if (reply == QMessageBox::No)
        return;

    list = res.split("\n");
    // get map series and put in a separate QstringList
    for(int i = 0; i < list.count(); i++)
    {
        if (!list[i].contains(".")) // mapseries
        {
            for(int j = 0; j < fns.count(); j++)//nrseries; j++)
                if (list[i] == fns[j].base)
                    series << fns[j].series;
        }
    }

    for(int i = 0; i < list.count(); i++)
    {
        list[i].insert(0, currentPath + QDir::separator());
        list[i] = QDir::fromNativeSeparators(list[i]);
    }

    // delete mapseries basename from report list
    for(int i = 0; i < list.count(); i++)
        if (!list[i].contains(".")) // mapseries
            list.removeAt(i);

    // add full map series
    list << series;

    // check if there exists anything to delete
    bool nothing = true;
    foreach (QString str, list)
        if (dir.exists(str))
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

    changeFileFilter(_filternr);
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

    GetMapSeries();
    // fill fns list

    foreach (index, indexes)
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
//---------------------------------------------------------------
// delete one or multiple files and file series
/*
void nutshellqt::deleteFiles()
{
   QFile file;
   QModelIndexList indexes = selectionModel->selectedIndexes();
   QModelIndex index;

//   if (calcProcess && calcProcess->state() == QProcess::Running)
//   {
//      toolButton_startrun->setChecked(false);
//      ErrorMsg("pcrcalc is active, wait until it is finished or press stop first");
//      return;
//   }

   // each file has 4 indexes (name, date, type, size)
   bool start = true;
   bool doall = false;
   int k = 0;
   QMessageBox::StandardButton reply;

   GetMapSeries();
//   fns[i].name.isEmpty();
//   fns[i].base.isEmpty();
//   fns[i].begin.isEmpty();
//   fns[i].end.isEmpty();
//   fns[i].series.clear();

   statusLabel.setText("Deleting: ");
   statusBarProgress.setMaximum(indexes.count()/4);
   statusBar()->addWidget(&statusLabel);
   statusBar()->addWidget(&statusBarProgress);
   statusBarProgress.show();
   statusLabel.show();

   foreach (index, indexes)
      if (index.column() == 0)
      {
         // if not single files displayed treat a numbered file as a series
         if (isExtentionInt(fileModel->fileName(index)))
         {
            if (currentFilter[0] != "*.*")
            {
               QString base = StripForName(fileModel->filePath(index));
               if(!doall)
               {
                  QMessageBox::StandardButton reply =
                        WarningAllMsg(QString("Delete highlighted map series ?\n"
                                              "If you want to delete single files form a series select the \"*.*\" file filter"));
                  if (reply == QMessageBox::Yes)// || reply == QMessageBox::YesToAll)
                  {
                     for(int i = 0; i < nrseries; i++)
                        if (base == fns[i].base)
                        {
                           statusLabel.setText("Deleting: ");
                           statusBarProgress.setMaximum(fns[i].series.count());
                           statusBar()->addWidget(&statusLabel);
                           statusBar()->addWidget(&statusBarProgress);
                           statusBarProgress.show();
                           statusLabel.show();
                           for (int k = 0; k < fns[i].series.count(); k++)
                           {
                              file.setFileName(fns[i].series[k]);
                              file.remove();
                              statusBarProgress.setValue(k);
                           }
                        }
                     doall = true;//reply == QMessageBox::YesToAll;
                  }
                  else
                     break; // answer no to delete go on with next file
               }
               else // yes to all
               {
                  for(int i = 0; i < nrseries; i++)
                     if (base == fns[i].base)
                     {
                        statusLabel.setText("Deleting: ");
                        statusBarProgress.setMaximum(fns[i].series.count());
                        statusBar()->addWidget(&statusLabel);
                        statusBar()->addWidget(&statusBarProgress);
                        statusBarProgress.show();
                        statusLabel.show();
                        for (int k = 0; k < fns[i].series.count(); k++)
                        {
                           file.setFileName(fns[i].series[k]);
                           file.remove();
                           statusBarProgress.setValue(k);
                        }
                     }

               }
            }
            else
            {
               if (!doall)
               {
                  reply = WarningAllMsg(QString("Delete highlighted file(s)?"));
                  if (reply == QMessageBox::Yes)// || reply == QMessageBox::YesToAll)
                  {
                     file.setFileName(fileModel->filePath(index));
                     file.remove();
                     doall = true;//reply == QMessageBox::YesToAll;
                  }
               }
               else // yes to all
               {
                  file.setFileName(fileModel->filePath(index));
                  file.remove();
                  statusBarProgress.setValue(k);
                  k++;
               }
            }
         }
         else
         {
            //delete a single or multiple selected file, one warning
            if (start)
               reply = WarningMsg(QString("Delete highlighted file(s)?"));
            if (reply == QMessageBox::Yes)
               start = false;
            if (!start)
            {
               file.setFileName(fileModel->filePath(index));
               file.remove();
            }
         }
      }
   statusBar()->removeWidget(&statusBarProgress);
   statusBar()->removeWidget(&statusLabel);
}
*/
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
    QString res = GetMapSeries();
    fileNamesToCopy = MakeFileListString().replace(plus,";");
    fileCutCopy = true;
}
//---------------------------------------------------------------
void nutshellqt::copyFile()
{
    QString res = GetMapSeries();
    fileNamesToCopy = MakeFileListString().replace(plus,";");
    fileCutCopy = false;
}
//---------------------------------------------------------------
void nutshellqt::pasteFile()
{
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
    setRootIndex(index);
}
//---------------------------------------------------------------
//from http://john.nachtimwald.com/2010/06/08/qt-remove-directory-and-its-contents/

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
        setRootPath1(currentPath);
        return true;
    }
    else
    {
        ErrorMsg(QString("Could not delete %1").arg(dirName));
        return false;
    }
}
