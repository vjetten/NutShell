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
    //find the integer, delete the dot
    Ss = QFileInfo(S).fileName();//absoluteFilePath();
    int i = Ss.lastIndexOf(".");
    Ss.remove(i,1);

    //FIND THE NON NUMBER PART AND DELETE THE REST
    while (i > 0 && int(Ss.toAscii()[i]) >= 48 && int(Ss.toAscii()[i]) <= 57)
        i--;
    for (int i = Ss.length()-1; i >= 0; i--)
        if (int(Ss.toAscii()[i]) < 48 && int(Ss.toAscii()[i]) > 57)
            break;

    Ss = Ss.remove(i+1,128);

    return(Ss);
}
//---------------------------------------------------------------------------
QString nutshellqt::StripForNumber(QString S)
{
    QString Ss = S;
    //find the integer, delete the dot
    Ss = QFileInfo(S).baseName() + QFileInfo(S).suffix();
    int i = Ss.length()-1;
    //FIND THE NON NUMBER PART AND DELETE IT
    while (i > 0 && int(Ss.toAscii()[i])>=48 && int(Ss.toAscii()[i])<=57)
        i--;
    i++;
    Ss = Ss.remove(0,i);
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
    // empty series structure
    for (int i = 0; i < 128; i++)
    {
        fns[i].name.isEmpty();
        fns[i].base.isEmpty();
        fns[i].begin.isEmpty();
        fns[i].end.isEmpty();
        fns[i].series.clear();
    }
    nrseries = 0;

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

    // sort to be sure???
    //FNall.sort();

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
        fns[nrseries].name = FNts.at(i);
        fns[nrseries].base = StripForName(FNts.at(i));
        fns[nrseries].begin = StripForNumber(FNts.at(i));
        fns[nrseries].end = StripForNumber(FNts.at(i+1));
        QStringList filter;
        filter << QString("%1*.*").arg(QFileInfo(fns[nrseries].base).baseName());
        allfiles = currentD.entryInfoList(filter, QDir::Files, QDir::Name);
        for (int j = 0; j < allfiles.count(); j++)
            fns[nrseries].series<< allfiles[j].absoluteFilePath();

        //      qDebug()<< nrseries << fns[nrseries].name << fns[nrseries].base << fns[nrseries].begin << fns[nrseries].end << fns[nrseries].series;

        nrseries++;
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
    // each file has 4 indexes (name, date, type, size)

    Sfilelist.clear();
    //new style aguila but how in 3D, documentation not clear ??
    QString temp;
    foreach (index, indexes)
    {
        if (index.column() == 0)
        {
            temp.isEmpty();
            if (isExtentionInt(fileModel->fileInfo(index).absoluteFilePath()) && ismapseries) // if it is a series and ismapseries = true
            {
                int j = 0;

                for (j = 0; j < nrseries; j++)
                    if (StripForName(fileModel->fileInfo(index).fileName()) == fns[j].base)
                        break;
                temp = QString("%1+%2").arg(fns[j].name).arg(fns[j].end);
            }
            else  // it is a regular file
                temp = fileModel->fileInfo(index).absoluteFilePath(); //fileName(); //

            if (Sfilelist.isEmpty())
                Sfilelist = temp;
            else
                Sfilelist = Sfilelist + plus + temp;
        }
    }
    //qDebug() << "Sfilelist" << Sfilelist;
    return(Sfilelist);
}
//---------------------------------------------------------------------------
// get all filenames reported,
// swap binding names with real names,
// series only as base name
QString nutshellqt::getScriptReport()
{
    if (tabWidget->currentIndex() >= 0)
    {

        QString all;
        QStringList SL;
        QStringList reports;
        QStringList binding;
        QStringList timer;
        bool go = false;

        all = ETPlainText;
        SL = all.split("\n");

        binding.clear();
        reports.clear();
        timer.clear();

        bool checksubs = false;
        // strip trailing spaces
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
//                        if(checkBox_argsubst->isChecked() && !lineEdit_argsubst->text().isEmpty())
                        if (toolButton_argSubs->isChecked() && !lineEdit_argsubst->text().isEmpty())
                            SL[i].replace(repl, subs[j]);
                        else
                            SL[i].replace(repl, "");
                        repl = QString("${%1}").arg(j+1);
                        if (toolButton_argSubs->isChecked() && !lineEdit_argsubst->text().isEmpty())
//                        if(checkBox_argsubst->isChecked() && !lineEdit_argsubst->text().isEmpty())
                            SL[i].replace(repl, subs[j]);
                        else
                            SL[i].replace(repl, "");
                    }
            }
        }

        // get the binding section, only lines with = in them and no comments
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
        // qDebug() << binding;

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
        QStringList name;
        name.clear();
        go = false;
        foreach (QString str, SL)
        {
            if (str.indexOf("report") == 0)
            {
                foreach (QString stra, timer)
                    str.remove(stra);
                // strip keywords under timer
                str.remove(QRegExp("\\([^\\)]*\\)"));
                //strip report (...)
                str.remove(str.indexOf("="),str.count()+10);
                //strip from = to end
                str.remove("report");
                //strip report
                str.remove(" ");
                //strip spaces

                foreach(QString stra, binding)
                {
                    QStringList bind = stra.split("=");
                    if (str == bind[0])
                        str = bind[1];//str.replace(bind[0],bind[1]);
                }
                //replace report vars with binding vars

                //            if (!str.contains(".")) // mapseries
                //            {
                //               name << QString("%1*.*").arg(str);
                //               go = true;
                //            }
                //				else
                reports << str;//currentPath+"/"+str;
            }
        }

        all = reports.join("\n");
        //qDebug() << "all" << all;
        return all;
    }

    return QString("");
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

            if (go  && str.contains(';') && !str.contains("endtime") && !str.simplified().indexOf("#")==0)
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
    int val = QFileInfo(name).suffix().toInt(&ok, 10);
    return (val > 0 && ok);
}
//---------------------------------------------------------------

