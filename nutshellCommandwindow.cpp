/*
 * NutshellCommandwindow

 * functions to deal with commands given in commandwindow and cmd list
 * Author: VJ 140222
 */


#include "nutshellqt.h"


//---------------------------------------------------------------
void nutshellqt::setupCommandwindow()
{
    PCRProcess = new QProcess(this);
    processError = false;
    PCRProcess->setProcessChannelMode(QProcess::MergedChannels);
    connect(PCRProcess, SIGNAL(readyReadStandardOutput()),this, SLOT(outputCommand()) );
    connect(PCRProcess, SIGNAL(readyReadStandardError()),this, SLOT(readFromStderr()) );
    connect(PCRProcess, SIGNAL(error(QProcess::ProcessError)),this, SLOT(errorCommand()) );
    // process called by command window, typing pcrcalc, or aguila or any other pcr prog

    commandWindow = new nutshelleditor(this, 1);
    commandWindow->document()->setDocumentMargin(2);
    commandWindow->setFrameShape(QFrame::StyledPanel);
   // commandWindow->setFrameStyle(QFrame::Sunken);

    verticalLayoutCmdlist->insertWidget(0, commandWindow);
    commandWindow->installEventFilter(this);
    commandWindow->document()->setDefaultFont(QFont("Consolas", 10));
    //commandWindow->document()->setDefaultFont(QFont("Courier New", 10));
    // declare and set the commandWindow

    connect(toolButton_clearcmd, SIGNAL(clicked()), this, SLOT(clearCommandWindow()));
    connect(toolButton_cmdlist, SIGNAL(clicked()), this, SLOT(clearCommandList()));

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

    if ((args[0].toUpper() == "PCRCALC" || args[0].toUpper() == "OLDCALC")
            && (calcProcess && calcProcess->state() == QProcess::Running))
    {
        ErrorMsg("pcrcalc is active, wait until it is finished or press stop first");
        return;
    }

    prog = PCRasterAppDirName + args[0] +".exe";

    if ((args[0].toUpper() == "PCRCALC" || args[0].toUpper() == "OLDCALC")
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
        {
            args.removeAt(0);
            PCRProcess->start(prog, args);
       //     PCRProcess->waitForReadyRead(10000);
            PCRProcess->waitForFinished(120000);
          //  commandWindow->appendPlainText("");
        }
    setCursorLast();
    if (!processError)
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
    QByteArray ba = PCRProcess->readAllStandardOutput();
    //	commandWindow->cursor = 1;
    commandWindow->appendPlainText(ba);
    QCoreApplication::sendPostedEvents(this, 0);
    //	commandWindow->cursor = 1;
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