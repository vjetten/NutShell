#ifndef NUTSHELLDRAGDROP_H
#define NUTSHELLDRAGDROP_H

#include "nutshellqt.h"

//---------------------------------------------------------------
class myListWidget : public QTableWidget
{
	Q_OBJECT

public:
	myListWidget(QWidget *parent = 0);

	virtual bool dropMimeData(int index, const QMimeData *data, Qt::DropAction action);
	QStringList mimeTypes() const;
	Qt::DropActions supportedDropActions () const;

//	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:

	QPoint dragStartPosition;
};



#endif // NUTSHELLDRAGDROP_H
