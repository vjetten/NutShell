#ifndef NUTSHELLOPTIONS_H
#define NUTSHELLOPTIONS_H

#include <QDialog>
#include "ui_nutshellOptions.h"

class nutshellOptions : public QDialog, private Ui::nutshellOptions
{
    Q_OBJECT

public:
    explicit nutshellOptions(QWidget *parent = 0);
    ~nutshellOptions();

	 QString setExistingDirectory(QString title, QString bd);
	 QStringList baseDirs;
	 QStringList getOptions();
	 void setupOptions(QStringList SL);

public slots:
	 void findPcrcalcDir();
	 void findAguilaDir();
	 void findMapeditDir();
 };

#endif // NUTSHELLOPTIONS_H
