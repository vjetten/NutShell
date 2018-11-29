/*
 * NutShellEvent,

 * separate file with event catching at application level
 * Author: VJ 140222,181001
 */


#include "nutshellqt.h"

//---------------------------------------------------------------
/*!
 * \brief nutshellqt::eventFilter Catches events at application level for general tasks and
 * PCRaster action (aguila etc)
 * \param obj
 * \param event
 * \return return true if event stops here, or false when it continues withd efault event handling
 */
bool nutshellqt::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == treeView)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

            if (keyEvent->key() == Qt::Key_Delete)
            {
                return(deleteDirectory());
            }
        }
    }
    if (obj == fileView)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

            if (keyEvent->key() == Qt::Key_Return)
            {
                if (!changeName)
                    PerformAction(GetActionType());
                changeName = false;
                return true;
            }

            if (keyEvent->key() == Qt::Key_Delete)
            {
                deleteFiles();
                return true;
            }

            if (keyEvent->key() == Qt::Key_F5)
            {
                changeFileFilter(_filternr);
                QCoreApplication::sendPostedEvents(this, 0);
              //  setRootIndex(dirModel->index(currentPath)); why???
                return true;
            }
            if (keyEvent->key() == Qt::Key_F2)
            {
                changeName = true;
                return false;
            }
        }
    }
    if (obj == commandWindow)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

            if (calcProcess && calcProcess->state() == QProcess::Running)
                return true;

            if (keyEvent->key() == Qt::Key_Return)
            {
                parseCommand();
                commandcounter = -1;
                return true;
            }
            if (keyEvent->key() == Qt::Key_Up)
            {
                prevCommand();
                return true;
            }
            if (keyEvent->key() == Qt::Key_Down)
            {
                nextCommand();
                return true;
            }
            if (keyEvent->key() == Qt::Key_Backspace)
            {
                int bsize = commandWindow->document()->lastBlock().text().count();
                if (bsize == 0)
                    return true;
            }
            if (keyEvent->key() == Qt::Key_Escape)
            {
                //delete last line
                commandWindow->moveCursor( QTextCursor::End, QTextCursor::MoveAnchor );
                commandWindow->moveCursor( QTextCursor::StartOfLine, QTextCursor::MoveAnchor );
                commandWindow->moveCursor( QTextCursor::End, QTextCursor::KeepAnchor );
                commandWindow->textCursor().removeSelectedText();
                return true;
            }
        }
    }
    if (obj == comboBox_cmdlist->lineEdit())
    {
        if (event->type() == QEvent::MouseButtonDblClick)
        {
            copyCommandList();
            return true;
        }
    }

    return QMainWindow::eventFilter(obj, event);
}

//---------------------------------------------------------------
void nutshellqt::resizeEvent(QResizeEvent *event)
{
    qDebug() << "hoi";
    findDPIscale();
    return QMainWindow::resizeEvent(event);
}
