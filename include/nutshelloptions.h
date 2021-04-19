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
     void setupOptions(QStringList SL, double dpi, bool CI);
     bool CondaInstall;
public slots:
     void on_checkBoxInstallation_stateChanged(int state);
     void findCondaDir();
	 void findPcrcalcDir();
     void findGDALDir();
 };

#endif // NUTSHELLOPTIONS_H
