/*
 * nutshellFilenames:
 * These functions make valid strings for Aguila of file selections
 * and make a display filter for fileView
 * also functions to get the reported output of a script
 */


#include "nutshellqt.h"


//---------------------------------------------------------------------------
QString nutshellqt::StripForName(QString S)
{
    QString Ss;
    Ss = QFileInfo(S).baseName();

    //FIND THE NUMBER PART AND DELETE IT
    int i = Ss.length()-1;
    while (i > 0 && int(Ss.toAscii()[i]) >= 48 && int(Ss.toAscii()[i]) <= 57)
        i--;
    Ss = Ss.remove(i+1, 256);

    return(Ss);
}
//---------------------------------------------------------------------------
QString nutshellqt::StripForNumber(QString S)
{
    QString Ss;
    //make name without the dot: myname01.001 -> myname01001
    Ss = QFileInfo(S).baseName() + QFileInfo(S).suffix();

    //FIND THE NON NUMBER PART AND DELETE IT
    int i = Ss.length()-1;
    while (i > 0 && int(Ss.toAscii()[i])>=48 && int(Ss.toAscii()[i])<=57)
        i--;
    Ss = Ss.remove(0, i+1);

    //strip all zero's before the value
    while(int(Ss.toAscii()[0])==48)
        Ss.remove(0,1);

    return(Ss);
}
//---------------------------------------------------------------------------
//called if mapseries is selected,
//checks for all separate series
//creates a valid string for aguila
// also fills and does not delete FNall with all series filenmes
QString nutshellqt::GetMapSeries()
{
//    // empty series structure
    fns.clear();

    QDir currentD = QDir(currentPath);
    QFileInfoList allfiles = currentD.entryInfoList(QStringList("*.*"), QDir::Files, QDir::Name);

    QStringList FNts; // list of first and last names in a series
    QString firstname, lastname, Sres = "";

    FNall.clear();
    // find all names that have an integer as extention
    // assumes list is sorted on name so 001 comes before 002
    for (int row = 0; row < allfiles.count(); row++)
        if (isExtentionInt(allfiles.at(row).absoluteFilePath()))
            FNall << allfiles.at(row).absoluteFilePath();//fileName();

    bool startserie = true;
    bool singleserie = false;
    //retain first and last of each series
    for (int i = 0; i < FNall.count()-1; i++)
    {

        QString S1 = FNall.at(i);
        QString S2 = FNall.at(i+1);
        QString S3 = "";

        firstname = S1;
        lastname = S2;
        S1 = StripForName(firstname); // get the serie base name without the "0"
        S2 = StripForName(lastname);

        // find all the separate series by comparing file names
        //if two different basenames at i and i + 1 do something
        if (S1 != S2 && !startserie)
        {
            // finish timeseries by adding first name is last of series
            if (!S1.isEmpty() && !singleserie)
                FNts.append(firstname);

            //extra check to disregard series of which only one file exists
            // if name at i+1 is only single name singleserie is true
            if (i < FNall.count()-2)
            {
                S3 = StripForName(FNall.at(i+2));
                singleserie = S1 != S2 && S2 != S3;
            }

            // start new timeserie by adding last name = first of new series
            if (!singleserie)
                FNts.append(lastname);
        }

        // start a series
        if (S1 == S2 && startserie)
        {
            FNts.append(firstname);
            startserie = false;
        }

        // stop when end of list is reached
        if (S1 == S2 && i+1 == FNall.count()-1)
            FNts.append(lastname);
    }

    for (int i = 0; i < FNts.count(); i+=2)
    {
        // put the first of each series in the filefilter string (only names not path)
        Sres = Sres + QFileInfo(FNts.at(i)).fileName() + ";";

        // fill fns structure for easier series handling
        filenameseries ofn;

        ofn.name = FNts.at(i);
        ofn.base = StripForName(FNts.at(i));
        ofn.begin = StripForNumber(FNts.at(i));
        ofn.end = StripForNumber(FNts.at(i+1));

        QStringList filter;
        filter << QString("%1*.*").arg(ofn.base);
        allfiles = currentD.entryInfoList(filter, QDir::Files, QDir::Name);
        for (int j = 0; j < allfiles.count(); j++)
            ofn.series << allfiles[j].absoluteFilePath();

        fns.append(ofn);

    }
    return Sres;
}
//---------------------------------------------------------------------------
// make a valid stringlist for aguila
// if single selection then check if it is a mapseries start (001) or a normal file
// if it is a multiple selection than add files in a list:
// name.001+34 + dem.map + etc.
// if maps are selected 001 002 003 then this is not seen as a series but separate files
QString nutshellqt::MakeFileListString()
{

    QString Sfilelist;
    QModelIndexList indexes = selectionModel->selectedIndexes();
    QModelIndex index;

    Sfilelist.clear();
    //new style aguila but how in 3D, documentation not clear ??
    QString temp;
    foreach (index, indexes)
    {
        // each file has 4 indexes (name, date, type, size), column 0 is the name
        if (index.column() == 0)
        {
            temp.clear();
            // if it is a series and ismapseries = true
            if (isExtentionInt(fileModel->fileInfo(index).absoluteFilePath()) && ismapseries)
            {
                int j = 0;
                // find the basename
                for (j = 0; j < fns.count(); j++)
                    if (StripForName(fileModel->fileInfo(index).fileName()) == fns[j].base)
                        break;
                temp = QString("%1+%2").arg(fns[j].name).arg(fns[j].end);
                // gives myname00.001+45
            }
            else  // it is a regular file
                temp = fileModel->fileInfo(index).absoluteFilePath(); //fileName(); //

            if (Sfilelist.isEmpty())
                Sfilelist = temp;
            // first selected name
            else
                Sfilelist = Sfilelist + plus + temp;
            // subsequent seleted names, selection order is remembered

            // plus is either "!+!" or "!", ! is later replaced by a space
            // this is to circumvent spaces in pathnames
        }
    }
    //qDebug() << "Sfilelist" << Sfilelist;
    return(Sfilelist);
}
//---------------------------------------------------------------------------
// get all filenames reported,
// swap binding names with real names,
// series only as base name
//QString
bool nutshellqt::getScriptReport()
{
    if (tabWidget->currentIndex() >= 0)
    {

        QStringList SL;      // all strings in document
        QStringList reports; // all reports
        QStringList binding; // binding lines
        QStringList timer;   // timer lines
        bool go;
        reportrecord reportRec;

        reportNames.clear();

        SL = ETPlainText.split("\n");

        binding.clear();
        reports.clear();
        timer.clear();

        bool checksubs = false;
        // strip trailing spaces and check arg substitution
        for(int i = 0; i < SL.count(); i++)
        {
            //SL[i].remove(QRegExp("^\\s+|\\s+$"));
            SL[i] = SL[i].simplified();
            if (SL[i].contains("$"))
                checksubs = true;
        }

        // replace substitutes or with empty string if substitution not checked
        if (checksubs)
        {
            QStringList subs;
            subs = lineEdit_argsubst->text().split(" ", QString::SkipEmptyParts);

            for(int i = 0; i < SL.count(); i++)
            {
                if(SL[i].contains("$"))
                    for (int j = 0; j < subs.count(); j++)
                    {
                        QString repl = QString("$%1").arg(j+1);
                        if (toolButton_argSubs->isChecked() && !lineEdit_argsubst->text().isEmpty())
                            SL[i].replace(repl, subs[j]);
                        else
                            SL[i].replace(repl, "");
                        repl = QString("${%1}").arg(j+1);
                        if (toolButton_argSubs->isChecked() && !lineEdit_argsubst->text().isEmpty())
                            SL[i].replace(repl, subs[j]);
                        else
                            SL[i].replace(repl, "");
                    }
            }
        }

        // get the binding section, only lines with = in them and not starting with comments
        go = false;
        foreach (QString str, SL)
        {
            if (str.indexOf("binding") == 0)
                go = true;
            if (str.indexOf("timer") == 0 ||
                    str.indexOf("areamap") == 0 ||
                    str.indexOf("initial") == 0 ||
                    str.indexOf("dynamic") == 0)
                go = false;

            if (go && str.contains('='))
            {
                QString line;
                line = str;
                line.remove(str.indexOf(";"),str.count()+10); // remove all behind ";"

                line.remove(" "); // remove all spaces
                if (line.contains("=") && !line.contains("#"))
                    binding << line;
            }
        }
         //qDebug() << binding;

        // check the timer section, to find out counter for series
        go = false;
        foreach (QString str, SL)
        {
            if (str.indexOf("timer") == 0)
                go = true;
            if (str.indexOf("areamap") == 0 ||
                    str.indexOf("initial") == 0 ||
                    str.indexOf("dynamic") == 0)
                go = false;

            if (go  && str.contains('='))
            {
                QString line;
                line = str;
                line.remove(str.indexOf("="),str.count()+10);
                //line.remove(QRegExp("^\\s+|\\s+$"));
                line = line.simplified();
                timer << line;
            }
        }
        timer << "endtime";
        //qDebug() << "timer" << timer;

        // get all reports
        go = false;
        foreach (QString str, SL)
        {
            if (str.indexOf("report") == 0)  // string is found
            {
                foreach (QString stra, timer)
                    str.remove(stra);
                // strip keywords under timer, reports only in initial and dynamic

                str.remove(QRegExp("\\([^\\)]*\\)"));
                //strip report (...)
                str.remove(str.indexOf("="),str.count()+10);
                //strip from = to end
                str.remove("report");
                //strip report
                str.remove(" ");
                //strip spaces

                reportRec.reportName = str;
                reportRec.fileName = str;
                reportRec.isSeries = false;

                //reportName << str;
                //reportFileName << str;
                foreach(QString stra, binding)
                {
                    QStringList bind = stra.split("=");
                    if (str == bind[0])
                       reportRec.fileName = bind[1];
                    // give report name the file name from binding
                    // if no dot in the filename it is a series
                }
                if(!reportRec.fileName.contains("."))
                   reportRec.isSeries = true;
                reportNames << reportRec;
            }
        }

        QStringList strtmp;

        for (int i = 0; i < reportNames.count(); i++)
           strtmp << reportNames[i].reportName;

        foreach(QString stra, binding)
        {
           QStringList bind = stra.split("=");
           if (!strtmp.contains(bind[0]))
           {
              reportrecord rr;
              rr.reportName = bind[0];
              rr.fileName = bind[1];
              rr.isSeries = !bind[1].contains(".");
              reportNames << rr;
           }
        }

//           for (int i = 0; i < reportNames.count(); i++)
  //     qDebug() << reportNames[i].fileName << reportNames[i].reportName << reportNames[i].isSeries;
    }

    return (reportNames.count() > 0);
}
//---------------------------------------------------------------------------
int nutshellqt::getTimesteps()
{
    if (tabWidget->currentIndex() >= 0)
    {
        bool go;
        bool nothing = true;
        QString all;
        QStringList SL;
        QStringList ss;
        ss << "0" << "0";

        all = ETPlainText;
        SL = all.split("\n");

        for(int i = 0; i < SL.count(); i++)
            SL[i] = SL[i].simplified();


        go = false;
        foreach (QString str, SL)
        {
            if (str.indexOf("timer") == 0)
            {
                go = true;
                nothing = false;
            }
            if (str.indexOf("areamap") == 0 ||
                    str.indexOf("initial") == 0 ||
                    str.indexOf("dynamic") == 0)
                go = false;

            if (go  && str.contains(';') && !str.contains("endtime")
                    && !str.simplified().indexOf("#") == 0)
            {
                QString line;
                line = str.simplified();
                line.remove(line.indexOf("#"),line.count()+10);
                ss = line.split(" ");
            }
        }
        if (nothing)
            return -1;
        else
            return ss[1].toInt();
    }
    return -1;
}
//---------------------------------------------------------------
bool nutshellqt::isTSSfile(QString name)
{
    bool istss = true;

    QFile file(name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    QString line = in.readLine();
    // read the header line
    bool ok;
    line = in.readLine();
    // read the nr of vars
    int nr = line.toInt(&ok, 10);
    if (!ok)
        istss = false;
    for (int i = 0; i < nr; i++)
        line = in.readLine();

    while (!in.atEnd())
    {
        line = in.readLine();
        QStringList vals;
        vals = line.split(QRegExp("\\s"),QString::SkipEmptyParts);
        if (vals.count() != nr )
            istss = false;
    }
    return(istss);
}
//---------------------------------------------------------------
bool nutshellqt::isExtentionInt(QString name)
{
    bool ok;
    long val = QFileInfo(name).suffix().toInt(&ok, 10);
    return (val > 0 && ok);
}
//---------------------------------------------------------------

