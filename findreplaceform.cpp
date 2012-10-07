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
	textToFind->setMaxCount(20);
	textToReplace->setInsertPolicy (QComboBox::InsertAtTop);
	textToReplace->setMaxCount(20);
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

   // VJ set search field to selection before opening find dialog
	//	textToFind->setText(textEdit->textCursor().selectedText());
	// add selected text to find combobox
	insertFind();

	// VJ why this, replace with empty should be available and when?
	//connect(textEdit, SIGNAL(copyAvailable(bool)), replaceButton, SLOT(setEnabled(bool)));
	//connect(textEdit, SIGNAL(copyAvailable(bool)), replaceAllButton, SLOT(setEnabled(bool)));
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

	QRegExp reg(text,
					(caseCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive));

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
								  "</spam>");
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
								  "</spam>");
	}
}
//---------------------------------------------------------------
void FindReplaceForm::find()
{
	_find(downRadioButton->isChecked());
}
//---------------------------------------------------------------
void FindReplaceForm::insertFind()
{
	bool fnd = false;
	for(int i = 0; i < textToFind->count(); i++)
		if(textEdit->textCursor().selectedText() == textToFind->itemText(i))
			fnd = true;
	if (!fnd)
	{
		if (textToFind->count() == textToFind->maxCount())
			textToFind->removeItem(textToFind->count());
		textToFind->insertItem(0, textEdit->textCursor().selectedText());
	}
	textToFind->setCurrentIndex(0);
}
//---------------------------------------------------------------
void FindReplaceForm::_find(bool next)
{
	if (!textEdit)
		return; // TODO: show some warning?

	// VJ use the selected text as a first guess, facilitates find next/prev
	// insert in find combobox
	//	if (textEdit->textCursor().hasSelection())
	//	insertFind();

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

	if (regexCheckBox->isChecked()) {
		QRegExp reg(toSearch,
						(caseCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive));


		//qDebug() << "searching for regexp: " << reg.pattern();

		textCursor = textEdit->document()->find(reg, textCursor, flags);
		textEdit->setTextCursor(textCursor);
		findresult = (!textCursor.isNull());
	} else {
		//qDebug() << "searching for: " << toSearch;
		findresult = textEdit->find(toSearch, flags);
	}

	if (findresult) {
		showError("");
	} else {
		showError(tr("no match found"));
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
	if (!textEdit->textCursor().hasSelection()) {
		find();
	} else {
		textEdit->textCursor().insertText(textToReplace->currentText());
		find();
	}
}
//---------------------------------------------------------------
void FindReplaceForm::replaceAll()
{
	int i=0;
	while (textEdit->textCursor().hasSelection()){
		textEdit->textCursor().insertText(textToReplace->currentText());
		find();
		i++;
	}
	showMessage(tr("Replaced %1 occurrence(s)").arg(i));
}
//---------------------------------------------------------------

