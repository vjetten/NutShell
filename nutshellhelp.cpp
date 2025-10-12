/*
 * nutshellHelp:
 * simple help functions

 * Author: VJ 140222,181001
 */

#include <QUrl>
#include "nutshellqt.h"

//---------------------------------------------------------------
void nutshellqt::showNutshellHelp()
{
    QDialog *box = new QDialog();
    box->setWindowTitle("Overview of functions in NutShell");
  // box->setWindowFlags(Qt::Window|Qt::WindowTitleHint);

    QLabel *label = new QLabel();
    label->setPixmap(QPixmap(":/resources/nutshellhelp.png"));
    label->setScaledContents(true);
    QHBoxLayout *horizontalLayout = new QHBoxLayout(box);
    horizontalLayout->addWidget(label);

    box->show();
}
//---------------------------------------------------------------
void nutshellqt::showHelp()
{
//    if (QFileInfo(PCRasterDocDirName+"index.html").exists())
//        QDesktopServices::openUrl(QUrl("file:///"+PCRasterDocDirName + "index.html"));
//    else
      showWebHelp();
}
//---------------------------------------------------------------
void nutshellqt::showWebHelp()
{
    QDesktopServices::openUrl(QUrl("http://pcraster.geo.uu.nl/documentation/PCRaster/html/index.html"));
}
//---------------------------------------------------------------
void nutshellqt::showAguilaHelp()
{
    QDesktopServices::openUrl(QUrl("http://pcraster.geo.uu.nl/documentation/Aguila/Aguila.pdf"));
}
//---------------------------------------------------------------
void nutshellqt::showHelpOperation()
{
    if (!ETExists)
        return;

    QString helptxt = ETEditor->textCursor().selectedText();

    QString URL = "http://pcraster.geo.uu.nl/documentation/PCRaster/html/";

//    if (!QUrl(URL).isValid())
//    {
//        QMessageBox::warning(this,"web based help",QString("Cannot find the PCRaster website"));
//        return;
//    }

    if (helptxt.isEmpty())
    {
        QMessageBox::warning(this,"Context-sensitive help",QString("Highlight a keyword to find help with ctrl-F1"));
        return;
    }

    QString found = "";

    if (helptxt.contains("$"))
        found = QString("secpcrcalcscriptfeatures.html#substitution-of-arguments");
    else
        if (helptxt.contains("report"))
            found = QString("secpcrcalcscriptfeatures.html#index-3");
        else
            if (helptxt.contains("--"))
                found = QString("secimport.html#overview-of-global-options");
            else
                if (helptxt.contains("#!"))
                    found = QString("secimport.html#global-options");
                else
                    if (helptxt.toUpper() == "BINDING" ||
                            helptxt.toUpper() == "INITIAL" ||
                            helptxt.toUpper() == "TIMER" ||
                            helptxt.toUpper() == "AREAMAP" ||
                            helptxt.toUpper() == "DYNAMIC")
                        found = QString("secdyn.html#%1-section").arg(helptxt);
                    else
                    {
                        if (helptxt.contains("lookup"))
                            helptxt = "lookup";
                        // lookupscalar and so on
                        if (helptxt.contains("timeinput") && helptxt != "timeinput")
                            helptxt = "timeinput...";
                        // timeinputscalar and so on, but not timrinput itself
                        found = QString("op_%1.html").arg(helptxt);
                        // all the rest
                    }
    //qDebug() << URL+found << helptxt;
    QDesktopServices::openUrl(QUrl(URL+found));

}
//---------------------------------------------------------------

/*
 * OLD STUFF TO USE OLD HELP ON DISK
 * all help is online docs via iexplorer, firefox etc

nutshellHelp::nutshellHelp(QWidget *parent) :
      QDialog(parent)
{
   setupUi(this);

   //helpView = new QWebView(this);
   //verticalLayout->insertWidget(0, helpView);
}

nutshellHelp::~nutshellHelp()
{
}

void nutshellHelp::seturl(QString url)
{
   helpurl = QString("file:///") + url;
}

void nutshellHelp::findOperation(QString operation)
{
   bool nothing = true;
   if (operation.isEmpty())
      return;

   QString helpstr = QString("http://pcraster.geo.uu.nl/documentation/PCRaster/html/op_%1.html").arg(operation);

   if (operation.contains("lookup"))
      operation = "lookup";

   if (operation == "if" || operation == "then")
      operation = "if then";

   if (operation == "else")
      operation = "if then else";

   if (operation == "/" || operation == "div")
      operation = "/ or div";

   if (operation == "==" || operation == "eq")
      operation = "== or eq";

   if (operation == ">=" || operation == "ge")
      operation = ">= or ge";

   if (operation == "<=" || operation == "le")
      operation = "<= or le";

   if (operation == "<" || operation == "lt")
      operation = "< or lt";

   if (operation == ">" || operation == "gt")
      operation = "> or gt";

   if (operation == "!=" || operation == "ne")
      operation = "!= or ne";

   QStringList line;
   foreach(QString str, helphtml)
   {
      line.clear();
      line << str.split(";");
      if(line[1] == operation)
      {
         helpurl = line[0];
         break;
         nothing = false;
      }
   }
   if (nothing)
      operation = "";
}

void nutshellHelp::setuphelp(QString pcrhelp)
{
   QFile file(pcrhelp + "index.html");
   if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
      return;

   QStringList line;
   QTextStream in(&file);

   while (!in.atEnd())
   {
      QString str = in.readLine();
      if (str.contains("rn01re"))
         line << str;
      if (str.contains("rn02"))
         break;
   }

   helphtml.clear();
   foreach(QString str, line)
   {
      str.remove(0, str.indexOf("rn01re"));
      str.remove(str.indexOf("</a>"), str.size()+1);
      str.replace("\">",";");

      if (str.contains("timeinput..."))
      {
         str.remove(str.indexOf(";"),str.size());
         helphtml << str+";timeinputscalar";
         helphtml << str+";timeinputnominal";
         helphtml << str+";timeinputordinal";
         helphtml << str+";timeinputdirectional";
         helphtml << str+";timeinputldd";
         helphtml << str+";timeinputboolean";
      }
      else
       helphtml << str;
   }

   QString url = "ch03s06.html#secimportoptover";
   helphtml << url + ";clone"       ;
   helphtml << url + ";unittrue"    ;
   helphtml << url + ";unitcell"    ;
   helphtml << url + ";coorcentre"  ;
   helphtml << url + ";coorul or"   ;
   helphtml << url + ";coorlr"      ;
   helphtml << url + ";degrees"     ;
   helphtml << url + ";radians"     ;
   helphtml << url + ";columntable" ;
   helphtml << url + ";matrixtable" ;
   helphtml << url + ";lddout"      ;
   helphtml << url + ";lddin"       ;
   helphtml << url + ";lddfill"     ;
   helphtml << url + ";lddcut"      ;
   helphtml << url + ";diagonal"    ;
   helphtml << url + ";nondiagonal" ;
   helphtml << url + ";noprogress"  ;
   helphtml << url + ";progress"    ;
   helphtml << url + ";nothing"     ;

   helphtml << "ch05.html#secseqbla;report";

   helphtml << "ch05.html#figseqfig;binding";
   helphtml << "ch05.html#figseqfig;timer";
   helphtml << "ch05.html#figseqfig;areamap";
   helphtml << "ch05.html#figseqfig;initial";
   helphtml << "ch05.html#figseqfig;dynamic";

}
*/
