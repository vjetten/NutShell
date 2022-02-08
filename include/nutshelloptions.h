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
     void setupOptions(QStringList SL);
     bool CondaInstall;
     bool GetCondaAllEnvs();

     QString CondaDirName;

public slots:
     void on_checkBoxInstallation_stateChanged(int state);
     bool findCondaDir();
	 void findPcrcalcDir();
     void findGDALDir();
private slots:
     void on_toolButton_2_clicked();
};

#endif // NUTSHELLOPTIONS_H
