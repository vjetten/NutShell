/*
 * Copyright (C) 2009  Lorenzo Bettini <http://www.lorenzobettini.it>
 * See COPYING file that comes with this distribution
 */

#ifndef FINDREPLACEFORM_H
#define FINDREPLACEFORM_H

#include <QtGui>
#include "ui_findreplaceform.h"

class FindReplaceForm : public QDialog,private Ui::FindReplaceForm
{
	Q_OBJECT

public:
	FindReplaceForm(QWidget *parent = 0);
	virtual ~FindReplaceForm();

	bool findresult;

	void setTextEdit(QPlainTextEdit *textEdit_);

	void hideReplaceWidgets(bool doit);


public slots:
	void _find(bool down);

	void find();
	void findNext() { _find(true); }

	void findPrev() { _find(false); }
	void replace();

	void replaceAll();

	void insertFind();

protected:
	void showError(const QString &error);

	void showMessage(const QString &message);

protected slots:
	void textToFindChanged();

	void validateRegExp(const QString &text);

	void regexpSelected(bool sel);

protected:
	QTextCursor textCursor;
	QPlainTextEdit *textEdit;
};

#endif // FINDREPLACEFORM_H
