#ifndef NUTSHELLOPTIONS_H
#define NUTSHELLOPTIONS_H

#include <QDialog>
#include "ui_nutshelloptions.h"

class nutshellOptions : public QDialog, private Ui::nutshellOptions
{
    Q_OBJECT

public:
    explicit nutshellOptions(QWidget *parent = nullptr);
    ~nutshellOptions();

	 QString setExistingDirectory(QString title, QString bd);
	 QStringList baseDirs;
	 QStringList getOptions();
     void setupOptions(QStringList SL, double dpi);

public slots:
     void findCondaDir();
	 void findPcrcalcDir();
	 void findAguilaDir();
	 void findMapeditDir();
     void findGDALDir();
 };

#endif // NUTSHELLOPTIONS_H
