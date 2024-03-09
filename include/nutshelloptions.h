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

     QString getOptions();
     void setOptions(QString S);
     bool CondaInstall;
     bool GetCondaAllEnvs();
     QString CondaDirName;
     QStringList allCondaDirNames;

public slots:
     bool findCondaDir(QString dirS);
private slots:
     void on_combo_envs_currentTextChanged(const QString &arg1);
};

#endif // NUTSHELLOPTIONS_H
