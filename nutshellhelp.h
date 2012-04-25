#ifndef NUTSHELLHELP_H
#define NUTSHELLHELP_H

#include <QDialog>
//#include <QtWebKit>
#include "ui_nutshellhelp.h"


class nutshellHelp : public QDialog, private Ui::nutshellHelp
{
    Q_OBJECT

public:
    explicit nutshellHelp(QWidget *parent = 0);
    ~nutshellHelp();
	 QString helpurl;
//	 QWebView *helpView;

	 void seturl(QString url);
	 void findOperation(QString operation);
	 void setuphelp(QString pcrhelp);

	 QStringList helphtml;

private:
    Ui::nutshellHelp *ui;
};

#endif // NUTSHELLHELP_H
