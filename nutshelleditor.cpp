/*
 * NutshellEditor
 * v 1.0
 * functions to create and delete editor PlainTextEdit in tabWidget
 * author: VJ 100814
 */

#include "nutshellqt.h"


//---------------------------------------------------------------
void nutshellqt::setupEditor()
{
    m_findReplaceDialog = new FindReplaceForm(this);
    m_findReplaceDialog->setModal(false);

    tabWidget->clear();
    tabWidget->setTabsClosable(true);

    connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(tabWidget, SIGNAL(currentChanged(int)),this, SLOT(changeSyntax(int)));

    //	displayvarAct = new QAction(QIcon(""), "&Show variable...", this);
    //	displayvarAct->setShortcut(Qt::Key_F4);
    //	connect(displayvarAct, SIGNAL(triggered()), this, SLOT(displayVar()));
    dosyntax = true;
}
//---------------------------------------------------------------
void nutshellqt::makeNewFile()
{
    QWidget *tabWidg = new QWidget;
    QHBoxLayout *tabLayout = new QHBoxLayout;
    nutshelleditor *myeditor = new nutshelleditor(this, 0);
    // make a plaintextedit with linenumbers

    myeditor->document()->setDefaultFont(QFont("Consolas", 10));
    myeditor->setFont(QFont("Consolas", 10));
    //myeditor->document()->setDefaultFont(QFont("Courier New", 10));

    myeditor->setWordWrapMode(QTextOption::NoWrap);

    connect(pasteAct, SIGNAL(triggered()), myeditor, SLOT(paste()));
    connect(copyAct, SIGNAL(triggered()), myeditor, SLOT(copy()));
    connect(cutAct, SIGNAL(triggered()), myeditor, SLOT(cut()));
    connect(undoAct, SIGNAL(triggered()), myeditor, SLOT(undo()));
    connect(redoAct, SIGNAL(triggered()), myeditor, SLOT(redo()));

    tabLayout->setMargin(2);
    tabLayout->addWidget(myeditor);

    editortabs newedit;
    // convenient structure
    newedit.editor = myeditor;
    newedit.fileName = QString("<empty %1>").arg(tabWidget->currentIndex()+1);
    newedit.filePath.isEmpty();
    newedit.highlighter = new Highlighter(myeditor->document());
    newedit.syntax = 0;

    ET.append(newedit);
    // ET defined as QList<editortabs> ET;


    tabWidget->addTab(tabWidg, newedit.fileName);
    tabWidget->setCurrentWidget(tabWidg);
    tabWidget->currentWidget()->setLayout(tabLayout);

}
//---------------------------------------------------------------
void nutshellqt::closeFile()
{
    closeTab(tabWidget->currentIndex());
}
//---------------------------------------------------------------
void nutshellqt::openFile()
{
    QFileDialog dialog(this);
    dialog.setViewMode(QFileDialog::Detail);
    QString fileName = dialog.getOpenFileName(this,
                                              "Open model/script", currentPath, "*.mod *.txt;;*.*");
    if (!fileName.isEmpty())
    {
#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        AddModel(fileName, 1);
#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
    }}
//---------------------------------------------------------------
bool nutshellqt::saveFile()
{
    if (ETfilePath.isEmpty()) {
        return saveasFile();
    } else {
        ETEditor->document()->setModified(false);
        ETEditor->clearerror();
        return saveFileName(ETfilePath);
    }
}
//---------------------------------------------------------------
bool nutshellqt::saveasFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, QString("Save File As"),ETfileName);
    if (fileName.isEmpty())
        return false;

    ETfilePath = fileName;
    ETfileName = QFileInfo(fileName).fileName();
    tabWidget->setTabText(tabWidget->currentIndex(), ETfileName);
    ETEditor->document()->setModified(false);

    return saveFileName(fileName);
}
//---------------------------------------------------------------
bool nutshellqt::saveFileName(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        ErrorMsg(QString("Cannot write file %1:\n%2.")
                 .arg(fileName)
                 .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << ETPlainText;
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    STATUS("File saved");
    return true;
}
//---------------------------------------------------------------
void nutshellqt::AddModel(QString name, int syntax)
{
    QFile file(name);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        ErrorMsg(QString("Cannot read file %1:\n%2.").arg(name)
                 .arg(file.errorString()));
        return;
    }

    makeNewFile();
    // make a new tab with an empty edit

    int nr = tabWidget->currentIndex();

    ET[nr].filePath = name;
    ET[nr].fileName = QFileInfo(name).fileName();
    tabWidget->setTabText(nr, ET.at(nr).fileName);
    syntax = 1;
    ET[nr].syntax = syntax;
    syntaxAct->setChecked(syntax == 1);
    showsyntax(syntax == 1);


    QTextStream in(&file);
    ET[nr].editor->setPlainText(in.readAll());
    // fill empty tab editor with file text
    file.close();

}
//---------------------------------------------------------------
void nutshellqt::closeTab(int index)
{
    if (index >= 0)
    {
        if (ETEditor->document()->isModified())
        {
            QMessageBox::StandardButton reply = WarningMsg(QString("Document is modified, save first?"));
            if (reply == QMessageBox::Yes)
                saveFile();
        }

        tabWidget->removeTab(index);
        ET.removeAt(index);
    }
}
//---------------------------------------------------------------
void nutshellqt::findDialog()
{
    if (tabWidget->currentIndex() < 0)
        return;
    m_findReplaceDialog->hideReplaceWidgets(false);
    m_findReplaceDialog->setTextEdit(ETEditor);
    m_findReplaceDialog->show();
}
//---------------------------------------------------------------
void nutshellqt::findReplaceDialog()
{
    if (tabWidget->currentIndex() < 0)
        return;

    m_findReplaceDialog->hideReplaceWidgets(true);
    m_findReplaceDialog->setTextEdit(ETEditor);
    m_findReplaceDialog->show();
}
//---------------------------------------------------------------
void nutshellqt::findNextfind()
{
    if (tabWidget->currentIndex() < 0)
        return;

    QTextCursor cur = ETEditor->textCursor();

    if (!cur.hasSelection())
        cur.select(QTextCursor::WordUnderCursor);
    ETEditor->setTextCursor(cur);
    // set the selection to word the cursor is in

    m_findReplaceDialog->setTextEdit(ETEditor);
    m_findReplaceDialog->findNext();
    if (!m_findReplaceDialog->findresult)
    {
        cur.setPosition(0);
        ETEditor->setTextCursor(cur);
        m_findReplaceDialog->findNext();
    }
}
//---------------------------------------------------------------
void nutshellqt::findPrevfind()
{
    if (tabWidget->currentIndex() < 0)
        return;

    QTextCursor cur = ETEditor->textCursor();

    if (!cur.hasSelection())
        cur.select(QTextCursor::WordUnderCursor);
    ETEditor->setTextCursor(cur);
    // set the selection to word the cursor is in

    m_findReplaceDialog->setTextEdit(ETEditor);
    m_findReplaceDialog->findPrev();
    if (!m_findReplaceDialog->findresult)
    {
        cur.setPosition(ETEditor->document()->characterCount()-1);
        ETEditor->setTextCursor(cur);
        m_findReplaceDialog->findPrev();
    }
}
//---------------------------------------------------------------
void nutshellqt::fontSelect()
{
    if (tabWidget->currentIndex() == -1)
        return;

    if (commandWindow->hasFocus())
        commandWindow->setFont(QFontDialog::getFont(0, commandWindow->font()));
    else
        ETEditor->setFont(QFontDialog::getFont(0, ETEditor->font()));
}
//---------------------------------------------------------------
void nutshellqt::fontDecrease()
{
    if (commandWindow->hasFocus())
    {
        QFont f = commandWindow->font();
        int size = f.pointSize();
        size--;
        size = max(6, size);
        f.setPointSize(size);
        commandWindow->setFont(f);
    }
    else
        if (tabWidget->currentIndex() >= 0)
        {
            QFont f = ETEditor->font();
            int size = f.pointSize();
            size--;
            size = max(6, size);
            f.setPointSize(size);
            ETEditor->setFont(f);
        }
}
//---------------------------------------------------------------
void nutshellqt::fontIncrease()
{
    if (commandWindow->hasFocus())
    {
        QFont f = commandWindow->font();
        int size = f.pointSize();
        size++;
        size = min(32, size);
        f.setPointSize(size);
        commandWindow->setFont(f);
    }
    else
        if (tabWidget->currentIndex() >= 0)
        {
            QFont f = ETEditor->font();
            int size = f.pointSize();
            size++;
            size = min(32, size);
            f.setPointSize(size);
            ETEditor->setFont(f);
        }
}
//---------------------------------------------------------------
void nutshellqt::increaseHash()
{
    insertType = INSERTHASH;
    doIndent(false);
}
//---------------------------------------------------------------
void nutshellqt::increaseIndent()
{
    insertType = INSERTSPACE;
    doIndent(false);
}
//---------------------------------------------------------------
void nutshellqt::decreaseIndent()
{
    insertType = INSERTSPACE;
    doIndent(true);
}
//---------------------------------------------------------------
void nutshellqt::increaseReport()
{
    insertType = INSERTREPORT;
    doIndent(false);
}
//---------------------------------------------------------------
void nutshellqt::doIndent(bool back)
{
    if (tabWidget->currentIndex() == -1)
        return;

    QTextCursor cur = ETEditor->textCursor();
    int a = cur.anchor();
    int p = cur.position();

    if (a > p)
    {
        int q = a; a = p; p = q;
    }
    // switch bottomto top if selection from down upwards
    cur.setPosition(a);
    cur.movePosition(QTextCursor::StartOfBlock,QTextCursor::MoveAnchor);
    a = cur.position();
    // save a new anchor at the beginning of the line of the selected text
    cur.setPosition(p);
    cur.movePosition(QTextCursor::EndOfBlock,QTextCursor::MoveAnchor);
    p = cur.position();

    cur.setPosition(a);
    cur.setPosition(p, QTextCursor::KeepAnchor);
    // set a new selection with the new beginning
    QString str = cur.selection().toPlainText();
    QStringList line, list = str.split("\n");
    // get the selected text and split into lines

    for (int i = 0; i < list.count(); i++)
    {
        switch (insertType)
        {
        case INSERTSPACE:
            if (back)
            {
                if (list[i].indexOf(" ") == 0) list[i].remove(0,1);
            }
            else
                list[i].insert(0," ");
            break;
        case INSERTHASH:
            if (list[i].startsWith("##"))
            {
                int k = list[i].indexOf("##");
                list[i].remove(k,2);
            }
            else
                list[i].insert(0,"##");
            break;
        case INSERTREPORT:
            if (list[i].startsWith("report "))
            {
                int k = list[i].indexOf("report ");
                list[i].remove(k,7);
            }
            else
                list[i].insert(0,"report ");
            break;
        }
    }
    str=list.join("\n");

    cur.removeSelectedText();
    cur.insertText(str);
    // put the new text back

    cur.setPosition(a);
    cur.movePosition(QTextCursor::StartOfBlock,QTextCursor::MoveAnchor);
    a = cur.position();
    // save a new anchor at the beginning of the line of the selected text
    p = a + str.size();
    cur.setPosition(p);
    cur.movePosition(QTextCursor::StartOfBlock,QTextCursor::MoveAnchor);
    p = cur.position();

    cur.setPosition(a);
    cur.setPosition(p, QTextCursor::KeepAnchor);
    // reselect the text for more indents

    ETEditor->setTextCursor(cur);
    // put the whole thing back into the main text
}
//---------------------------------------------------------------
void nutshellqt::showsyntax(bool doit)
{
    if (tabWidget->currentIndex() == -1)
        return;
    doit = true;
    ETHighlighter->dosyntax = doit;
    ET[tabWidget->currentIndex()].syntax = (doit ? 1 : 0);
    ETHighlighter->rehighlight();
}
//---------------------------------------------------------------
void nutshellqt::changeSyntax(int index)
{
    if (index >= 0)
        syntaxAct->setChecked(ET[index].highlighter->dosyntax);
}
//---------------------------------------------------------------
void nutshellqt::displayVar(QPoint point)//QTextCursor cur)
{

    //if (tabWidget->currentIndex() < 0)
    //return;
    QApplication::setOverrideCursor(Qt::WaitCursor);

    showPCR();

    QString var;
    //	QTextCursor cur = ETEditor->textCursor();
    QTextCursor cur = ETEditor->cursorForPosition(point);

    if (!cur.hasSelection())
    {
        bool dotone = false;
        bool dottwo = false;

        cur.select(QTextCursor::WordUnderCursor);
        QString word = cur.selectedText();
        int a = cur.selectionStart();
        int p = cur.selectionEnd();

        QString text = ETPlainText;

        if (a > 0 && text.data()[a-1] == '.')
            dotone = true;
        if (text.data()[p] == '.')
            dottwo = true;

        var = cur.selectedText();

        if (dotone)
        {
            cur.setPosition(a-2, QTextCursor::MoveAnchor);
            cur.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
            a = cur.position();
        }
        if (dottwo)
        {
            cur.setPosition(p+2, QTextCursor::MoveAnchor);
            cur.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
            p = cur.position();
        }
        cur.setPosition(a, QTextCursor::MoveAnchor);
        cur.setPosition(p, QTextCursor::KeepAnchor);

        var = cur.selectedText();

    }

    ETEditor->setTextCursor(cur);
    // set the new selection in the document


    QString all;
    QStringList SL;
    QStringList binding;
    bool go = false;

    all = ETPlainText;
    SL = all.split("\n");

    binding.clear();
    // strip trailing spaces
    for(int i = 0; i < SL.count(); i++)
    {
        SL[i].remove(QRegExp("^\\s+|\\s+$"));
    }

    // get the binding section, only lines with = in them and no comments
    foreach (QString str, SL)
    {
        if (str.indexOf("binding") == 0)
            go = true;
        if (str.indexOf("timer") == 0 ||
                str.indexOf("areamap") == 0 ||
                str.indexOf("initial") == 0 ||
                str.indexOf("dynamic") == 0)
            go = false;

        if (go && str.contains('='))
        {
            QString line;
            line = str;
            line.remove(str.indexOf(";"),str.count()+10);
            line.remove(" ");
            if (line.contains("="))
                binding << line;
        }
    }

    // replace variable name by binding name
    foreach(QString stra, binding)
    {
        QStringList bind = stra.split("=");
        if (bind[0] == var)
            var = bind[1];
    }

    if (!var.contains("."))//QFileInfo(var).suffix().isEmpty())
    {
        var = var + "00000000";
        var.remove(8,100);
        var = var + ".001";
    }

    QDir dir(currentPath);
    if (dir.exists(var))
    {
        QModelIndex ind = fileModel->index(var);
        selectionModel->clearSelection();
        selectionModel->setCurrentIndex(ind, QItemSelectionModel::Select);
        PerformAction(GetActionType());
    }
    else
        STATUS("Not an existing file.");

    QApplication::restoreOverrideCursor();
}
//---------------------------------------------------------------
void nutshellqt::showHelp()
{
    QDesktopServices::openUrl(QUrl("file:///"+PCRasterDocDirName + "index.html"));
}
//---------------------------------------------------------------
void nutshellqt::showWebHelp()
{
    QDesktopServices::openUrl(QUrl("http://pcraster.geo.uu.nl/documentation/PCRaster/html/index.html"));
}
//---------------------------------------------------------------
void nutshellqt::showAguilaHelp()
{
    QDesktopServices::openUrl(QUrl("http://pcraster.geo.uu.nl/documentation/Aguila/Aguila.pdf"));
}
//---------------------------------------------------------------
void nutshellqt::showHelpOperation()
{
    if (tabWidget->currentIndex() < 0)
        return;

    QString helptxt = ETEditor->textCursor().selectedText();
    qDebug() << helptxt;
    QString URL = "http://pcraster.geo.uu.nl/documentation/PCRaster/html/";
    QString found = "";

    if (helptxt.contains("$"))
        found = QString("secpcrcalcscriptfeatures.html#substitution-of-arguments");
    else
        if (helptxt.contains("--"))
            found = QString("secimport.html#overview-of-global-options");
        else
            if (helptxt.contains("#!"))
                found = QString("secimport.html#global-options");
            else
                if (helptxt.toUpper() == "BINDING" ||
                        helptxt.toUpper() == "INITIAL" ||
                        helptxt.toUpper() == "TIMER" ||
                        helptxt.toUpper() == "AREAMAP" ||
                        helptxt.toUpper() == "DYNAMIC")
                    found = QString("secdyn.html#%1-section").arg(helptxt);
                else
                {
                    if (helptxt.contains("lookup"))
                        helptxt = "lookup";
                    // lookupscalar and so on
                    if (helptxt.contains("timeinput") && helptxt != "timeinput")
                        helptxt = "timeinput...";
                    // timeinputscalar and so on, but not timrinput itself
                    found = QString("op_%1.html").arg(helptxt);
                    // all the rest
                }
    qDebug() << QString(URL+found);
    QDesktopServices::openUrl(QUrl(URL+found));

    //help.helpurl = "";
    //   help.findOperation(cur.selectedText());
    //   if (!help.helpurl.isEmpty())
    //      QDesktopServices::openUrl(QUrl("file:///"+PCRasterDocDirName + help.helpurl));
    //   else
    //      STATUS("No help found.");
    //	if (!help.helpurl.isEmpty())
    //	{
    //		help.show();
    //		help.helpView->load(QUrl("file:///"+PCRasterDocDirName + help.helpurl));
    //	}
    //	else
    //		STATUS("No help found.");

}
//---------------------------------------------------------------
