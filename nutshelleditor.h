#ifndef NUTSHELLEDITOR_H
#define NUTSHELLEDITOR_H

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
   bool doReport;

protected:
	void resizeEvent(QResizeEvent *event);
   void mouseDoubleClickEvent ( QMouseEvent * event );

public slots:
	void updateLineNumberAreaWidth(int newBlockCount);
	void highlightCurrentLine();
	void updateLineNumberArea(const QRect &, int);
   //void displayVar();

signals:
	void showVar(QPoint point);//QTextCursor);

private:
	QWidget *lineNumberArea;
// actions themselves are defined in the main class
//    QAction *cutAct;
//    QAction *copyAct;
//    QAction *pasteAct;
//    QAction *undoAct;
//    QAction *redoAct;
//    QAction *actionFind;
//    QAction *actionReplace;
//    QAction *actionFindNext;
//    QAction *actionFindPrev;
//    QAction *fontAct;
//    QAction *fontIncreaseAct;
//    QAction *fontDecreaseAct;
//    QAction *decreaseIndentAct;
//    QAction *increaseIndentAct;
//    QAction *toggleHashAct;
//    QAction *toggleReportAct;
//    QAction *syntaxAct;

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
