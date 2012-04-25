#ifndef NUTSHELLLEGEND_H
#define NUTSHELLLEGEND_H

//#include <QDialog>
#include <QtGui>
#include "csf.h"
#include "ui_nutshelllegend.h"

class nutshellLegend : public QDialog, private Ui::nutshellLegend
{
	Q_OBJECT

	QStandardItemModel *legendmodel;

public:
	explicit nutshellLegend(QWidget *parent = 0);
	~nutshellLegend();

   int sizel;
  	CSF_LEGEND *theLegend;
	QString filename;
	QList<long> list;
	int prevrowcount;

	void makelegend(QString name);
	int CountClasses(MAP *legendmap);
	void closeEvent(QCloseEvent *event);

public slots:
	void AddNumbers();
	void RemoveNumbers();
	void ClearAll();
	void Accept();
	void Cancel();
};

#endif // NUTSHELLLEGEND_H
