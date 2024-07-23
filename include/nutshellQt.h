#ifndef NUTSHELLQT_H
#define NUTSHELLQT_H

//---------------------------------------------------------------

#include <QtGui>
#include <QtWidgets>
//#include <QStyledItemDelegate>
#include <QFileInfo>

#include "csf.h"
//#include "CsfMap.h"
#include "ui_nutshellqt.h"
#include "nutshelllegend.h"
#include "nutshellmapattribute.h"
#include "nutshelleditor.h"
#include "nutshellhighlighter.h"
#include "findreplaceform.h"
#include "nutshellhelp.h"
#include "nutshelloptions.h"


//---------------------------------------------------------------

#define STATUS(s) statusBar()->showMessage(QString(s),1000);

#define ErrorMsg(s) QMessageBox::warning(this,QString("NutShell"),QString(s),QMessageBox::Ok)
#define WarningAllMsg(s) QMessageBox::warning(this,"NutShell",QString(s),QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll)
#define WarningMsg(s) QMessageBox::warning(this,"NutShell WARNING",QString(s),QMessageBox::Yes)// | QMessageBox::No )
#define QuestionMsg(s) QMessageBox::question(this,"NutShell",QString(s),QMessageBox::Yes | QMessageBox::No)

#define max(a, b)  (((a) > (b)) ? (a) : (b))
#define min(a, b)  (((a) < (b)) ? (a) : (b))

#define SelectedPathName fileModel->fileInfo(selectionModel->currentIndex()).absoluteFilePath()
#define SelectedSuffix   fileModel->fileInfo(selectionModel->currentIndex()).suffix()
//#define SelectedFileName fileModel->fileName(selectionModel->currentIndex())
#define SelectedFileName fileModel->fileInfo(selectionModel->currentIndex()).fileName()
//#define SelectPatnNameIndexes(j) fileModel->fileInfo(indexes.at(j)).absoluteFilePath()

#define ACTIONTYPEUNDEFINED -1
#define ACTIONTYPENONE       0
//#define ACTIONTYPEDISPLAY    1
#define ACTIONTYPEAGUILA3D   2
#define ACTIONTYPEDRAPE      3
#define ACTIONTYPETIMEPLOT   4
//#define ACTIONTYPEPCRASTER   5
#define ACTIONTYPEMAPEDIT    6
#define ACTIONTYPEMODEL      7
#define ACTIONTYPELEGEND     8
#define ACTIONTYPEAGUILA2D   9
#define ACTIONTYPEATTRIBUTE 10
#define ACTIONTYPEATTRIBUTENEW 11
//#define ACTIONTYPEGSTAT     12
//#define ACTIONTYPEMAPDISPLAY 13
#define ACTIONTYPEWINDOWSCMD 14
#define ACTIONTYPEMAP2TIFF 15
//#define ACTIONTYPEMAP2ILWIS 16

#define ETfilePath ET[tabWidget->currentIndex()].filePath
#define ETfileName ET[tabWidget->currentIndex()].fileName
#define ETEditor ET[tabWidget->currentIndex()].editor
#define ETHighlighter ET[tabWidget->currentIndex()].highlighter
#define ETPlainText ET[tabWidget->currentIndex()].editor->toPlainText()
#define ETExists (tabWidget->currentIndex() >= 0)

#define INSERTSPACE  0
#define INSERTHASH   1
#define INSERTREPORT 2


#define fPCR 0
#define fMaps 1
#define fPlot 2
#define fScript 3
#define fSeries 4
#define freport 5
#define fdelreport 6
#define fTIFF 7
#define fall 8

#define Delay(x) Sleeper::msleep(x);

//---------------------------------------------------------------
//! structure to keep relevant data of all mapseries in a directry
typedef struct filenameseries{
    QString name;
    QString base;
    QString begin;
    QString end;
    QStringList series;
} _fns;
//---------------------------------------------------------------
//! list of editor tabs and corrsponding filenames
typedef struct editortabs{
    nutshelleditor *editor;
//    PythonHighlighter *highlighter;
//    PCRHighlighter *pHighlighter;
    QSyntaxHighlighter *highlighter;
    QString filePath;
    QString fileName;
    int syntax;
} _editortab;
//---------------------------------------------------------------
//! variables reported or in the binding of a script
typedef struct reportrecord{
    QString reportName;
    QString fileName;
    bool isSeries;
    bool isBinding;
    //int lineNumber;
} _reportrecord;
//---------------------------------------------------------------
// used to create a millisecond delay in output
#include <QThread>

//class Sleeper : public QThread
//{
//public:
//    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
//    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
//    static void sleep(unsigned long secs){QThread::sleep(secs);}
//};

//---------------------------------------------------------------
//! used to paint first instance of map series in blue in file viewer
class BlueDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    BlueDelegate(QWidget *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    bool ismapseries;
    void setSeries(bool iss = false) {
        ismapseries = iss;
    }
};
//---------------------------------------------------------------
class FSM : public QFileSystemModel
{
    Q_OBJECT

public:
    FSM(QWidget *parent = nullptr) : QFileSystemModel(parent) {}

    QVariant data(const QModelIndex &index, int role) const
    {
      if (role == Qt::TextAlignmentRole && index.column() == 1)
      {
        // QFileSystemModel just sets the horizontal alignment which mispositions the text of column 1
        return QVariant(Qt::AlignRight | Qt::AlignVCenter);
      }
      return QFileSystemModel::data(index, role);
    }
};

//---------------------------------------------------------------
class QDetachableProcess : public QProcess
{
        Q_OBJECT
public:
    QDetachableProcess(QObject *parent = nullptr) : QProcess(parent){}
    void detach()
    {
        this->waitForStarted();
        setProcessState(QProcess::NotRunning);
    }
};
//---------------------------------------------------------------
class CustomFileSystemModel : public QFileSystemModel {
public:
    CustomFileSystemModel(QObject *parent = nullptr) : QFileSystemModel(parent) {}

    bool hasChildren(const QModelIndex &index) const override {
        if (!index.isValid()) {
            return true; // The root always has children
        }

        QFileInfo fileInfo(filePath(index));

        return fileInfo.isDir() && QFileSystemModel::hasChildren(index);
    }
};

class QDropEvent;

class myTreeView : public QTreeView
{
//    myTreeView(QWidget *parent = nullptr) : QTreeView(parent), hasFocus(false) {}

public:
    myTreeView(QTreeView *parent = nullptr);
    QList <filenameseries> fns;
    QString StripForName(QString S);

protected:
    void focusInEvent(QFocusEvent *event) override {
        hasFocus = true;
        QTreeView::focusInEvent(event);
    }

    void focusOutEvent(QFocusEvent *event) override {
        hasFocus = false;
        QTreeView::focusOutEvent(event);
    }

    void mouseDoubleClickEvent(QMouseEvent *event) override {
        if (hasFocus) {
            QModelIndex index = indexAt(event->pos());
            if (index.isValid() && model()->hasChildren(index)) {
                if (isExpanded(index)) {
                    collapse(index);
                } else {
                    expand(index);
                }
            } else {
                QTreeView::mouseDoubleClickEvent(event);
            }
        } else {
            QTreeView::mouseDoubleClickEvent(event);
        }
    }

    void dropEvent(QDropEvent *event);

private:
    bool hasFocus;
};
//---------------------------------------------------------------
class nutshellqt : public QMainWindow, private Ui::nutshellqt
{
    Q_OBJECT

public:
    nutshellqt(QWidget *parent = nullptr);
    ~nutshellqt();

    bool CondaInstall;
    bool PCRasterInstall;
    int genfontsize;
    double dpiscale, opDPIscale;
    void setfontSize();
    QSize iSize;

    myTreeView *treeView;
   // QTableView  *fileView; // is defined in ui
    void setupActions();
    void createMainActions();
    void createEditorActions();
    void createExplorerActions();
    void createContextMenuActions();
    void createModelActions();

    void setupToolBars();
    void setupMenu();
    void setupModel();
    void setupExplorer();
    void setupEditor();
    void setupCommandwindow();
    void setNutshellIni();
    void getNutshellIni();

    bool isTextFile(const QString& filename);
    bool isMapFile(const QString& filename);
    bool isTSSFile(const QString& filename);
    bool isTiffFile(const QString& filename);


    void createBatch(QString sss, QString args);
    void deleteBatch();

    void findDPIscale();

    QList<editortabs> ET;

    QMenu *fileMenu;
    QMenu *runMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QMenu *findMenu;
    QMenu *formatMenu;
    //QMenu *filecontextMenu;
    QToolBar *pcrToolBar;
    //void contextMenuEvent(QContextMenuEvent *event);
    QToolBar *dirToolBar;
    QToolBar *expToolBar;

    QString NutshellDirName;
    QString CondaDirName;
    QString rootPath;


    //======================
    //Vars for commandWindow
    //======================
    nutshelleditor *commandWindow;
    void parseCommand();
    void executeCommand(QStringList args);
    void prevCommand();
    void nextCommand();
    int commandcounter;

    QTextCursor calcCursor;
    int cursorPosition;

    //========================
    //Vars for pcrcalc process
    //========================
    int xlast;
    bool processDone;
    void doRunErrorMessage(QString buffer);
    void onScreen(QString buffer);
  //  bool useOldCalc;
    int errorpos[2];
    void runModelCommandwindow(QString prog, QStringList args);
    bool runPaused;
    void setButtons(bool one, bool two, bool three);
    int expFont;
    QString bufprev;
    //======================
    //Vars for editor
    //======================
    FindReplaceForm *m_findReplaceDialog;
    bool dosyntax;

    //======================
    //Vars for explorer
    //======================

    nutshellLegend maplegend;
    nutshellmapattribute mapattribute;
    //nutshellHelp help;
    nutshellOptions nutOptions;
    QProgressBar statusBarProgress;
    QLabel statusLabel;
    QList <filenameseries> fns;
    bool ismapseries;
    BlueDelegate *BDgate;   // display series in blue
    QStringList currentFilter;
    QStringList baseFilters;
    bool remember; //for walking through directory tree
    QString fileNamesToCopy;
    bool fileCutCopy;
    QString SAguilaBase; //obsolete
    QString plus;
    QStringList FNall; // list of all names containing integer extentions
    QList <reportrecord> reportNames;

    int insertType;
    int _filternr;

    QString getFileListString();
    QString getMapSeries();
    QString StripForNumber(QString S);
    QString StripForName(QString S);
    bool getScriptReport(bool addBinding);
    QStringList getReportFilter();
    int getTimesteps();
    QElapsedTimer time_ms;
    int totalsteps;
    int GetActionType();
    void PerformAction(int actiontype);
    bool isExtentionInt(QString name);
    void scriptFold(int section);
    bool changeName;
    bool crtlpressed;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QObject *obj, QWheelEvent *event);

public slots:
    //void doSomething(QProcess::ProcessState hoi);

//    void initExplorer(QString);
//    void initExplorer2(QString);
    //=========================
    //slots to run model script
    //=========================
    void runModel();
    void killModel();
    void suspendModel(bool pause);
    void finishedModel(int);
    //void toggleOldcalc(bool checked);
    void readFromStderr();
    void readFromStderrPCR();
    //void checkDPIscale();
//    void setWaitingCursor();
//    void restoreCursor();


    //=======================
    //slots for commandWindow
    //=======================
    void clearCommandWindow();
    void clearCommandList();
    void saveCommandList();
    void copyCommandList();
    void outputCommand();
    void errorCommand();
    void setCursorLast();

    //=====================
    //slots for main/editor
    //=====================
    void makeNewFile(bool script =  false, int typ = 1);
    void makeNewScriptFile();
    void closeFile();
    void closeTab(int index);
    void openFile();
    bool saveFile();
    bool saveasFile();
    bool saveFileName(const QString &fileName);
    void AddModel(QString name, int syntax);
    void setWorkdirectory();
    void returnToWorkdirectory();
    void removeWorkdirectory();
    void saveWorkdirectory();
    void clearWorkdirectories();
    void setWorkdirectoryNr(int);
    void findDialog();
    void findReplaceDialog();
    void findNextfind();
    void findPrevfind();
    void fontSelect();
    void fontDecrease();
    void fontIncrease();
    void decreaseIndent();
    void increaseIndent();
    void increaseHash();
    void increaseReport();
    void doIndent(bool back);
   // void showsyntax(bool doit);
   // void changeSyntax(int index);
    void displayVar();
    void getOptions();
    void clearNutshellIni();
    // void findScriptReport();
    void deleteLine();

//    void makeNewMapedit();
//    double fillDrawMapData(cTMap *_M, QwtMatrixRasterData *_RD);
//    void showMap();

    //====================
    //slots for explorer
    //====================
    void selectFiles(const QModelIndex& index);
    void contextualMenu(const QPoint& point);

    // left horizontal toolbar file commands
    void goBack();
    void goForward();
    void goUp();
    void goHome();
    void newDirectory();
    bool removeDirectory(const QString &dirName);
    bool deleteDirectory();
    void deleteSeries();
    void deleteScriptReport();
    void deleteFiles();

    // vertical toolbar
    void showAll();
    void showMaps();
    void showPCR();
    void showScript();
    void showPlot();
    void showSeries();
    void showReport();
    void showDelReport();
    void showTIF();

    // right horizontal toolbar PCR commands
    void actionplusaguila(bool check);
    void actionaguila2D();
    void actionaquila3D();
    void actionaguila3Drape();
    void actionaguilaplot();
    void actionlegend();
    void actionmapattribute();
    void actionmapnew();
    void actionmapedit();
    void actioneditor();
    void actionmapMap2Tiff();

    void cutFile();
    void copyFile();
    void pasteFile();

    void showHelp();
    void showWebHelp();
    void showAguilaHelp();
    void showHelpOperation();
    void showNutshellHelp();

    void changeFileFilter(int filterNr);

    void setNSRootIndex(const QModelIndex& index);
    void setWorkDirectoryIndex(const QModelIndex& index);

    void getScriptLinks();

private:

    //editor toolbar actions
    QAction *setDirAct;
    QAction *newDirAct;
    QAction *delDirAct;
    //QAction *nextDirAct;
    //QAction *prevDirAct;

    QAction *optionsAct;
    QAction *newfileAct;
    QAction *newfileScriptAct;
    QAction *closefileAct;
    QAction *openfileAct;
    QAction *savefileAct;
    QAction *saveasfileAct;

    QAction *runmodelAct;
    QAction *pausemodelAct;
    QAction *killmodelAct;
    QAction *oldmodelAct;

    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *wheelAct;

    QAction *clearOptionsAct;
    QAction *cutFileAct;
    QAction *copyFileAct;
    QAction *pasteFileAct;

    QAction *aguilaplusAct;
    QAction *aguila2DAct;
    QAction *aguila3DAct;
    QAction *aguila3DrapeAct;
    QAction *aguilaplotAct;
    QAction *legendAct;
    QAction *editorAct;
    QAction *mapattributeAct;
    QAction *mapnewAct;
    QAction *mapeditAct;
    QAction *mapDisplayAct;
    QAction *map2TiffAct;
    QAction *map2IlwisAct;

    QAction *actionFind;
    QAction *actionReplace;
    QAction *actionFindNext;
    QAction *actionFindPrev;

    QAction *fontAct;
    QAction *fontIncreaseAct;
    QAction *fontDecreaseAct;
    QAction *decreaseIndentAct;
    QAction *increaseIndentAct;
    QAction *deleteLineAct;
    QAction *toggleHashAct;
    QAction *toggleReportAct;
    QAction *displayvarAct;
   // QAction *syntaxAct;
    QAction *getdisplayvarAct;
    QAction *helpAct;
    QAction *helpWebAct;
    QAction *helppcrcalcAct;
    QAction *helpAguilaAct;
    QAction *helpNutshellAct;

    // explorer variables
    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;
   // FSM *fileModel;
    QItemSelectionModel *selectionModel;
    QItemSelectionModel *selectionDirModel;

    QString currentPath;
    QStack<QString> history;
    QStack<QString> future;

    QProcess *PCRProcess;
    QProcess *CMDProcess;
    //bool processError;
    QProcess *calcProcess;
    //QProcess *CMDProcess;

};

#endif // NUTSHELLQT_H
