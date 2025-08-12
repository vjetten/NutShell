/*
 * nutshelLinenumber

 * PCRaster editor,
 * adapted from linenumbers are from the codeeditor example in QT examples
 *	\Qt\4.6.2\examples\widgets\codeeditor
 * Author: VJ 140222
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
   return  (fontMetrics().horizontalAdvance(QLatin1Char('9')) * w);
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

void nutshelleditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
   QPainter painter(lineNumberArea);

   if (cursor > 0)
      painter.fillRect(event->rect(), QColor(245, 245, 245, 255));
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
            number = ">>";
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
void nutshelleditor::mouseDoubleClickEvent ( QMouseEvent * event )
{

   QTextCursor cur = textCursor();
   cur.select(QTextCursor::WordUnderCursor);

   if (cur.selectedText().simplified() == "binding")
      scriptFold(1);
   else
      if (cur.selectedText().simplified() == "areamap")
         scriptFold(2);
      else
         if (cur.selectedText().simplified() == "timer")
            scriptFold(3);
         else
            if (cur.selectedText().simplified() == "initial")
               scriptFold(4);
            else
               if (cur.selectedText().simplified() == "dynamic")
                  scriptFold(5);
//               else
//               {
//                  QKeyEvent key(QEvent::KeyPress, Qt::Key_F4, Qt::NoModifier);
//                  QApplication::sendEvent(this, &key);
//               }

   QPlainTextEdit::mouseDoubleClickEvent(event);
}
//---------------------------------------------------------------
int nutshelleditor::scriptFindSectionBlock(QString str)
{
   QTextCursor cur = document()->find(str, 0, QTextDocument::FindWholeWords);

   QString S = cur.block().text().simplified();
   if (S.isEmpty())
      return -1;

   if (S.contains("#"))
      S.remove(S.indexOf("#"), S.size()+1);
   S = S.simplified();
   while (S != str && !cur.atEnd())
   {
      cur = document()->find(str, cur.position(), QTextDocument::FindWholeWords);
      S = cur.block().text().simplified();
      if (S.contains("#"))
         S.remove(S.indexOf("#"), S.size()+1);
      S = S.simplified();
   }
   return (cur.block().blockNumber());
}
//---------------------------------------------------------------
void nutshelleditor::scriptFold(int section)
{
   QTextCursor cur;
   cur = textCursor();

   int lnbinding = scriptFindSectionBlock("binding");
   int lnareamap = scriptFindSectionBlock("areamap");
   int lntimer   = scriptFindSectionBlock("timer");
   int lninitial = scriptFindSectionBlock("initial");
   int lndynamic = scriptFindSectionBlock("dynamic");

   int lnfrom = 0, lnto = 0;
   bool do_fold = false;

   if (section == 1)
   {
      fold_binding = !fold_binding;
      do_fold = fold_binding;

      lnfrom = lnbinding;
      lnto = lnareamap;
      if (lnto < 0)
         lnto = lntimer;
      if (lnto < 0)
         lnto = lninitial;
      if (lnto < 0)
         lnto = lndynamic;
      if (lnto < 0)
         lnto = document()->lastBlock().blockNumber();

   }
   else
      if (section == 2)
      {
         fold_areamap = !fold_areamap;
         do_fold = fold_areamap;
         lnfrom = lnareamap;
         lnto = lntimer;
         if (lnto < 0)
            lnto = lninitial;
         if (lnto < 0)
            lnto = lndynamic;
         if (lnto < 0)
            lnto = document()->lastBlock().blockNumber();
      }
      else
         if (section == 3)
         {
            fold_timer = !fold_timer;
            do_fold = fold_timer;
            lnfrom = lntimer;
            lnto = lninitial;
            if (lnto < 0)
               lnto = lndynamic;
            if (lnto < 0)
               lnto = document()->lastBlock().blockNumber();
         }
         else
            if (section == 4)
            {
               fold_initial = !fold_initial;
               do_fold = fold_initial;
               lnfrom = lninitial;
               lnto = lndynamic;
               if (lnto < 0)
                  lnto = document()->lastBlock().blockNumber();
            }
            else
               if (section == 5)
               {
                  fold_dynamic = !fold_dynamic;
                  do_fold = fold_dynamic;
                  lnfrom = lndynamic;
                  lnto = document()->lastBlock().blockNumber();
               }


   QTextBlock startBlock = document()->findBlockByNumber(lnfrom+1);
   QTextBlock endBlock = document()->findBlockByNumber(lnto);
   QTextBlock block = startBlock.previous();

   while (startBlock < endBlock)
   {
      if (do_fold)
      {
         startBlock.setVisible(false);
         startBlock.setLineCount(0);
         startBlock = startBlock.next();
      }
      else
      {
         startBlock.setVisible(true);
         startBlock.setLineCount(startBlock.layout()->lineCount());
         startBlock = startBlock.next();
      }
   }

   QString str;
   switch (section){
   case 1: str = "binding"; break;
   case 2: str = "areamap"; break;
   case 3: str = "timer"; break;
   case 4: str = "initial"; break;
   case 5: str = "dynamic"; break;
   }
   if (do_fold)
   {
      cur.setPosition(block.position());
      cur.select(QTextCursor::WordUnderCursor);
      int pos = cur.selectionEnd();
      cur.setPosition(pos);
      cur.insertText(" #["+str+"...]");
      setTextCursor(cur);
   }
   else
   {
      cur = document()->find(" #["+str+"...]");
      cur.removeSelectedText();
      setTextCursor(cur);
   }

   // this is to trick the document in repainting!!!
   QFont f = font();
   f.setBold(true);
   setFont(f);
   f.setBold(false);
   setFont(f);

   document()->setModified(false);
}
//---------------------------------------------------------------------------
