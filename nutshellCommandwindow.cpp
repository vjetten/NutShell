/*
 * NutshellCommandwindow

 * functions to deal with commands given in commandwindow and cmd list
 * Author: VJ 140222,181001
 */


#include "nutshellqt.h"


//---------------------------------------------------------------
void nutshellqt::setupCommandwindow()
{
    PCRProcess = new QProcess(this);
    processError = false;
    PCRProcess->setProcessChannelMode(QProcess::MergedChannels);
    connect(PCRProcess, SIGNAL(readyReadStandardOutput()),this, SLOT(outputCommand()) );
    connect(PCRProcess, SIGNAL(readyReadStandardError()),this, SLOT(readFromStderrPCR()) );
    connect(PCRProcess, SIGNAL(error(QProcess::ProcessError)),this, SLOT(errorCommand()) );
    // process called by command window, typing pcrcalc, or aguila or any other pcr prog

    CMDProcess = new QProcess(this);
    connect(CMDProcess, SIGNAL(readyReadStandardOutput()),this, SLOT(outputCmd()) );

    commandWindow = new nutshelleditor(this, 1);
    commandWindow->document()->setDocumentMargin(2);
    commandWindow->setFrameShape(QFrame::StyledPanel);
    // commandWindow->setFrameStyle(QFrame::Sunken);

    verticalLayoutCmdlist->insertWidget(0, commandWindow);
    commandWindow->installEventFilter(this);
  //  commandWindow->document()->setDefaultFont(QFont("Consolas", 10));
    commandWindow->setFont(QFont("Consolas", 10));
  //  commandWindow->document()->setDefaultFont(QFont("Courier New", 10));
    // declare and set the commandWindow

    connect(toolButton_clearcmd, SIGNAL(clicked()), this, SLOT(clearCommandWindow()));
    connect(toolButton_cmdlist, SIGNAL(clicked()), this, SLOT(clearCommandList()));
    connect(toolButton_savecmd, SIGNAL(clicked()), this, SLOT(saveCommandList()));
    commandcounter = -1;
}
//---------------------------------------------------------------
void nutshellqt::clearCommandWindow()
{
    commandWindow->clear();
    commandcounter = -1;
}
//---------------------------------------------------------------
void nutshellqt::clearCommandList()
{
    comboBox_cmdlist->clear();
    commandcounter = -1;
}
//---------------------------------------------------------------
void nutshellqt::saveCommandList()
{
    setNutshellIni();
    WarningMsg("Commands are stored in nutshell.ini");

}
//---------------------------------------------------------------
void nutshellqt::copyCommandList()
{
    QString cmd = comboBox_cmdlist->currentText();
    if (!cmd.isEmpty())
        commandWindow->appendPlainText(cmd);
}
//---------------------------------------------------------------
void nutshellqt::parseCommand()
{
    QString all;
    all = commandWindow->toPlainText();

    processError = false;

    if (all.isEmpty())
    {
        commandWindow->appendPlainText("\n");
        return;
    }
    QStringList lines;
    QStringList args;
    QString prog;

    lines = all.split("\n");
    args = lines[lines.count()-1].split(" ");
    bool args1 = args.count() > 1;

    if (args[0].isEmpty())
    {
        commandWindow->appendPlainText("");
        return;
    }

    if ((args[0].toUpper() == "PCRCALC")// || args[0].toUpper() == "OLDCALC")
            && (calcProcess && calcProcess->state() == QProcess::Running))
    {
        ErrorMsg("pcrcalc is active, wait until it is finished or press stop first");
        return;
    }

    if (args[0].contains(".cmd", Qt::CaseInsensitive) || QFileInfo(args[0] +".cmd").exists()) {
        prog = args[0] +".cmd";
        deleteBatch();
        createBatch(prog, "");
        args << QString("/C " + MapeditDirName + "_nutshell_batchjob");
        QString hoi = MapeditDirName + "_nutshell_batchjob.cmd";
        QDesktopServices::openUrl(QUrl("file:///"+hoi));
        setCursorLast();
        comboBox_cmdlist->insertItem(0, lines[lines.count()-1]);
        commandcounter = -1;
        return;
    }

    prog = PCRasterAppDirName + args[0] +".exe";
   // qDebug() << args.count() << args << prog;

    if (args[0].contains("gdal", Qt::CaseInsensitive))
        prog = GDALAppDirName + args[0] +".exe";

    if ((args[0].toUpper() == "PCRCALC")
            && (args1 && args[1].indexOf("-f",Qt::CaseInsensitive) == 0))
    {
        args.removeAt(0);
        runModelCommandwindow(prog, args);
    }
    else
        if (args[0].toUpper() == "AGUILA")
        {
            args.removeAt(0);
            PCRProcess->startDetached(prog, args);
            commandWindow->appendPlainText("");
        }
        else
            if (args[0].contains("gdal", Qt::CaseInsensitive))
            {
                args.removeAt(0);
                if(!CondaInstall) {
                    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
                    QString str = GDALDirName+QString("bin;")+ GDALAppDirName+ PCRasterAppDirName;
                    env.insert("PATH",str);
                    str = GDALDirName + QString("bin/gdal-data");
                    env.insert("GDAL_DATA",str);
                    //  env.insert("PATH",GDALDirName);
                    PCRProcess->setProcessEnvironment(env);
                }
                PCRProcess->start(prog, args);
            }
            else
            {
                args.removeAt(0);
                PCRProcess->start(prog, args);
                //     PCRProcess->waitForReadyRead(10000);

                PCRProcess->waitForFinished(-1);

                QStringList S = args[0].split("=");
                if (QFileInfo(S[0]).exists()) {
                    MAP *m = Mopen(S[0].toLatin1().data(),M_WRITE);
                    if (m != nullptr) {
                        MputProjection(m,PT_YDECT2B);
                        qDebug() << S[0] << "changed projection";
                        Mclose(m);
                    }
                }
            }

    setCursorLast();
    //  if (!processError)
    comboBox_cmdlist->insertItem(0, lines[lines.count()-1]);
    commandcounter = -1;
    // add command to commandlist
}
//---------------------------------------------------------------
void nutshellqt::setCursorLast()
{
    QString output = commandWindow->toPlainText();
    QStringList hop = output.split("\n");
    if (hop[hop.count()-1].size() > 1)
        output.append("\n");

    commandWindow->setPlainText(output);
    QTextCursor cur = commandWindow->textCursor();
    cur.setPosition(output.size());
    commandWindow->setTextCursor(cur);
    QCoreApplication::sendPostedEvents(this, 0);
}
//---------------------------------------------------------------
void nutshellqt::errorCommand()
{
    commandWindow->appendPlainText(PCRProcess->errorString());
    QCoreApplication::sendPostedEvents(this, 0);
    processError = true;
}

//---------------------------------------------------------------
void nutshellqt::outputCommand()
{
    QString buffer = QString(PCRProcess->readAllStandardOutput());

    if (!buffer.contains('\r')) {
        bufprev = bufprev + buffer;
        return;
    }
    else {
        bufprev = bufprev + buffer;
        buffer = bufprev;
        bufprev = "";
    }

    commandWindow->appendPlainText(buffer);
    QCoreApplication::sendPostedEvents(this, 0);
}
//---------------------------------------------------------------
void nutshellqt::outputCmd()
{
    QString buffer = QString(CMDProcess->readAllStandardOutput());

    if (!buffer.contains('\r')) {
        bufprev = bufprev + buffer;
        return;
    }
    else {
        bufprev = bufprev + buffer;
        buffer = bufprev;
        bufprev = "";
    }

    commandWindow->appendPlainText(buffer);
    QCoreApplication::sendPostedEvents(this, 0);
}
//---------------------------------------------------------------
void nutshellqt::prevCommand()
{
    QStringList lines;
    QString all;

    if (commandcounter == -1)
        commandcounter = 0;//comboBox_cmdlist->count();
    else
    {
        commandcounter--;
        commandcounter	= max(commandcounter, 0);
    }

    comboBox_cmdlist->setCurrentIndex(commandcounter);

    all = commandWindow->toPlainText();
    lines = all.split("\n");
    lines.replace(lines.count()-1,comboBox_cmdlist->itemText(commandcounter));
    all = lines.join("\n");
    commandWindow->setPlainText(all);

    QTextCursor cur = commandWindow->textCursor();
    cur.setPosition(all.size());
    commandWindow->setTextCursor(cur);
    //put the cursor at the end
}
//---------------------------------------------------------------
void nutshellqt::nextCommand()
{
    QStringList lines;
    QString all;

    if (commandcounter == -1)
        commandcounter = 0;
    else
    {
        commandcounter++;
        commandcounter = min(commandcounter, comboBox_cmdlist->count()-1);
    }

    comboBox_cmdlist->setCurrentIndex(commandcounter);

    all = commandWindow->toPlainText();
    lines = all.split("\n");
    lines.replace(lines.count()-1,comboBox_cmdlist->itemText(commandcounter));
    all = lines.join("\n");
    commandWindow->setPlainText(all);

    QTextCursor cur = commandWindow->textCursor();
    cur.setPosition(all.size());
    commandWindow->setTextCursor(cur);
}
