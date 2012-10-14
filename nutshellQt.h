#ifndef NUTSHELLQT_H
#define NUTSHELLQT_H

//---------------------------------------------------------------

#include <QtGui>
//#include <QMainWindow>
//#include <QtGlobal>
#include <QStyledItemDelegate>

#include "csf.h"
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

#define ErrorMsg(s) QMessageBox::critical(this,"NutShell",QString(s));
#define WarningAllMsg(s) QMessageBox::warning(this,"NutShell",QString(s),QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll);
#define WarningMsg(s) QMessageBox::warning(this,"NutShell",QString(s),QMessageBox::Yes | QMessageBox::No );
#define QuestionMsg(s) QMessageBox::question(this,"NutShell",QString(s),QMessageBox::Yes | QMessageBox::No);

#define max(a, b)  (((a) > (b)) ? (a) : (b))
#define min(a, b)  (((a) < (b)) ? (a) : (b))

#define SelectedPathName fileModel->fileInfo(selectionModel->currentIndex()).absoluteFilePath()
#define SelectedSuffix   fileModel->fileInfo(selectionModel->currentIndex()).suffix()
#define SelectedFileName fileModel->fileName(selectionModel->currentIndex())
//#define SelectPatnNameIndexes(j) fileModel->fileInfo(indexes.at(j)).absoluteFilePath()

#define ACTIONTYPEUNDEFINED -1
#define ACTIONTYPENONE       0
#define ACTIONTYPEDISPLAY    1
#define ACTIONTYPEAGUILA3D   2
#define ACTIONTYPEDRAPE      3
#define ACTIONTYPETIMEPLOT   4
#define ACTIONTYPEPCRASTER   5
#define ACTIONTYPEMAPEDIT    6
#define ACTIONTYPEMODEL      7
#define ACTIONTYPELEGEND     8
#define ACTIONTYPEAGUILA2D   9
#define ACTIONTYPEATTRIBUTE 10
#define ACTIONTYPEATTRIBUTENEW 11
#define ACTIONTYPEGSTAT     12
#define ACTIONTYPEMAPDISPLAY 13
#define ACTIONTYPEWINDOWSCMD 14

#define ETfilePath ET[tabWidget->currentIndex()].filePath
#define ETfileName ET[tabWidget->currentIndex()].fileName
#define ETEditor ET[tabWidget->currentIndex()].editor
#define ETHighlighter ET[tabWidget->currentIndex()].highlighter
#define ETPlainText ET[tabWidget->currentIndex()].editor->toPlainText()
#define ETExists (tabWidget->currentIndex() >= 0)


#define INSERTSPACE  0
#define INSERTHASH   1
#define INSERTREPORT 2

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
    Highlighter *highlighter;
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

class Sleeper : public QThread
{
public:
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};

//---------------------------------------------------------------
//! used to paint first instance of map series in blue in file viewer
class BlueDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    BlueDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    bool ismapseries;
    void setSeries(bool iss = false) {
        ismapseries = iss;
    }
};
//---------------------------------------------------------------
class QDropEvent;

class myTreeView : public QTreeView
{
public:
    myTreeView(QTreeView *parent = 0);
    QList <filenameseries> fns;
    QString StripForName(QString S);


protected:
    void dropEvent(QDropEvent *event);
};
//---------------------------------------------------------------
class nutshellqt : public QMainWindow, private Ui::nutshellqt
{
    Q_OBJECT

public:
    nutshellqt(QWidget *parent = 0);
    ~nutshellqt();

    myTreeView *treeView;

    void setupActions();
    void createMainActions();
    void createEditorActions();
    void createExplorerActions();
    void createContextMenuActions();
    void createModelActions();

    void setupToolBar();
    void setupMenu();
    void setupModel();
    void setupExplorer();
    void setupEditor();
    void setupCommandwindow();
    void setNutshellIni();
    void getNutshellIni();
    void getRegPCRaster();    

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

    QString AguilaDirName;
    QString PCRasterDirName;
    QString PCRasterAppDirName;
    QString PCRasterDocDirName;
    QString MapeditDirName;


    //======================
    //Vars for commandWindow
    //======================
    nutshelleditor *commandWindow;
    void parseCommand();
    void prevCommand();
    void nextCommand();
    int commandcounter;

    QTextCursor calcCursor;
    int cursorPosition;


    //========================
    //Vars for pcrcalc process
    //========================
    int xlast;
    void doRunErrorMessage(QString buffer);
    void onScreen(QString buffer);
    bool useOldCalc;
    int errorpos[2];
    void runModelCommandwindow(QString prog, QStringList args);
    bool runPaused;
    void setButtons(bool one, bool two, bool three);

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
    QString GetMapSeries();
    QString StripForNumber(QString S);
    QString StripForName(QString S);
    bool getScriptReport();
    QStringList getReportFilter();
    int getTimesteps();
    QTime time_ms;
    int totalsteps;
    int GetActionType();
    void PerformAction(int actiontype);
    bool isTSSfile(QString name);
    bool isExtentionInt(QString name);
    void scriptFold(int section);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

public slots:

    //=========================
    //slots to run model script
    //=========================
    void runModel();
    void killModel();
    void suspendModel(bool pause);
    void finishedModel(int c);
    void toggleOldcalc(bool checked);
    void readFromStderr();

    //=======================
    //slots for commandWindow
    //=======================
    void clearCommandWindow();
    void clearCommandList();
    void copyCommandList();
    void outputCommand();
    void setCursorLast();

    //=====================
    //slots for main/editor
    //=====================
    void makeNewFile();
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
    void setWorkdirectoryNr(int index);
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
    void showsyntax(bool doit);
    void changeSyntax(int index);
    void displayVar();
    void getDirectories();
    void clearNutshellIni();
    void findScriptReport();


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
    void actionmapDisplay();

    void cutFile();
    void copyFile();
    void pasteFile();

    void showHelp();
    void showWebHelp();
    void showAguilaHelp();
    void showHelpOperation();
    void showNutshellHelp();

    void changeFileFilter(int filterNr);

    void setRootIndex(const QModelIndex& index);

    void getScriptLinks();


private:

    //editor toolbar actions
    QAction *newDirAct;
    QAction *delDirAct;
    QAction *nextDirAct;
    QAction *prevDirAct;

    QAction *newfileAct;
    QAction *closefileAct;
    QAction *openfileAct;
    QAction *savefileAct;
    QAction *saveasfileAct;

    QAction *runmodelAct;
    QAction *pausemodelAct;
    QAction *killmodelAct;
    QAction *oldmodelAct;
    QAction *argsubsAct;

    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *clearOptionsAct;
    QAction *optionsAct;
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

    QAction *actionFind;
    QAction *actionReplace;
    QAction *actionFindNext;
    QAction *actionFindPrev;

    QAction *fontAct;
    QAction *fontIncreaseAct;
    QAction *fontDecreaseAct;
    QAction *decreaseIndentAct;
    QAction *increaseIndentAct;
    QAction *toggleHashAct;
    QAction *toggleReportAct;
    QAction *displayvarAct;
    QAction *syntaxAct;
    QAction *getdisplayvarAct;
    QAction *helpAct;
    QAction *helpWebAct;
    QAction *helppcrcalcAct;
    QAction *helpAguilaAct;
    QAction *helpNutshellAct;

    // explorer variables
    QFileSystemModel *dirModel;
    QFileSystemModel* fileModel;
    QItemSelectionModel *selectionModel;
    QItemSelectionModel *selectionDirModel;

    QString currentPath;
    QStack<QString> history;
    QStack<QString> future;

    QProcess *PCRProcess;
    QProcess *calcProcess;

};

#endif // NUTSHELLQT_H
