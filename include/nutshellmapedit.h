#ifndef NUTSHELLMAPEDIT_H
#define NUTSHELLMAPEDIT_H

#include <QMainWindow>
#include "ui_nutshellmapedit.h"

namespace Ui {
class nutshellmapedit;
}

class nutshellmapedit : public QMainWindow, private Ui::nutshellmapedit
{
    Q_OBJECT

public:
    explicit nutshellmapedit(QWidget *parent = nullptr);
    ~nutshellmapedit();


    void loadmap(QString filename);
//private:
//    Ui::nutshellmapedit *ui;
};

#endif // NUTSHELLMAPEDIT_H
