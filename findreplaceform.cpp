/* BASED ON:
 * Copyright (C) 2009  Lorenzo Bettini <http://www.lorenzobettini.it>
 * See COPYING file that comes with this distribution
 */

#include "findreplaceform.h"


//---------------------------------------------------------------
FindReplaceForm::FindReplaceForm(QWidget *parent) :
    QDialog(parent), textEdit(0)
{
    setupUi(this);

    errorLabel->setText("");
    //VJ find and replace are comboboxes
    textToFind->setInsertPolicy (QComboBox::InsertAtTop);
    textToFind->setMaxCount(40);
    textToFind->setDuplicatesEnabled(false);
    textToReplace->setInsertPolicy (QComboBox::InsertAtTop);
    textToReplace->setMaxCount(40);
    textToReplace->setDuplicatesEnabled(false);
    connect(textToFind, SIGNAL(editTextChanged(QString)), this, SLOT(textToFindChanged()));
    connect(textToFind, SIGNAL(editTextChanged(QString)), this, SLOT(validateRegExp(QString)));

    connect(regexCheckBox, SIGNAL(toggled(bool)), this, SLOT(regexpSelected(bool)));

    connect(findButton, SIGNAL(clicked()), this, SLOT(find()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    connect(replaceButton, SIGNAL(clicked()), this, SLOT(replace()));
    connect(replaceAllButton, SIGNAL(clicked()), this, SLOT(replaceAll()));
}
//---------------------------------------------------------------
FindReplaceForm::~FindReplaceForm()
{
    //delete ui;
}
//---------------------------------------------------------------
void FindReplaceForm::hideReplaceWidgets(bool doit)
{
    replaceLabel->setVisible(doit);
    textToReplace->setVisible(doit);
    replaceButton->setVisible(doit);
    replaceAllButton->setVisible(doit);
    if(doit)
    {
        setWindowTitle(tr("Find/Replace"));
        setWindowIcon(QIcon(":/resources/editreplace.png"));
    }
    else
    {
        setWindowTitle(tr("Find"));
        setWindowIcon(QIcon(":/resources/editFind.png"));
    }
}
//---------------------------------------------------------------
void FindReplaceForm::setTextEdit(QPlainTextEdit *textEdit_)
{
    textEdit = textEdit_;
    // copy pointer current texteditor to findreplaceform texteditor

    insertFind();
    // find the keyword under the cursor
}
//---------------------------------------------------------------
void FindReplaceForm::textToFindChanged()
{
    findButton->setEnabled(textToFind->currentText().size() > 0);
}
//---------------------------------------------------------------
void FindReplaceForm::regexpSelected(bool sel)
{
    if (sel)
        validateRegExp(textToFind->currentText());
    else
        validateRegExp("");
}
//---------------------------------------------------------------
void FindReplaceForm::validateRegExp(const QString &text)
{
    if (!regexCheckBox->isChecked() || text.size() == 0) {
        errorLabel->setText("");
        return; // nothing to validate
    }

    QRegularExpression reg(text,
                (caseCheckBox->isChecked() ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption));

    if (reg.isValid()) {
        showError("");
    } else {
        showError(reg.errorString());
    }
}
//---------------------------------------------------------------
void FindReplaceForm::showError(const QString &error)
{
    if (error == "") {
        errorLabel->setText("");
    } else {
        errorLabel->setText("<span style=\" font-weight:600; color:#ff0000;\">" +
                            error +
                            "</span>");
    }
}
//---------------------------------------------------------------
void FindReplaceForm::showMessage(const QString &message)
{
    if (message == "") {
        errorLabel->setText("");
    } else {
        errorLabel->setText("<span style=\" font-weight:600; color:green;\">" +
                            message +
                            "</span>");
    }
}
//---------------------------------------------------------------
void FindReplaceForm::find()
{
    _find(downRadioButton->isChecked());
}
//---------------------------------------------------------------
//! Find the keyword under the cursor and add to the list if not exist in the list
void FindReplaceForm::insertFind()
{
    bool fnd = false;
    int j = 0;

    if (!textEdit->textCursor().hasSelection())
    {
        if (!textToFind->currentText().isEmpty())
            j=-1;
            // textToFind->insertItem(0,"");
        textToFind->setCurrentIndex(j);
        return;
    }

    for(int i = 0; i < textToFind->count(); i++)
        if(textEdit->textCursor().selectedText() == textToFind->itemText(i))
        {
            fnd = true;
            j = i;
        }

    if (!fnd)
    {
        if (textToFind->count() == textToFind->maxCount())
            textToFind->removeItem(textToFind->count());
        textToFind->insertItem(0, textEdit->textCursor().selectedText());
        j = 0;
    }
    textToFind->setCurrentIndex(j);
    // go look for the next occurence, change index triggers search
}
//---------------------------------------------------------------
void FindReplaceForm::_find(bool next)
{
    if (!textEdit)
        return; // TODO: show some warning?

    // backward search
    bool back = !next;

    const QString &toSearch = textToFind->currentText();

    //VJ global check to flag if a text is found
    findresult = false;

    QTextDocument::FindFlags flags;

    if (back)
        flags |= QTextDocument::FindBackward;
    if (caseCheckBox->isChecked())
        flags |= QTextDocument::FindCaseSensitively;
    if (wholeCheckBox->isChecked())
        flags |= QTextDocument::FindWholeWords;

    if (regexCheckBox->isChecked())
    {
//        QRegularExpression reg(toSearch, (caseCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive));
        QRegularExpression reg(toSearch,
                    (caseCheckBox->isChecked() ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption));

        textCursor = textEdit->document()->find(reg, textCursor, flags);
        textEdit->setTextCursor(textCursor);
        findresult = (!textCursor.isNull());
    } else
    {
        findresult = textEdit->find(toSearch, flags);
    }

    if (findresult)
    {
        showError("");
    }
    else
    {
        showError("No match found");
        // VJ moved this to he main app, don't know if that is necesary?

        // move to the beginning or end (!) of the document for the next or prev find
        /* move to the main application, this blocked the main textEdit and until
            the find window is opened again?
        if (next)
            textCursor.setPosition(0);
        else
            textCursor.setPosition(textEdit->document()->characterCount()-1);
            //VJ move to the end when find previous
        */
    }
}
//---------------------------------------------------------------
void FindReplaceForm::replace()
{
    if (!textEdit->textCursor().hasSelection())
    {
        find();
    }
    else
    {
        textEdit->textCursor().insertText(textToReplace->currentText());
        find();
    }
}
//---------------------------------------------------------------
void FindReplaceForm::replaceAll()
{
    int i=0;
    while (textEdit->textCursor().hasSelection())
    {
        textEdit->textCursor().insertText(textToReplace->currentText());
        find();
        i++;
    }
    showMessage(QString("Replaced %1 occurrence(s)").arg(i));
}
//---------------------------------------------------------------

