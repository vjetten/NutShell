/*
 * NutshellCommandwindow

 * functions to deal with commands given in commandwindow and cmd list
 * Author: VJ 140222,181001
 */


#include "nutshellqt.h"

void setWaitingCursor() {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    qApp->processEvents(); // Ensure the cursor change takes effect immediately
}

void restoreCursor() {
    QApplication::restoreOverrideCursor();
    qApp->processEvents(); // Ensure the cursor change takes effect immediately
}
//---------------------------------------------------------------
void nutshellqt::setupCommandwindow()
{
    PCRProcess = new QProcess(this);
    //processError = false;
    PCRProcess->setProcessChannelMode(QProcess::MergedChannels);
    connect(PCRProcess, SIGNAL(readyReadStandardOutput()),this, SLOT(outputCommand()) );
    connect(PCRProcess, SIGNAL(readyReadStandardError()),this, SLOT(readFromStderrPCR()) );
    connect(PCRProcess, SIGNAL(error(QProcess::ProcessError)),this, SLOT(errorCommand()) );
    connect(PCRProcess, &QProcess::started,setWaitingCursor);
    connect(PCRProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), restoreCursor);

    CMDProcess = new QProcess(this);
    CMDProcess->setProcessChannelMode(QProcess::MergedChannels);
    connect(CMDProcess, SIGNAL(readyReadStandardOutput()),this, SLOT(outputCommand()) );
    connect(CMDProcess, SIGNAL(readyReadStandardError()),this, SLOT(readFromStderrPCR()) );
    connect(CMDProcess, SIGNAL(error(QProcess::UnknownError)),this, SLOT(errorCommand()) );

    commandWindow = new nutshelleditor(this, 1);
    commandWindow->document()->setDocumentMargin(2);
    commandWindow->setFrameShape(QFrame::StyledPanel);
    // commandWindow->setFrameStyle(QFrame::Sunken);

    verticalLayoutCmdlist->insertWidget(0, commandWindow);
    commandWindow->installEventFilter(this);
    commandWindow->setFont(QFont("Consolas", 10));
    // declare and set the commandWindow

    toolButton_savecmd->setVisible(false);
    connect(toolButton_clearcmd, SIGNAL(clicked()), this, SLOT(clearCommandWindow()));
    connect(toolButton_cmdlist, SIGNAL(clicked()), this, SLOT(clearCommandList()));
   // connect(toolButton_savecmd, SIGNAL(clicked()), this, SLOT(saveCommandList()));
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
    WarningMsg("Commands are stored in [USERNAME]/apps/local/nutshell/nutshell.ini");

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
    QString all = commandWindow->toPlainText();

    //processError = false;

    if (all.isEmpty())
    {
        commandWindow->appendPlainText("\n");
        return;
    }
    QStringList lines;
    QStringList args;
    QString prog;

    lines = all.split("\n");
    args = lines[lines.size()-1].split(" ");

    if (args[0].isEmpty())
    {
        commandWindow->appendPlainText("");
        return;
    }

    if ((args[0].toUpper() == "PCRCALC")
            && (calcProcess && calcProcess->state() == QProcess::Running))
    {
        ErrorMsg("pcrcalc is active, wait until it is finished or press stop first");
        return;
    }
    if (PCRProcess && PCRProcess->state() == QProcess::Running)
    {
        ErrorMsg("A process is active, wait until it is finished or press stop first");
        return;
    }
    if (CMDProcess && CMDProcess->state() == QProcess::Running)
    {
        ErrorMsg("A process is active, wait until it is finished or press stop first");
        return;
    }

    setWorkdirectory();

    executeCommand(args);

    setCursorLast();

    comboBox_cmdlist->insertItem(0, lines[lines.size()-1]);
    commandcounter = -1;
    // add command to commandlist
}
//---------------------------------------------------------------

void nutshellqt::executeCommand(QStringList args)
{
    QStringList newPath;
    newPath << CondaDirName
            << CondaDirName + "/Library/bin"
            << CondaDirName + "/DLLs"
            << CondaDirName + "/Scripts";

    QString mergedPath = newPath.join(";") + ";" +
            QProcessEnvironment::systemEnvironment().value("PATH");

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PATH", mergedPath);
    env.insert("GDAL_DATA", CondaDirName + "/Library/share/gdal");

    // Set the command to run

    // run pcrcalc with -f: because of timer treat differently
    if ((args[0].toUpper() == "PCRCALC")
            && (args.size() > 1 && args[1].indexOf("-f",Qt::CaseInsensitive) == 0))
    {
        args.removeAt(0);
        QString prog = CondaDirName+"Library/bin/pcrcalc.exe";
        runModelCommandwindow(prog, args);
        return;
    }

    QString prog;
    bool isCMD = false;
    bool isAguila = false;
    bool moreArgs = args.size() > 1;

    if (!args[0].contains(".")) {
        QFile fff(args[0]+".cmd");
        if (fff.exists())
            args[0] += ".cmd";
    }
    // add .cmd if it exists

    if (args[0].toUpper() == "PYTHON" && moreArgs && args[1].toUpper().contains(".PY")) {
        prog = CondaDirName+args[0]+".exe";
    } else
        if (args[0].toUpper() == "AGUILA") {
            prog = CondaDirName+"Library/bin/"+args[0]+".exe";
            isAguila = true;
        } else
            if (args[0].toUpper().contains(".CMD") || args[0].toUpper().contains(".BAT")) {
                createBatch(args[0],"");
                prog = "cmd.exe";
                args.clear();
                QString batchFilePath = NutshellDirName + "_nutshell_batchjob.cmd";
                args << "xxx" << "/c" << batchFilePath;
                isCMD = true;
                QDesktopServices::openUrl(QUrl("file:///"+batchFilePath));
                return;
            } else
                if (args[0].toUpper().contains("MAPEDIT")) {
                    prog = qApp->applicationDirPath()+"/" + args[0]+".exe";
                    isCMD = true;
                }
                else
                    prog = CondaDirName+"Library/bin/"+args[0]+".exe";


    // Set the command arguments if needed
    args.removeAt(0);

    // difference between CMD Process and PCR process is only change of cursor
    //qDebug() << prog << args;
    if (isAguila) {
        QProcess *AQProcess = new QProcess(this);
        AQProcess->setProcessChannelMode(QProcess::MergedChannels);
        connect(AQProcess, &QProcess::finished, AQProcess, &QObject::deleteLater);
        connect(AQProcess, &QProcess::errorOccurred,
                this, [this, prog, args](QProcess::ProcessError) {
            QString ss = "Aguila cannot display this file.";
            bool isTIFF= isTiffFile(args[1]);
            if (isTIFF)
                ss = "Aguila can not open this file. If it is tif file it could be that the cellsize in x and y direction are not exctly the same. You can correct this with gdalwarp.";
            QMessageBox::critical(this, "Program Error Output", ss);
        });

        AQProcess->setProcessEnvironment(env);
        AQProcess->setWorkingDirectory(currentPath);
       // CMDProcess->startDetached(prog, args);
        AQProcess->start(prog, args);
    } else
        if (isCMD) {
qDebug() << prog << args;
            CMDProcess->setProcessEnvironment(env);
            CMDProcess->setWorkingDirectory(currentPath);
            CMDProcess->startDetached(prog, args);
    } else {
        PCRProcess->setProcessEnvironment(env);
        PCRProcess->setWorkingDirectory(currentPath);
        PCRProcess->start(prog, args, QIODevice::ReadWrite);
        PCRProcess->waitForFinished(-1);

        // hack to avoid wrong projection in created pcraster maps
        if (moreArgs && prog.toUpper().contains("PCRCALC")) {
            QStringList S = args[0].split("=");
            if (QFileInfo(S[0]).exists()) {
                MAP *m = Mopen(S[0].toLatin1().data(),M_WRITE);
                if (m != nullptr) {
                    MputProjection(m,PT_YDECT2B);
                    Mclose(m);
                }
            }
        }
    }
}
//---------------------------------------------------------------
void nutshellqt::setCursorLast()
{
    QString output = commandWindow->toPlainText();
    QStringList hop = output.split("\n");
    if (hop[hop.size()-1].size() > 1)
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
    //processError = true;
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
void nutshellqt::prevCommand()
{
    QStringList lines;
    QString all;

    if (commandcounter == -1)
        commandcounter = 0;
    else
    {
        commandcounter--;
        commandcounter	= std::max(commandcounter, 0);
    }

    comboBox_cmdlist->setCurrentIndex(commandcounter);

    all = commandWindow->toPlainText();
    lines = all.split("\n");
    lines.replace(lines.size()-1,comboBox_cmdlist->itemText(commandcounter));
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
        commandcounter = std::min(commandcounter, comboBox_cmdlist->count()-1);
    }

    comboBox_cmdlist->setCurrentIndex(commandcounter);

    all = commandWindow->toPlainText();
    lines = all.split("\n");
    lines.replace(lines.size()-1,comboBox_cmdlist->itemText(commandcounter));
    all = lines.join("\n");
    commandWindow->setPlainText(all);

    QTextCursor cur = commandWindow->textCursor();
    cur.setPosition(all.size());
    commandWindow->setTextCursor(cur);
}
