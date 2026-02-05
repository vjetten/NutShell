/*
 * nutshellFilenames:
 * These functions make valid strings for Aguila of file selections
 * and make a display filter for fileView
 * also functions to get the reported output of a script
 * Author: VJ 140222,181001
 */


#include "nutshellqt.h"
#include <gdal_priv.h>
#include <gdal.h>

/*
 *QString nutshellqt::StripForName(QString S)
 *QString nutshellqt::StripForNumber(QString S)
 *QString nutshellqt::getMapSeries()
 *QString nutshellqt::getFileListString()
 *bool nutshellqt::getScriptReport()
 *int nutshellqt::getTimesteps()
 *bool nutshellqt::isTSSfile(QString name)
 *bool nutshellqt::isExtentionInt(QString name)
*/

//---------------------------------------------------------------------------
//! Find the number part of a map series and delete it for the base name
QString nutshellqt::StripForName(QString S)
{
    QString Ss;
    Ss = QFileInfo(S).baseName();
    int i = Ss.length()-1;
    while (i > 0 && int(Ss.toLatin1()
                        [i]) >= 48 && int(Ss.toLatin1()
                                          [i]) <= 57)
        i--;
    Ss = Ss.remove(i+1, 256);

    return(Ss);
}
//---------------------------------------------------------------------------
//! Find the base name part of a map series and delete it for the numer info
QString nutshellqt::StripForNumber(QString S)
{
    QString Ss;
    //make name without the dot: myname01.001 -> myname01001
    Ss = QFileInfo(S).baseName() + QFileInfo(S).suffix();

    int i = Ss.length()-1;
    while (i > 0 && int(Ss.toLatin1()[i])>=48 && int(Ss.toLatin1()[i])<=57)
        i--;
    Ss = Ss.remove(0, i+1);

    //strip all zero's before the value
    while(int(Ss.toLatin1()[0])==48)
        Ss.remove(0,1);

    return(Ss);
}
//---------------------------------------------------------------------------
//called if mapseries is selected,
//checks for all separate series, returns a valid string for aguila
// also fills FNall with all series filenames
QString nutshellqt::getMapSeries()
{
    //empty series structure and its treeview copy
    fns.clear();
    treeView->fns.clear();

    QDir currentD = QDir(currentPath);
    QFileInfoList allfiles = currentD.entryInfoList(QStringList("*.*"), QDir::Files, QDir::Name);

    QStringList FNts; // list of first and last names in a series
    QString firstname, lastname, Sres = "";

    FNall.clear();
    // find all names that have an integer as extention
    // assumes list is sorted on name so 001 comes before 002
    for (int row = 0; row < allfiles.size(); row++)
        if (isExtentionInt(allfiles.at(row).fileName()))
            FNall << allfiles.at(row).absoluteFilePath();

    bool startserie = true;
    bool singleserie = false;
    //retain first and last of each series
    for (int i = 0; i < FNall.size()-1; i++)
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
            if (i < FNall.size()-2)
            {
                S3 = StripForName(FNall.at(i+2));
                singleserie = S1 != S2 && S2 != S3;
            }

            // start new timeserie by adding last name = first of new series
            // unless new name is last in the series, then it is a single name
            if (!singleserie  && i+2 != FNall.size())
                FNts.append(lastname);
        }

        // start a series
        if (S1 == S2 && startserie)
        {
            FNts.append(firstname);
            startserie = false;
        }

        // stop when end of list is reached
        if (S1 == S2 && i+1 == FNall.size()-1)
            FNts.append(lastname);
    }

    for (int i = 0; i < FNts.size(); i+=2)
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
        for (int j = 0; j < allfiles.size(); j++)
            if (isExtentionInt(allfiles[j].fileName()))
                ofn.series << allfiles[j].absoluteFilePath();

        fns.append(ofn);
        treeView->fns.append(ofn);

    }
    return Sres;
}
//---------------------------------------------------------------------------
// make a valid stringlist for aguila
// if single selection then check if it is a mapseries start (001) or a normal file
// if it is a multiple selection than add files in a list:
// name.001+34 + dem.map + etc.
// if maps are selected 001 002 003 then this is not seen as a series but separate files
QString nutshellqt::getFileListString()
{
//qDebug() << "getfileliststring";
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
                for (j = 0; j < fns.size(); j++)
                {
                    QString sss = StripForName(fileModel->fileInfo(index).fileName());
                    if (sss.compare(fns[j].base, Qt::CaseInsensitive) == 0)
                        break;
                }
                temp = QString("%1+%2").arg(fns[j].name).arg(fns[j].end);
                // gives myname00.001+45
            }
            else  // it is a regular file
                temp = fileModel->fileInfo(index).absoluteFilePath(); //fileName(); //
            //qDebug() << index << temp;
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
    //qDebug() << Sfilelist;
    return(Sfilelist);
}
//---------------------------------------------------------------------------
#define NOTCONSTANT(V)   (!V.contains("scalar(") && !V.contains("nominal(") && !V.contains("ordinal(") \
    && !V.contains("dirctional(") && !V.contains("ldd(") && !V.contains("boolean("))

// get all filenames reported,
// swap binding names with real names, series only as base name
bool nutshellqt::getScriptReport(bool addBinding)
{
    if (ETExists)
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
        for(int i = 0; i < SL.size(); i++)
        {
            SL[i] = SL[i].simplified();
            if (SL[i].contains("$"))
                checksubs = true;
        }

        // replace substitutes or with empty string if substitution not checked
        if (checksubs)
        {
            QStringList subs;
            subs = lineEdit_argsubst->text().split(" ", Qt::SkipEmptyParts);

            for(int i = 0; i < SL.size(); i++)
            {
                if(SL[i].contains("$"))
                    for (int j = 0; j < subs.size(); j++)
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
        for (int i = 0; i < SL.size(); i++)
        {
            QString str = SL[i];
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
                line.remove(str.indexOf(";"),str.size()+10); // remove all behind ";"

                line.remove(" "); // remove all spaces
                if (line.contains("=") && !line.contains("#"))
                {
                    //line = line + QString("=%1").arg(i);
                    // add org line number, needed later
                    binding << line;
                }
            }
        }

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
                line.remove(str.indexOf("="),str.size()+10);
                line = line.simplified();
                timer << line;
            }
        }
        timer << "endtime";

        // get all reports
        go = false;
        for (int i = 0; i < SL.size(); i++)
        {
            QString str = SL[i];
            if (str.indexOf("report") == 0)  // string is found
            {
                foreach (QString stra, timer)
                    str.remove(stra);
                // strip keywords under timer, reports only in initial and dynamic

                str.remove(QRegularExpression("\\([^\\)]*\\)"));
                //strip report (...)
                str.remove(str.indexOf("="),str.size()+10);
                //strip from '=' to end of line
                str.remove("report");
                //strip report keyword
                str.remove(" ");
                //strip spaces

                reportRec.reportName = str;
                reportRec.fileName = str;
                reportRec.isSeries = false;
                reportRec.isBinding = false;
                //reportRec.lineNumber = i;

                // add filename to reportname if it exists in the binding
                // else filename = reportname
                foreach(QString stra, binding)
                {
                    QStringList bind = stra.split("=");
                    if (str == bind[0])
                    {
                        reportRec.fileName = bind[1];
                        reportRec.isBinding = true;
                    }
                }
                // if no dot in the filename it is a series

                //reportRec.isSeries = (!reportRec.fileName.contains(".") && NOTCONSTANT(reportRec.fileName));
                //                        !reportRec.fileName.contains("scalar(")
                //                        && !reportRec.fileName.contains("nominal(") && !reportRec.fileName.contains("ordinal(")
                //                        && !reportRec.fileName.contains("dirctional(") && !reportRec.fileName.contains("ldd(") && !reportRec.fileName.contains("boolean(");
                reportNames << reportRec;
            }
        }
        //        if (reportNames.size() == 0)
        //            return(false);
        // add the variables in the binding that are not reported but
        // may have a file, these are prob. mostly input file

        if (addBinding)
        {
            for (int i = 0; i < binding.size(); i++)
            {
                QStringList bind = binding[i].split("=");
                bool found = false;

                foreach(reportrecord reportRec, reportNames)
                {
                    if (reportRec.reportName == bind[0])
                    {
                        found = true;
                        break;
                    }
                }

                if (!found && NOTCONSTANT(bind[1]))
                {
                    reportRec.reportName = bind[0];
                    reportRec.fileName = bind[1];
                    //reportRec.isSeries = !bind[1].contains(".");
                    reportRec.isBinding = true;
                    reportNames << reportRec;
                }
            }
        }

        for (int i = 0; i < reportNames.size(); i++)
            reportNames[i].isSeries = !reportNames[i].fileName.contains(".");

        //        for (int i = 0; i < reportNames.size(); i++)
        //            qDebug() << "rn" << reportNames[i].fileName << reportNames[i].reportName << reportNames[i].isSeries << reportNames[i].isBinding;
    }
    return (reportNames.size() > 0);
}
//---------------------------------------------------------------------------
int nutshellqt::getTimesteps()
{
    if (ETExists)
    {
        bool go;
        bool nothing = true;
        QString all;
        QStringList SL;
        QStringList ss;
        ss << "0" << "0";

        all = ETPlainText;
        SL = all.split("\n");

        for(int i = 0; i < SL.size(); i++)
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
                line.remove(line.indexOf("#"),line.size()+10);
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
bool nutshellqt::isTSSFile(const QString& filename)
{
    bool istss = true;

    QFile file(filename);
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
        // skip empty lines
        if (line.simplified().isEmpty())
            continue;
        QStringList vals;
        vals = line.split(QRegularExpression("\\s"),Qt::SkipEmptyParts);
        if (vals.size() != nr )
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
//---------------------------------------------------------------------------
bool nutshellqt::isTextFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    const qint64 bufferSize = 1024;
    QByteArray data = file.read(bufferSize);
    file.close();

    if (data.isEmpty())
        return false;

    int binaryCount = 0;
    for (char byte : data) {
        uchar u = static_cast<uchar>(byte);

        if (u == 0) {
            return false; // NUL byte = binary
        }

        // Count as binary if outside common printable text range
        if ((u < 7 || (u > 14 && u < 32)) && u != 9) {
            ++binaryCount;
        }
    }

    // Heuristic: if more than 10% is suspicious, it's probably binary
    return binaryCount < data.size() / 10;
}

/*
bool nutshellqt::isTextFile(const QString& filename)
{
    qint64 bufferSize = 1024;
    bool okay = true;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        okay = false;
        //return false;
    }

    if (okay) {
        char buffer[bufferSize];
        qint64 bytesRead = file.read(buffer, bufferSize);

        for (int i = 0; i < bytesRead; ++i) {
            // Check if the character is outside the typical ASCII text range
            if (buffer[i] < 0 || buffer[i] > 127) {
                file.close();
                okay = false;  // Binary file
            }
        }
    }

    file.close();

//    if (!okay)
//        ErrorMsg(QString("File %1 cannot be read as text file").arg(filename));

    return okay;  // Text file
}
*/
//---------------------------------------------------------------------------
bool nutshellqt::isMapFile(const QString& filename)
{
    MAP *m = Mopen(filename.toLatin1(),M_READ_WRITE);

    if (m != nullptr)
    {
        MputProjection(m,PT_YDECT2B);
        // HACK to force axis direction

        Mclose(m);
        m=nullptr;
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool nutshellqt::isTiffFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open file:" << filePath;
        return false;
    }

    // Read the first 4 bytes
    QByteArray header = file.read(4);
    if (header.size() < 4) {
        qWarning() << "Error reading file:" << filePath;
        return false;
    }

    // Check for TIFF headers
    bool isTiff = (header[0] == 'I' && header[1] == 'I' && header[2] == 42 && header[3] == 0) ||
                  (header[0] == 'M' && header[1] == 'M' && header[2] == 0 && header[3] == 42);

    file.close();
    return isTiff;
}
