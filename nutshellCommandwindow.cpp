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
    QString all = commandWindow->toPlainText();

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

    setWorkdirectory();

    executeCommand(args);

    setCursorLast();

    comboBox_cmdlist->insertItem(0, lines[lines.count()-1]);
    commandcounter = -1;
    // add command to commandlist
}
//---------------------------------------------------------------
void nutshellqt::executeCommand(QStringList args)
{
    QString condaenv = CondaDirName;
    QStringList Sn = condaenv.split('/');
    QString envname = Sn.at(Sn.count()-2);

    QString condabase = QDir(condaenv+"/../..").absolutePath();
    QString condascripts = QDir(condabase+"/Scripts").absolutePath();

    qDebug() << CondaDirName << envname << condabase << condascripts;

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("CONDA_DEAFULT_ENV",envname);
    env.insert("CONDA_EXE",condascripts+"conda.exe");
    env.insert("CONDA_PREFIX", condaenv);
    env.insert("CONDA_PREFIX_1", condabase);
    env.insert("CONDA_PYTHON_EXE",condabase+"/python.exe");
    env.insert("CONDA_SHLVL","2"); // 1 ?

    env.insert("USE_PATH_FOR_GDAL_PYTHON","YES");
    env.insert("CONDA_ACTIVATE_SCRIPT",condascripts+"/activate");
    env.insert("CONDA_ENV_PATH",condaenv);
    env.insert("CONDA_ENV_PYTHON",condaenv+"python.exe");

    env.insert("GDAL_DATA",condaenv+"Library/share/gdal");
    env.insert("GDAL_DRIVER",condaenv+"Library/share/gdal");
    env.insert("GDAL_DRIVER_PATH",condaenv+ "Library/lib/gdalplugins");
    env.insert("GEOTIFF_CSV",condaenv+"Library/share/epsg_csv");
    QString addpath = condaenv+";"
                      +condaenv+"Library/bin/minw-w64/bin;"
                      +condaenv+"Library/usr/bin;"
                      +condaenv+"Library/bin;"
                      +condaenv+"Scripts;"
                      +condaenv+"bin;"
                      +condaenv+"condabin;"
                      +condaenv+"/Scripts;";

    env.insert("PATH", addpath + env.value("Path"));

    PCRProcess->setProcessEnvironment(env);

    // Set the working directory for the process
    PCRProcess->setWorkingDirectory(currentPath);

    // Set the command to run

    // run pcrcalc with -f: because of timer treat differently
    if ((args[0].toUpper() == "PCRCALC")
            && (args.count() > 1 && args[1].indexOf("-f",Qt::CaseInsensitive) == 0))
    {
        args.removeAt(0);
        QString prog = CondaDirName+"Library/bin/pcrcalc.exe";
        runModelCommandwindow(prog, args);
        return;
    }

    QString prog;
    bool isCMD = false;
    bool moreArgs = args.count() > 1;

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
            isCMD = true;
        } else
            if (args[0].toUpper().contains(".CMD") || args[0].toUpper().contains(".BAT")) {
               // prog = currentPath+"/"+args[0];
                createBatch(args[0],"");//prog, "");
                prog = "cmd.exe";
                args.clear();
                args << "cmd.exe" << "/c" << NutshellDirName + "_nutshell_batchjob.cmd";
              //  isCMD = true;
            } else
                if (args[0].toUpper().contains("MAPEDIT")) {
                    prog = NutshellDirName + args[0]+".exe";
                    isCMD = true;
                }
                else
                    prog = CondaDirName+"Library/bin/"+args[0]+".exe";


    // Set the command arguments if needed
    args.removeAt(0);

    if (isCMD) {
        PCRProcess->startDetached(prog, args);
    } else {
        bufprev = "";
        PCRProcess->start(prog, args, QIODevice::ReadWrite);
        PCRProcess->waitForFinished(-1);

        QString buffer = QString(PCRProcess->readAllStandardOutput());
        commandWindow->appendPlainText(buffer);
        QCoreApplication::sendPostedEvents(this, 0);

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
void nutshellqt::prevCommand()
{
    QStringList lines;
    QString all;

    if (commandcounter == -1)
        commandcounter = 0;
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
