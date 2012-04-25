#include "nutshellqt.h"

myListWidget::myListWidget(QWidget *parent)
    : QTableWidget(parent)
{

}


bool myListWidget::dropMimeData(int index, const QMimeData *data, Qt::DropAction action)
{
	QList<QUrl> urlList;
	QTableWidgetItem *item;
	QFileInfo info;
	QString fName;

	urlList = data->urls(); // retrieve list of urls

	foreach(QUrl url, urlList) // iterate over list
	{
		fName = url.toLocalFile();
		info.setFile( fName );

		item = new QTableWidgetItem(info.fileName());
//		insertItem(index, item);
		++index; // increment index to preserve drop order
	}

	return true;
}


QStringList myListWidget::mimeTypes () const
{
	QStringList qstrList;
	// list of accepted mime types for drop
	qstrList.append("text/uri-list");
	return qstrList;
}


Qt::DropActions myListWidget::supportedDropActions () const
{
	return Qt::CopyAction;
}


void myListWidget::mouseMoveEvent(QMouseEvent *event)
{
	// if not left button - return
	if (!(event->buttons() & Qt::LeftButton)) return;

	// if no item selected, return (else it would crash)
	if (currentItem() == NULL) return;

	QDrag *drag = new QDrag(this);
	QMimeData *mimeData = new QMimeData;

	// construct list of QUrls
	// other widgets accept this mime type, we can drop to them
	QList<QUrl> list;
	list.append(QUrl(currentItem()->text())); // only QUrl in list will be text of actual item

	// mime stuff
	mimeData->setUrls(list);
	drag->setMimeData(mimeData);

	// start drag
	drag->start(Qt::CopyAction | Qt::MoveAction);
}


