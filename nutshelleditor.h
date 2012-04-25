#ifndef NUTSHELLEDITOR_H
#define NUTSHELLEDITOR_H

//#include <QPlainTextEdit>
#include <QtGui>


QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class LineNumberArea;
class nutshelleditor;


class nutshelleditor : public QPlainTextEdit
{
	Q_OBJECT

public:
	nutshelleditor(QWidget *parent = 0, int curs = 0);

	void lineNumberAreaPaintEvent(QPaintEvent *event);
	int lineNumberAreaWidth();
	int cursor;
	bool doerror;
	int errorline;
	void clearerror();
	bool dodisplay;

protected:
	void resizeEvent(QResizeEvent *event);
//	void mousePressEvent(QMouseEvent *evt);
//	void mouseReleaseEvent(QMouseEvent *evt);

public slots:
	void updateLineNumberAreaWidth(int newBlockCount);
	void highlightCurrentLine();
	void updateLineNumberArea(const QRect &, int);
	//void findText();

signals:
	void showVar(QPoint point);//QTextCursor);

private:
	QWidget *lineNumberArea;
};


class LineNumberArea : public QWidget
{
public:
	LineNumberArea(nutshelleditor *editor) : QWidget(editor) {
		codeEditor = editor;
	}

	QSize sizeHint() const {
		return QSize(codeEditor->lineNumberAreaWidth(), 0);
	}

protected:
	void paintEvent(QPaintEvent *event) {
		codeEditor->lineNumberAreaPaintEvent(event);
	}

private:
	nutshelleditor *codeEditor;
};



#endif // NUTSHELLEDITOR_H
