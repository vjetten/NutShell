/*
 * NutshellEditor

 * functions to create and delete editor PlainTextEdit in tabWidget
 * Author: VJ 140222,181001
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
    //connect(tabWidget, SIGNAL(currentChanged(int)),this, SLOT(changeSyntax(int)));

    dosyntax = true;

}
//---------------------------------------------------------------
void nutshellqt::makeNewScriptFile()
{
    makeNewFile(true, 1);
}
//---------------------------------------------------------------
void nutshellqt::makeNewFile(bool script, int typ)
{
    QWidget *tabWidg = new QWidget;
    QHBoxLayout *tabLayout = new QHBoxLayout;
    nutshelleditor *myeditor = new nutshelleditor(this, 0);
    // make a plaintextedit with linenumbers

    myeditor->document()->setDefaultFont(QFont("Consolas", 11));
    myeditor->setFont(QFont("Consolas", 11));
    //myeditor->document()->setDefaultFont(QFont("Courier New", 10));
    myeditor->setTabStopDistance(60);

    myeditor->setWordWrapMode(QTextOption::NoWrap);

   // tabLayout->setMargin(2);
    tabLayout->addWidget(myeditor);

    editortabs newedit;
    // convenient structure
    newedit.editor = myeditor;
    newedit.fileName = QString("<empty %1>").arg(tabWidget->currentIndex()+1);
    newedit.filePath.isEmpty();
    if (typ == 1)
        newedit.highlighter = new PCRHighlighter(myeditor->document());
    else
        if (typ == 2)
            newedit.highlighter = new PythonHighlighter(myeditor->document());
    newedit.syntax = typ;
    newedit.editor->doReport = false;
    newedit.editor->fold_binding = false;
    newedit.editor->fold_areamap = false;
    newedit.editor->fold_timer = false;
    newedit.editor->fold_initial = false;
    newedit.editor->fold_dynamic = false;

    if (script)
    {
        newedit.editor->appendPlainText("############################################");
        newedit.editor->appendPlainText("# Model:                                   #");
        newedit.editor->appendPlainText("# Date:                                    #");
        newedit.editor->appendPlainText("# Version: 1.0                             #");
        newedit.editor->appendPlainText("# Author:                                  #");
        newedit.editor->appendPlainText("############################################\n\n");
        newedit.editor->appendPlainText("binding\n\n");
        newedit.editor->appendPlainText("areamap\n\n");
        newedit.editor->appendPlainText("timer\n1 10 1;\n\n");
        newedit.editor->appendPlainText("initial\n\n");
        newedit.editor->appendPlainText("dynamic\n\n");
    }

    ET.append(newedit);
    // ET defined as QList<editortabs> ET;

    tabWidget->addTab(tabWidg, newedit.fileName);
    tabWidget->setCurrentWidget(tabWidg);
    tabWidget->currentWidget()->setLayout(tabLayout);
}
//---------------------------------------------------------------
//void nutshellqt::makeNewScript()
//{
//    makeNewFile();
//    ETEditor->document()->setPlainText
//}
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
        "Open model/script", currentPath, "*.mod *.txt *.cmd *.bat;;*.py;;*.*");
    if (!fileName.isEmpty()) {
        if (QFileInfo(fileName).suffix().toUpper() == "PY")
            AddModel(fileName, 2);
        else
            AddModel(fileName, 1);
    }
}
//---------------------------------------------------------------
bool nutshellqt::saveFile()
{
    if (!ETExists)
    {
       // QMessageBox::StandardButton reply = ErrorMsg(QString("You do not have a file to save!"));
        return false;
    }

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
    if (!ETExists)
    {
        QMessageBox::StandardButton reply = ErrorMsg(QString("You do not have script a file to save!"));
        return false;
    }

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
    out << ETPlainText;
    STATUS("File saved");
    return true;
}
//---------------------------------------------------------------
void nutshellqt::AddModel(QString name, int syntax)
{

    if (!isTextFile(name))
        return;

    QFile fff(name);
    fff.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&fff);
    QString txt = in.readAll();
    fff.close();

    makeNewFile(false, syntax);
    // make a new tab with an empty edit

    int nr = tabWidget->currentIndex();

    ET[nr].filePath = name;
    ET[nr].fileName = QFileInfo(name).fileName();
    tabWidget->setTabText(nr, ET.at(nr).fileName);
    tabWidget->setTabToolTip(nr,ET.at(nr).filePath);

    ET[nr].syntax = syntax;
  //  syntaxAct->setChecked(syntax == 1);
  //  showsyntax(syntax == 1);

    ET[nr].editor->setPlainText(txt);
    // fill empty tab editor with file text

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
    if (!ETExists)
        return;
    m_findReplaceDialog->hideReplaceWidgets(false);
    m_findReplaceDialog->setTextEdit(ETEditor);
    m_findReplaceDialog->show();
}
//---------------------------------------------------------------
void nutshellqt::findReplaceDialog()
{
    if (!ETExists)
        return;

    m_findReplaceDialog->hideReplaceWidgets(true);
    m_findReplaceDialog->setTextEdit(ETEditor);
    m_findReplaceDialog->show();
}
//---------------------------------------------------------------
void nutshellqt::findNextfind()
{
    if (!ETExists)
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
    if (!ETExists)
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
    bool ok;
    if (commandWindow->hasFocus())
        commandWindow->setFont(QFontDialog::getFont(&ok, commandWindow->font()));
    else
        if (ETExists)
            ETEditor->setFont(QFontDialog::getFont(&ok, ETEditor->font()));
}
//---------------------------------------------------------------
void nutshellqt::fontDecrease()
{
    genfontsize--;
    genfontsize =  std::max(6,genfontsize);

    qApp->setStyleSheet(QString("* { font-size: %1px; }").arg(genfontsize));
}
//---------------------------------------------------------------
void nutshellqt::fontIncrease()
{
    genfontsize++;
    genfontsize =  std::min(32,genfontsize);

    qApp->setStyleSheet(QString("* { font-size: %1px; }").arg(genfontsize));
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
    if (!ETExists)
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
//void nutshellqt::showsyntax(bool doit)
//{
//    if (!ETExists)
//        return;
//    doit = true;
//   // ETHighlighter->dosyntax = doit;
//    ET[tabWidget->currentIndex()].syntax = (doit ? 1 : 0);
//    ETHighlighter->rehighlight();
//}
//---------------------------------------------------------------
//void nutshellqt::changeSyntax(int index)
//{
//    if (index >= 0) {
//        syntaxAct->setChecked(true);//ET[index].highlighter->dosyntax);
//    }
//    if (index >= 0)
//        changeFileFilter(_filternr);
//}
//---------------------------------------------------------------
void nutshellqt::displayVar()
{

    if (!ETExists)
        return;
    if (!ETEditor->hasFocus())
        return;

    QTextCursor cur = ETEditor->textCursor();
    cur.select(QTextCursor::WordUnderCursor);

    QString var = cur.selectedText().simplified();

    if (!getScriptReport(true))
    {
        QMessageBox::warning(this,"NutShell",QString("Selection not found in directory."));
        return;
    }

    // expand selection if cursor is on a word
    if (cur.hasSelection())
    {
        bool dotone = false;
        bool dottwo = false;

        //cur.select(QTextCursor::WordUnderCursor);
        //QString word = cur.selectedText();
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

    if (var.isEmpty())
    {
        // QMessageBox::warning(this,"NutShell",QString("Selection not valid."));
        return;
    }

    int nr = 0;
    bool found = false;
    for (int i = 0; i < reportNames.count(); i++)
    {
        if (var == reportNames[i].reportName ||
            var == reportNames[i].fileName)
        {
            var = reportNames[i].fileName;
            nr = i;
            found = true;
            break;
        }
    }
    if (!found)
    {
        //QMessageBox::warning(this,"NutShell",QString("Selected variable not reported and not in binding."));
        return;
    }


    if (reportNames[nr].isSeries)
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
        QMessageBox::warning(this,"NutShell",QString("Selected variable not found in directory."));

}
//---------------------------------------------------------------------------
void nutshellqt::getScriptLinks()
{
    if (ETExists)
    {
        QTextCharFormat UL;
        QTextCursor cur = ETEditor->textCursor();

        ETEditor->doReport = !ETEditor->doReport;
        getdisplayvarAct->setChecked(ETEditor->doReport);

        if(ETEditor->doReport)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);

            // get a list of reported words
            if (!getScriptReport(true))
            {
                STATUS("No reported variables to show.");
                ETEditor->doReport = false;
                QApplication::restoreOverrideCursor();

                return;
            }


            UL.setBackground(QColor(255,255,164,255));

            for(int i = 0; i < reportNames.count(); i++)
            {
                const QString &search = reportNames[i].reportName;
                cur = ETEditor->document()->find(search, 0, QTextDocument::FindCaseSensitively | QTextDocument::FindWholeWords);

                QString S = cur.block().text();
                if (S.indexOf("#",0) < 0 || S.indexOf("#",0) > cur.positionInBlock ())
                {
                    cur.setCharFormat(UL);
                    ETEditor->setTextCursor(cur);
                }
                //find first occurence and set cursor if not commented out

                // find other occurences
                for(;;)
                {
                    cur = ETEditor->document()->find(search, cur, QTextDocument::FindCaseSensitively | QTextDocument::FindWholeWords);
                    if (cur.isNull())
                        break;
                    QString S = cur.block().text();
                    if (S.indexOf("#",0) < 0 || S.indexOf("#",0) > cur.positionInBlock())
                    {
                        cur.setCharFormat(UL);
                        ETEditor->setTextCursor(cur);
                    }
                }

            }
            cur = ETEditor->textCursor();
            cur.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
            ETEditor->setTextCursor(cur);
            QApplication::restoreOverrideCursor();
        }
        else
        {
            UL.setBackground(QColor(255,255,255,255));
            cur = ETEditor->textCursor();
            cur.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
            cur.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
            cur.setCharFormat(UL);

            cur.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);

            ETEditor->setTextCursor(cur);

        }
    }
    ETEditor->document()->setModified(false);
}
//---------------------------------------------------------------
//! Delete the current line in the active script, Ctrl-D and Ctrl-Y work both
void nutshellqt::deleteLine()
{
    if (ETExists)
    {
        QTextCursor cur = ETEditor->textCursor();

        cur.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
        cur.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor);
        cur.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);

        cur.removeSelectedText();

        // cur.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    }
}
//---------------------------------------------------------------
