/*
 * NutshellProcess
 * v 1.1
 * functions to run script with pcrcalc/oldcalc and QPRocess
 * Note: pause/resume process is only implemented for windows
 * author: VJ 120226
 */


#include "nutshellqt.h"

#ifdef Q_OS_WIN32
#include <windows.h>
#endif


void nutshellqt::setButtons(bool one, bool two, bool three)
{
   toolButton_startrun->setChecked(one);
   toolButton_pauserun->setChecked(two);
   toolButton_stoprun->setChecked(three);

   runmodelAct->setChecked(one);
   pausemodelAct->setChecked(two);
   killmodelAct->setChecked(three);
}

//---------------------------------------------------------------
void nutshellqt::setupModel()
{
   calcProcess = new QProcess(this);
   calcProcess->setReadChannel ( QProcess::StandardError );
   // pcrcalc outputs on the error channel
   //calcProcess->setTextModeEnabled (true);
   connect(calcProcess, SIGNAL(readyReadStandardError()),this, SLOT(readFromStderr()) );
   connect(calcProcess, SIGNAL(finished(int)),this, SLOT(finishedModel(int)) );
   //connect(PCRProcess, SIGNAL(readyReadStandardError()),this, SLOT(readFromStderr()) );
   useOldCalc = false;

   connect(toolButton_oldcalc, SIGNAL(toggled(bool)), this, SLOT(toggleOldcalc(bool)));
   connect(toolButton_startrun, SIGNAL(clicked()), this, SLOT(runModel()));
   connect(toolButton_stoprun, SIGNAL(clicked()), this, SLOT(killModel()));
   connect(toolButton_pauserun, SIGNAL(toggled(bool)), this, SLOT(suspendModel(bool)));

   setButtons(false, false, true);

   runPaused = false;

}
//---------------------------------------------------------------
void nutshellqt::runModelCommandwindow(QString prog, QStringList args)
{
   ETEditor->clearerror();
   statusBar()->clearMessage();

   if (!QFileInfo(prog).exists())
   {
      ErrorMsg("pcrcalc not found. Specify directory in File->Options.");
      return;
   }

   if (!runPaused && calcProcess && calcProcess->state() == QProcess::Running)
   {
      ErrorMsg("pcrcalc is active, wait until it is finished or press stop first");
      return;
   }

   totalsteps = getTimesteps();
   time_ms.start();
   statusBar()->addWidget(&statusLabel);

   commandWindow->appendPlainText("starting... ");
   commandWindow->appendPlainText(" ");
   commandWindow->appendPlainText(" ");
   // make room in the plainTextEdit for the pcrcalc counter

   QString ext = QFileInfo(prog).suffix();
   if (ext.toUpper() == "BAT" || ext.toUpper() == "CMD")
   {
      STATUS("Opening file in operating system");
      QDesktopServices::openUrl(QUrl("\""+prog+"\""));
      return;
   }
   // run a batch file by passing it on to the system

   calcProcess->start(prog, args, QIODevice::ReadWrite);

   runPaused = false;

}
//---------------------------------------------------------------
void nutshellqt::runModel()
{
   QStringList args, argsscreen;
   QString prog;

   ETEditor->clearerror();
   statusBar()->clearMessage();

   if (!runPaused && calcProcess && calcProcess->state() == QProcess::Running)
   {
      ErrorMsg("pcrcalc is active, wait until it is finished or press stop first");
      return;
   }
   if (runPaused)
   {
      suspendModel(false);
      return;
   }

   setButtons(true, false, false);

   totalsteps = getTimesteps();
   time_ms.start();
   statusBar()->addWidget(&statusLabel);

   setCursorLast();

   if (!ETExists)
   {
      setButtons(false, false, true);
      ErrorMsg("No model\\script to run");
      return;
   }
   if (ETEditor->document()->isEmpty())
   {
      setButtons(false, false, true);
      ErrorMsg("The script is empty");
      return;
   }
   if (ETEditor->document()->isModified())
   {
      setButtons(false, false, true);
      ErrorMsg("The model\\script is modified, save first");
      return;
   }

   QString ext = QFileInfo(ETfilePath).suffix();
   if (ext.toUpper() == "BAT" || ext.toUpper() == "CMD")
   {
      STATUS("Opening file in operating system");
      QDesktopServices::openUrl(QUrl("\""+ETfilePath+"\""));
      return;
   }
   // run a batch file by passing it on to the system

   args << QString("-f") <<  ETfilePath;
   argsscreen << QString("-f") <<  ETfileName;

   if (toolButton_argSubs->isChecked())
   {
      QStringList subs;
      subs = lineEdit_argsubst->text().split(" ",QString::SkipEmptyParts);
      args << subs;
      argsscreen << subs;
   }

   if (useOldCalc)
   {
      prog = PCRasterAppDirName + "oldcalc.exe";
      commandWindow->appendPlainText("oldcalc "+argsscreen.join(" "));
   }
   else
   {
      prog = PCRasterAppDirName + "pcrcalc.exe";
      commandWindow->appendPlainText("pcrcalc "+argsscreen.join(" "));
   }

   if (!QFileInfo(prog).exists())
   {
      ErrorMsg("pcrcalc not found. Specify directory in File->Options.");
      return;
   }

   commandWindow->appendPlainText("starting... ");
   commandWindow->appendPlainText("");
   commandWindow->appendPlainText("");
   // make room in the plainTextEdit for the pcrcalc counter
   //QCoreApplication::sendPostedEvents(this, 0);

   calcProcess->start(prog, args, QIODevice::ReadWrite);

   setCursorLast();
}
//---------------------------------------------------------------
void nutshellqt::suspendModel(bool pause)
{
   if (calcProcess && calcProcess->state() == QProcess::NotRunning)
   {
      if (pause)
         ErrorMsg("pcrcalc is not active, nothing to pauze");
      pause = false;
      setButtons(false, false, true);
      return;
   }

   setButtons(!pause, pause, false);


#ifdef Q_OS_WIN32
   // only windows uses PROCESS_INFORMATION in this way, on UNIX systems pid
   // returns an integer of the proces number
   // needs to be implemented
   PROCESS_INFORMATION *pi = calcProcess->pid();

   if (pause)
   {
      SuspendThread(pi->hThread);
      runPaused = true;
   }
   else
   {
      ResumeThread(pi->hThread);
      runPaused = false;
   }
#else
   ErrorMsg("Only implemented on Windows systems");
#endif
}
//---------------------------------------------------------------
void nutshellqt::killModel()
{
   if (calcProcess && calcProcess->state() == QProcess::Running)
   {
      QString output = commandWindow->toPlainText();

      calcProcess->kill();

      calcProcess->waitForFinished();

      output.append("\nuser interupt...\n");
      commandWindow->setPlainText(output);

      QTextCursor cur = commandWindow->textCursor();
      cur.setPosition(output.size());
      commandWindow->setTextCursor(cur);

      setButtons(false, false, true);

   }
   else
   {
      //ErrorMsg("pcrcalc is not active, nothing to stop");
      toolButton_stoprun->setChecked(true);
   }

}
//---------------------------------------------------------------
void nutshellqt::onScreen(QString buffer)
{
   QString output, SStep;
   QStringList list, listb;

   listb.clear();
   listb = buffer.split("\r");
   // split process output all out in seperate lines, using UNIX CR

   output = commandWindow->toPlainText();
   list = output.split("\n");
   xlast = list.count();
   // get the lines in the commandWindow

   // first output at start run
   if (buffer.contains("ERROR") )
   {
      list.replace(xlast-3,listb[0]);  // pcrcalc version
      list.replace(xlast-2,listb[1]);  // error message
      output=list.join("\n");
      commandWindow->setPlainText(output);
      // join new lines and replace the commandWindow
   }
   else
      if(listb[0].contains("version"))
      {
         int last = listb.count() - 1;

         list.replace(xlast-3,listb[0]);
         list.replace(xlast-2,listb[last]);
         output=list.join("\n");
         commandWindow->setPlainText(output);
         // join new lines and replace the commandWindow

         calcCursor = commandWindow->textCursor();
         calcCursor.setPosition(commandWindow->toPlainText().size() - listb[last].size() - 1);
         cursorPosition = calcCursor.position();
         //save output cursor position
         commandWindow->setTextCursor(calcCursor);
      }
      else
      {
         // runstep output
         for (int j = 0; j < listb.count(); j+=2) //<= each runstep is repeated twice for some reason
            if (listb[j].contains("Exec"))
            {
               //Delay(10);

               calcCursor.setPosition(commandWindow->toPlainText().size(),QTextCursor::KeepAnchor);
               commandWindow->setTextCursor(calcCursor);
               //set cursor at the end
               SStep = listb[j];
               if (totalsteps > 0)
               {
                  SStep = listb[j].remove(0, 19);
                  double timemin = time_ms.elapsed()*(0.001/60);
                  SStep = QString("Executing timestep %1 (runtime: %2 of %3 min)")
                        .arg(SStep)
                        .arg(timemin,0,'f',2)
                        .arg(timemin * totalsteps/SStep.toInt(),0,'f',2);
                  // calculate runtime and show
               }

               commandWindow->textCursor().removeSelectedText();
               commandWindow->textCursor().insertText(SStep);
               // delete run step number text and insert new run number

               calcCursor.setPosition(cursorPosition);
               //set cursor back to preferred position
            }
      }

   //    if (totalsteps > 0)
   //    {
   //        int steps = SStep.toInt();
   //        double timemin = time_ms.elapsed()*(0.001/60);
   //        SStep = QString(" Estimated runtime : %1 of %2 minutes").arg(timemin,0,'g',2).arg(timemin * totalsteps/steps,0,'g',2);
   //        statusLabel.setText(SStep);
   //        statusLabel.show();
   //        // calculate runtime and show
   //    }

   QCoreApplication::sendPostedEvents(this, 0);
   // update the plaintextedit with these actions
}
//---------------------------------------------------------------
void nutshellqt::readFromStderr()
{
   QString buffer = QString(calcProcess->readAllStandardError());
   onScreen(buffer);

   if (buffer.contains("ERROR"))
      doRunErrorMessage(buffer);
}

//---------------------------------------------------------------
void nutshellqt::doRunErrorMessage(QString buffer)
{
   QString hoi;
   QStringList list;
   errorpos[0] = 0;
   errorpos[1] = 0;

   list = buffer.split(":");
   int i = 0;
   foreach(QString str, list) {
      bool ok;
      int num = str.toInt(&ok, 10);
      if (ok) {
         errorpos[i]=num;
         i++;
      }
   }

   ETEditor->doerror = true;
   if (errorpos[0] == 0)
       return;
   // in case a typing error

   ETEditor->errorline = errorpos[0];

   hoi= ETPlainText;
   list = hoi.split("\n");
   int curpos = 0;
   for(int i = 0; i < errorpos[0]-1; i++)
      curpos += list[i].size()+1;
   curpos += errorpos[1]-1;

   QTextCursor cur = ETEditor->textCursor();
   cur.setPosition(curpos);
   ETEditor->setTextCursor(cur);

   ETEditor->repaint();
   statusBar()->showMessage(QString("Error in line %1 pos %2, or before.").arg(errorpos[0]).arg(errorpos[1]));

   setButtons(false, false, true);

}
//---------------------------------------------------------------
void nutshellqt::finishedModel(int c)
{

   if (calcProcess->bytesAvailable() > 0)
   {
      QByteArray buf;
      buf.clear();
      buf = calcProcess->readAllStandardError();
      onScreen(QString(buf));
   }

   setCursorLast();

   setButtons(false, false, true);


   changeFileFilter(_filternr);
   // equivalent to F5

   statusBar()->removeWidget(&statusLabel);

   commandWindow->setFocus();

   //    QCoreApplication::sendPostedEvents(this, 0);
   if(ETEditor->doReport)
   {
      getScriptLinks();
      getScriptLinks();
   }
}
//---------------------------------------------------------------
void nutshellqt::toggleOldcalc(bool checked)
{
   useOldCalc = checked;
}
//---------------------------------------------------------------

