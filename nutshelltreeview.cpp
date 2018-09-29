/*
 * NutShelltreeview,

 * based on Qtreeview but capture dragevent
 * Author: VJ 140222
 */


#include "nutshellqt.h"

/*!
 * \brief myTreeView captures dropevent to copy/move files, check for existance,
 *  drop map series
 */
//---------------------------------------------------------------
myTreeView::myTreeView(QTreeView *parent)
   : QTreeView(parent)
{
}
//---------------------------------------------------------------
//! strip number from basename in map series
QString myTreeView::StripForName(QString S)
{
   QString Ss;
   Ss = QFileInfo(S).baseName();

   int i = Ss.length()-1;
   while (i > 0 && int(Ss.toLatin1()[i]) >= 48 && int(Ss.toLatin1()[i]) <= 57)
      i--;
   Ss = Ss.remove(i+1, 256);

   return(Ss);
}
//---------------------------------------------------------------
/*!
 * \brief myTreeView::dropEvent Reimplementation of the QDropEvent for file handling
 * \param event
 */
void myTreeView::dropEvent(QDropEvent *event)
{
   QModelIndex index = indexAt(event->pos());
   // the dir moved into

   QFileSystemModel *m = static_cast<QFileSystemModel*>(model());
   // cast the linked model to myTreeView as a filesystem model

   QDir dir(m->filePath(index));

   if (event->keyboardModifiers() == Qt::ControlModifier)
      event->setDropAction(Qt::CopyAction);
   else
      event->setDropAction(Qt::MoveAction);


   foreach(QUrl url, event->mimeData()->urls())
   {
      QFileInfo fi = QFileInfo(url.toLocalFile());
      // the filepath that is moved
      bool isSeries = false;
      int thisnr;

      QString bn = StripForName(fi.fileName());
      for (int i = 0; i < fns.count(); i++)
         if (bn.compare(fns[i].base, Qt::CaseInsensitive) == 0)
         {
            thisnr = i;
            isSeries = true;
            break;
         }
      // copy/move a series. Only 0.001 is shown so only that file is in
      // the mimedata. Dropevent is called for all files in the series
      // but mimedata isn't there so must be copied/moved directly
      if (isSeries)
      {
         QString sss;
         if (event->dropAction() == (Qt::CopyAction))
            sss ="Copying mapseries: <i><font color=\"blue\">" + bn + "</font></i>";
         else
            sss = "Moving mapseries: <i><font color=\"blue\">" + bn + "</font></i>";

         QProgressDialog *bar = new QProgressDialog(sss, "", 1, fns[thisnr].series.count());
         bar->setCancelButton(0);
         int k = 0;
         foreach(QString str, fns[thisnr].series)
         {
            QString filePath = m->filePath(index) + "/" + QFileInfo(str).fileName();

            // make the drop filepath to check if it exists
            QString newFilePath = filePath;
            while(QFileInfo(newFilePath).exists())
               newFilePath = dir.absolutePath() + "/" + QFileInfo(newFilePath).baseName() + "_copy." + QFileInfo(newFilePath).suffix();
            // make rename filepath and add "_copy" is necessary

            if (QFileInfo(filePath).exists())   // if the file exists in the new directory
            {
               dir.rename(filePath,filePath+"tmpqt");
               // rename the old file to a temp name
//               if (first)
//               {
//                  QTreeView::dropEvent(event);
//                  first = false;
//               }
//               else
//               {
                  QFile(str).copy(filePath);
                  if (event->dropAction() == (Qt::MoveAction))
                     QFile(str).remove();
             //  }

               // continue to execute the drop event
               dir.rename(filePath,newFilePath);
               // rename the new file to the one with _copy(s) in it
               dir.rename(filePath+"tmpqt",filePath);
               // rename the temp name back to the original
            }
            else
            {
//               if (first)
//               {
//                  QTreeView::dropEvent(event);
//                  first = false;
//               }
//               else
//               {
                  QFile(str).copy(filePath);
                  if (event->dropAction() == (Qt::MoveAction))
                     QFile(str).remove();
             //  }
            }
            bar->setValue(k++);
         }
         bar->close();
      }
      else
      {
         QString filePath = m->filePath(index) + "/" + fi.fileName();
         // make the drop filepath to check if it exists

         QString newFilePath = filePath;
         while(QFileInfo(newFilePath).exists())
            newFilePath = dir.absolutePath() + "/" + QFileInfo(newFilePath).baseName() + "_copy." + fi.suffix();
         // make rename filepath and add "_copy" is necessary

         if (QFileInfo(filePath).exists())   // if the file exists in the new directory
         {
            dir.rename(filePath,filePath+"tmpqt");
            // rename the old file to a temp name

            QTreeView::dropEvent(event);
            // continue to execute the drop event

            dir.rename(filePath,newFilePath);
            // rename the new file to the one with _copy(s) in it
            dir.rename(filePath+"tmpqt",filePath);
            // rename the temp name back to the original
         }
         else
            QTreeView::dropEvent(event);
      }
   }
}

