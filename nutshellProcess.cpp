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
  //  connect(calcProcess, SIGNAL(readyReadStandardOutput()),this, SLOT(readFromStdOutput()) );
    connect(calcProcess, SIGNAL(finished(int)),this, SLOT(finishedModel(int)) );

    //    useOldCalc = false;

    //    connect(toolButton_oldcalc, SIGNAL(toggled(bool)), this, SLOT(toggleOldcalc(bool)));
    connect(toolButton_startrun, SIGNAL(clicked()), this, SLOT(runModel()));
    connect(toolButton_stoprun, SIGNAL(clicked()), this, SLOT(killModel()));
    connect(toolButton_pauserun, SIGNAL(toggled(bool)), this, SLOT(suspendModel(bool)));

    toolButton_startrun->setIconSize(iSize);
    toolButton_stoprun->setIconSize(iSize);
    toolButton_pauserun->setIconSize(iSize);

    setButtons(false, false, true);

    runPaused = false;

}
//---------------------------------------------------------------
// called from command window, only if option -f
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
        ErrorMsg("Another pcrcalc process is active, wait until it is finished or press stop first");
        return;
    }

    totalsteps = getTimesteps();
    time_ms.start();
    statusBar()->addWidget(&statusLabel);
    processDone = false;

    commandWindow->appendPlainText("Starting ... ");
    commandWindow->appendPlainText(" ");
    commandWindow->appendPlainText(" ");
    // make room in the plainTextEdit for the pcrcalc counter

    calcProcess->start(prog, args, QIODevice::ReadWrite);

    runPaused = false;

}
//---------------------------------------------------------------
// called from pressing start speed buttons
void nutshellqt::runModel()
{
    QStringList args, argsscreen;
    QString prog;

    ETEditor->clearerror();
    statusBar()->clearMessage();
    setButtons(true, false, false);

    QString ext = QFileInfo(ETfilePath).suffix();
    qDebug() << ETfilePath << ext;
    if (ext.toUpper() == "BAT" || ext.toUpper() == "CMD")
    {
        deleteBatch();
        QString name =  ETfilePath;
        QString ar = "";
        if (toolButton_argSubs->isChecked())
            ar = lineEdit_argsubst->text();
        createBatch(ETfilePath, ar);
        args << QString("/C " + MapeditDirName + "_nutshell_batchjob");
        QString hoi = MapeditDirName + "_nutshell_batchjob.cmd";
QDesktopServices::openUrl(QUrl("file:///"+hoi));
  //      CMDProcess->startDetached("cmd.exe",args,QIODevice::ReadWrite);
//        CMDProcess->setProgram( "cmd.exe" );
//        CMDProcess->setArguments( args );
//        CMDProcess->setStandardOutputFile( QProcess::nullDevice() );
//        CMDProcess->setStandardErrorFile( QProcess::nullDevice() );
//        CMDProcess->startDetached();

        setButtons(false, false, true);

        return;
    }
    // run a batch file by passing it on to the system

    if (!runPaused && calcProcess && calcProcess->state() == QProcess::Running)
    {
        ErrorMsg("Another pcrcalc process is active, wait until it is finished or press stop first");
        setButtons(false, false, true);
        return;
    }
    if (runPaused)
    {
        suspendModel(false);
        setButtons(true, false, false);
        return;
    }
    // check if something is running

//    totalsteps = getTimesteps();
//    time_ms.start();
//    statusBar()->addWidget(&statusLabel);

//    setCursorLast();

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
    // check the script stuff

    totalsteps = getTimesteps();
    time_ms.start();
    // get timer info
    statusBar()->addWidget(&statusLabel);
    setCursorLast();

    args << QString("-1");
    args  << QString("-f") <<  ETfilePath;
    argsscreen << QString("-1 -f") <<  ETfileName;
    if (toolButton_argSubs->isChecked())
    {
        QStringList subs;
        subs = lineEdit_argsubst->text().split(" ",QString::SkipEmptyParts);
        args << subs;
        argsscreen << subs;
    }
    // make argument list

    prog = PCRasterAppDirName + "pcrcalc.exe";
    commandWindow->appendPlainText("pcrcalc "+argsscreen.join(" "));

    if (!QFileInfo(prog).exists())
    {
        ErrorMsg("pcrcalc not found. Specify directory in File->Options.");
        return;
    }
    //check is pcrcalc can be found

    processDone = false;

    commandWindow->appendPlainText("starting ... ");
    commandWindow->appendPlainText(" ");
    commandWindow->appendPlainText(" ");
    // make room in the plainTextEdit for the pcrcalc counter
    //QCoreApplication::sendPostedEvents(this, 0);

    calcProcess->start(prog, args, QIODevice::ReadWrite);

    setCursorLast();
}
//---------------------------------------------------------------
//pause process only for windows systems
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
        calcProcess->kill();

        calcProcess->waitForFinished();

        QStringList list = commandWindow->toPlainText().split("\n");
        xlast = list.count()-1;
        while (xlast > 0  && (list[xlast] == " " || list[xlast].isEmpty()))
        {
            list.removeLast();
            xlast--;
        }
        if (!list[xlast].contains("pcrcalc") && !list[xlast].contains("ERROR"))
            list[xlast] = "user interupt...\n";
        else
            if(!list[xlast].contains("ERROR"))
            list << "user interupt...\n";
        QString output = list.join("\n");
        commandWindow->setPlainText(output);

        QTextCursor cur = commandWindow->textCursor();
        cur.setPosition(output.size());
        commandWindow->setTextCursor(cur);

        setButtons(false, false, true);
        processDone = true;
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
    bool done = false;
    //    QList<QByteArray> lines = byteArray.split('\r\n');
    //    foreach (const QByteArray& line, lines) {
    //        if (line.size())
    //            qDebug() << "PIPE STDERR" << line;
    //    }
    listb.clear();
    listb = buffer.split("\r");
    // split process output all out in seperate lines, using UNIX CR

    output = commandWindow->toPlainText();
    list = output.split("\n");
    xlast = list.count();
    // get the lines in the commandWindow
//qDebug() << list;
    // first output at start run
    if (buffer.contains("ERROR") ) {
        if (output.isEmpty()) {
            qDebug() << "hier";
        } else {
            qDebug() << xlast;
            list.replace(xlast-3,listb[0]);  // pcrcalc version
            list.replace(xlast-2,listb[1]);  // error message
            output=list.join("\n");
        }
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
            for (int j = 0; j < listb.count(); j++)//j+=2) //<= each runstep is repeated twice for some reason
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
                        double timeout = timemin * totalsteps/SStep.toInt();
                        SStep = QString("Executing timestep %1 (runtime: %2 of %3 min)")
                                .arg(SStep).arg(timemin,0,'f',2).arg(timeout,0,'f',2);
                        done = timemin*1.001 >= timeout;
                        // qDebug() << timemin*1.000001 << timeout << done;
                        // calculate runtime and show
                    }

                    commandWindow->textCursor().removeSelectedText();
                    commandWindow->textCursor().insertText(SStep);
                    // delete run step number text and insert new run number
                    if (done)
                        commandWindow->appendPlainText("Wrapping up output...");
                    calcCursor.setPosition(cursorPosition);
                    //set cursor back to preferred position
                    processDone = true;
                }
        }

    QCoreApplication::sendPostedEvents(this, 0);
    // update the plaintextedit with these actions
}
//---------------------------------------------------------------
void nutshellqt::readFromStderr()
{
    QString buffer = QString(calcProcess->readAllStandardError());

    if (!buffer.contains('\r')) {
        bufprev = bufprev + buffer;
        return;
    }
    else {
        bufprev = bufprev + buffer;
        buffer = bufprev;
        bufprev = "";
    }

// pcraster 4.2.0 with pcrcalc 2018 has different output: first the first chr then the rest?!

    qDebug() << buffer << bufprev;
    onScreen(buffer);

    if (buffer.contains("ERROR"))
        doRunErrorMessage(buffer);


    //    QByteArray byteArray = calcProcess->readAllStandardError();
    //    onScreen(byteArray);
    //    QString buffer = QString(byteArray);
    //    if (buffer.contains("ERROR"))
    //        doRunErrorMessage(buffer);

    //    QList<QByteArray> lines = byteArray.split('\r\n');
    //    foreach (const QByteArray& line, lines) {
    //        if (line.size())
    //            qDebug() << "PIPE STDERR" << line;
    //    }
    //    qDebug() << calcProcess->readAllStandardError().constData();
    //https://qt.gitorious.org/qtplayground/qtprocessmanager/commit/b3c127ab0d29f9d245a9639c216caedf4a41ca69
}//---------------------------------------------------------------
void nutshellqt::readFromStderrPCR()
{
    QString buffer = QString(PCRProcess->readAllStandardError());

    if (!buffer.contains('\r')) {
        bufprev = bufprev + buffer;
        return;
    }
    else {
        bufprev = bufprev + buffer;
        buffer = bufprev;
        bufprev = "";
    }

// pcraster 4.2.0 with pcrcalc 2018 has different output: first the first chr then the rest?!

    //qDebug() << buffer << bufprev;
    onScreen(buffer);

    if (buffer.contains("ERROR"))
        doRunErrorMessage(buffer);


    //    QByteArray byteArray = calcProcess->readAllStandardError();
    //    onScreen(byteArray);
    //    QString buffer = QString(byteArray);
    //    if (buffer.contains("ERROR"))
    //        doRunErrorMessage(buffer);

    //    QList<QByteArray> lines = byteArray.split('\r\n');
    //    foreach (const QByteArray& line, lines) {
    //        if (line.size())
    //            qDebug() << "PIPE STDERR" << line;
    //    }
    //    qDebug() << calcProcess->readAllStandardError().constData();
    //https://qt.gitorious.org/qtplayground/qtprocessmanager/commit/b3c127ab0d29f9d245a9639c216caedf4a41ca69
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
void nutshellqt::finishedModel(int)
{
    if (calcProcess->bytesAvailable() > 0)
    {
        QByteArray buf;
        buf.clear();
        buf = calcProcess->readAllStandardError();
        qDebug() << "buf" << buf;
        onScreen(QString(buf));
    }

    QStringList list = commandWindow->toPlainText().split("\n");
    xlast = list.count()-1;
    while (xlast > 0  && (list[xlast] == " " || list[xlast].isEmpty()))
    {
        list.removeLast();
        xlast--;
    }
   // qDebug() << list[xlast];
    if (!list[xlast].contains("pcrcalc") && !list[xlast].contains("ERROR"))
        list[xlast] = "Done.";
    else
        if(!list[xlast].contains("ERROR"))
        list << "Done.";
    QString output = list.join("\n");

    commandWindow->setPlainText(output);

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
//void nutshellqt::toggleOldcalc(bool checked)
//{
//    useOldCalc = checked;
//}
//---------------------------------------------------------------

