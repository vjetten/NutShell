
#include "nutshellqt.h"


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
		str.remove(str.indexOf("</a>"), str.count()+1);
		str.replace("\">",";");

		if (str.contains("timeinput..."))
		{
			str.remove(str.indexOf(";"),str.count());
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
