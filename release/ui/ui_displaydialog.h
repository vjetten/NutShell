/********************************************************************************
** Form generated from reading UI file 'displaydialog.ui'
**
** Created: Sat 21. Apr 09:18:23 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISPLAYDIALOG_H
#define UI_DISPLAYDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_displayDialog
{
public:

    void setupUi(QDialog *displayDialog)
    {
        if (displayDialog->objectName().isEmpty())
            displayDialog->setObjectName(QString::fromUtf8("displayDialog"));
        displayDialog->resize(400, 300);

        retranslateUi(displayDialog);

        QMetaObject::connectSlotsByName(displayDialog);
    } // setupUi

    void retranslateUi(QDialog *displayDialog)
    {
        displayDialog->setWindowTitle(QApplication::translate("displayDialog", "Dialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class displayDialog: public Ui_displayDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPLAYDIALOG_H
