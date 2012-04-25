/*
 * nutshelLinenumber
 * v 1.0
 * PCRaster editor,
 * adapted from linenumbers are from the codeeditor example in QT examples
 *	\Qt\4.6.2\examples\widgets\codeeditor
 * author: VJ 100814
 */


#include "nutshellqt.h"


//---------------------------------------------------------------
nutshelleditor::nutshelleditor(QWidget *parent, int curs) : QPlainTextEdit(parent)
{
	cursor = curs;
	lineNumberArea = new LineNumberArea(this);

	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
	connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
	doerror = false;
	errorline = 0;
	updateLineNumberAreaWidth(0);
	highlightCurrentLine();
}
//---------------------------------------------------------------
int nutshelleditor::lineNumberAreaWidth()
{
	int w = 5;
	if (cursor > 0)
		w = 3;
	return  (fontMetrics().width(QLatin1Char('9')) * w);
}
//---------------------------------------------------------------
void nutshelleditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}
//---------------------------------------------------------------
void nutshelleditor::updateLineNumberArea(const QRect &rect, int dy)
{
	if (dy)
		lineNumberArea->scroll(0, dy);
	else
		lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

	if (rect.contains(viewport()->rect()))
		updateLineNumberAreaWidth(0);
}
//---------------------------------------------------------------
void nutshelleditor::resizeEvent(QResizeEvent *e)
{
	QPlainTextEdit::resizeEvent(e);

	QRect cr = contentsRect();
	lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}
//---------------------------------------------------------------
void nutshelleditor::highlightCurrentLine()
{
	if (cursor > 0)
		return;

	QList<QTextEdit::ExtraSelection> extraSelections;

	if (!isReadOnly()) {
		QTextEdit::ExtraSelection selection;
		QColor lineColor;

		if (doerror)
		{
			lineColor = QColor(Qt::yellow);
			doerror = false;
		}
		else
		{
			lineColor = QColor(Qt::blue).lighter(180);
			errorline = 0;
		}

		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}

	setExtraSelections(extraSelections);

}
//---------------------------------------------------------------
void nutshelleditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
	QPainter painter(lineNumberArea);

	if (cursor > 0)
		painter.fillRect(event->rect(), QColor(245,245,245, 255));
	else
		painter.fillRect(event->rect(), QColor(230, 230, 230, 255));

	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + (int) blockBoundingRect(block).height();

	while (block.isValid() && top <= event->rect().bottom())
	{
		if (block.isVisible() && bottom >= event->rect().top())
		{
			QString number;
			number = " ";
			if (cursor == 1)
			{
				//if(blockNumber == blockCount()-1)
				number = "->";
			}
			else
				if (cursor == 2)
					number = "  ";
			else
				if (cursor == 0)
				{
				number = QString::number(blockNumber + 1);
				if ((blockNumber+1) == errorline)
				{
					QImage image(":/resources/errorline.png");
					painter.setPen(QColor(200, 0, 0, 255));
					QFont f = painter.font();
					f.setBold(true);
					painter.setFont(f);
					painter.drawImage(0,top, image);
				}
				else
				{
					painter.setPen(QColor(64, 64, 64, 255)); // color line numbers
					QFont f = painter.font();
					f.setBold(false);
					painter.setFont(f);
				}
			}

			painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
								  Qt::AlignRight, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + (int) blockBoundingRect(block).height();
		++blockNumber;
	}
}
//---------------------------------------------------------------
void nutshelleditor::clearerror()
{
	doerror = false;
	errorline = 0;
    highlightCurrentLine();
	repaint();
}
//---------------------------------------------------------------
//void nutshelleditor::mouseReleaseEvent(QMouseEvent *evt)
//{
//	if (evt->modifiers() == Qt::ControlModifier &&
//		 evt->button() == Qt::LeftButton)
//		emit showVar(evt->pos());
//	else
//	{
//		QPlainTextEdit::textCursor().clearSelection();
//		QPlainTextEdit::mouseReleaseEvent(evt);
//	}
//}
////---------------------------------------------------------------
//void nutshelleditor::mousePressEvent(QMouseEvent *evt)
//{
//	if (evt->modifiers() == Qt::ControlModifier &&
//		 evt->button() == Qt::LeftButton)
//		emit showVar(evt->pos());
//	else
//	{
//		QPlainTextEdit::textCursor().clearSelection();
//		QPlainTextEdit::mousePressEvent(evt);
//	}
//}
////---------------------------------------------------------------
