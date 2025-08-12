#ifndef NUTSHELLHIGHLIGHTER_H
#define NUTSHELLHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE


class PCRHighlighter : public QSyntaxHighlighter {
public:
    PCRHighlighter(QTextDocument *parent = nullptr) : QSyntaxHighlighter(parent) {
        QTextCharFormat keywordFormat;
        keywordFormat.setForeground(Qt::black);
        keywordFormat.setFontWeight(QFont::Bold);

        QTextCharFormat commentFormat;
        commentFormat.setForeground(QColor("gray"));

        QTextCharFormat PCRFormat;
        PCRFormat.setForeground(QColor("yellow"));
        PCRFormat.setBackground(QColor("gray"));
        PCRFormat.setFontWeight(QFont::Bold);

        QTextCharFormat ReportFormat;
        ReportFormat.setForeground(Qt::red);
        ReportFormat.setFontWeight(QFont::Bold);

        QTextCharFormat stringFormat;
        stringFormat.setForeground(QColor("green"));

        QStringList PCRkeywords = {"binding", "areamap", "timer", "initial", "dynamic"};
        QStringList Reportkeywords = {"report"};

        const QStringList Mainkeywords = {
            "le","div","**","and","eq","==","ge",">=","<=","gt",">","then","else","lt","<","ne","!=",
            "not","or","xor","abs","accucapacityflux","accucapacitystate","accuflux","accufractionflux",
            "accufractionstate","accuthresholdflux","accuthresholdstate","accutriggerflux","accutriggerstate",
            "accutraveltimestate","accutraveltimeflux","accutraveltimeremoved","accutraveltimefractionstate",
            "accutraveltimefractionflux","accutraveltimefractionremoved","acos","argorder","argorderwithid",
            "argorderaddarealimited","argorderwithidaddarealimited","argorderarealimited",
            "argorderwithidarealimited","areaarea","areaaverage","areadiversity","areamajority",
            "areamaximum","areaminimum","areanormal","areaorder","areatotal","areauniform","asin",
            "aspect","atan","boolean","catchment","catchmenttotal","cellarea","celllength","clump",
            "cos","cover","defined","directional","downstream","downstreamdist","dynwavestate",
            "dynwaveflux","dynamicwave","exp","extentofview","fac","idiv","if","influencesimplegauss",
            "inversedistance","kinematic","kinwavestate","kinwaveflux","ldd","lddcreate","lddcreatedem",
            "ldddist","lddmask","lddrepair","ln","log10","lookupboolean","lookupnominal",
            "lookupordinal","lookupscalar","lookupdirectional","lookupldd","lookuplinear",
            "lookupmapstack","maparea","mapmaximum","mapminimum","mapnormal","maptotal","mapuniform",
            "markwhilesumle","markwhilesumge","max","min","mod","nodirection","nominal","normal",
            "order","ordinal","path","pit","plancurv","pred","profcurv","repeat","rounddown",
            "roundoff","roundup","scalar","shift","shift0","sin","slope","slopelength","spread",
            "spreadldd","spreadlddzone","spreadmax","spreadzone","sqr","sqrt","streamorder",
            "subcatchment","succ","tan","time","timeinputboolean","timeinputnominal","timeinputordinal",
            "timeinputdirectional","timeinputscalar","timeinputldd","timeinput","timeinputmodulo",
            "timeinputsparse","timeoutput","timeslice","uniform","uniqueid","until","upstream","view",
            "windowtotal","windowaverage","windowdiversity","windowhighpass","windowmajority",
            "windowmaximum","windowminimum","xcoordinate","ycoordinate"
        };

        // Handle all keywords properly
        for (const QString &keyword : Mainkeywords) {
            QString pattern;
            if (isWordLike(keyword)) {
                pattern = "\\b" + QRegularExpression::escape(keyword) + "\\b";
            } else {
                pattern = QRegularExpression::escape(keyword);
            }

            QRegularExpression regex(pattern);
            if (regex.isValid()) {
                highlightingRules.append(qMakePair(regex, keywordFormat));
            }
        }

        // PCR keywords
        for (const QString &keyword : PCRkeywords) {
            QRegularExpression regex("\\b" + QRegularExpression::escape(keyword) + "\\b");
            highlightingRules.append(qMakePair(regex, PCRFormat));
        }

        // Report keywords
        for (const QString &keyword : Reportkeywords) {
            QRegularExpression regex("\\b" + QRegularExpression::escape(keyword) + "\\b");
            highlightingRules.append(qMakePair(regex, ReportFormat));
        }

        // Comments
        highlightingRules.append(qMakePair(QRegularExpression("#[^\n]*"), commentFormat));

        // Strings (single and double quoted)
        highlightingRules.append(qMakePair(QRegularExpression("'[^']*'"), stringFormat));
        highlightingRules.append(qMakePair(QRegularExpression("\"[^\"]*\""), stringFormat));
    }

protected:
    void highlightBlock(const QString &text) override {
        for (const auto &rule : highlightingRules) {
            QRegularExpressionMatchIterator i = rule.first.globalMatch(text);
            while (i.hasNext()) {
                QRegularExpressionMatch match = i.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.second);
            }
        }
    }

private:
    QVector<QPair<QRegularExpression, QTextCharFormat>> highlightingRules;

    bool isWordLike(const QString &s) const {
        static QRegularExpression re("^[A-Za-z_][A-Za-z0-9_]*$");
        return re.match(s).hasMatch();
    }
};

class PythonHighlighter : public QSyntaxHighlighter {
public:
    PythonHighlighter(QTextDocument *parent = nullptr) : QSyntaxHighlighter(parent) {
        QTextCharFormat keywordFormat;
        keywordFormat.setForeground(Qt::black);
        keywordFormat.setFontWeight(QFont::Bold);

        QTextCharFormat commentFormat;
        commentFormat.setForeground(QColor("gray"));

        QTextCharFormat PYFormat;
        PYFormat.setForeground(Qt::blue);
        PYFormat.setFontWeight(QFont::Bold);

        QTextCharFormat ReportFormat;
        ReportFormat.setForeground(Qt::red);
        ReportFormat.setFontWeight(QFont::Bold);

        QTextCharFormat stringFormat;
        stringFormat.setForeground(QColor("green"));

        QStringList PYkeywords = {"and", "as", "assert", "break", "class", "continue", "def",
                                 "del", "elif", "else", "except", "exec", "finally", "for",
                                 "from", "global", "if", "import", "in", "is", "lambda",
                                 "not", "or", "pass", "print", "raise", "return", "try",
                                  "while", "with", "yield"};
        QStringList Reportkeywords = {"report"};
        QStringList Mainkeywords = {"le","div","**","and","eq","==","ge",">=","<=","gt",">","then","else","lt","<","ne","!=","not","or","xor","abs",
                                 "accucapacityflux","accucapacitystate","accuflux","accufractionflux","accufractionstate","accuthresholdflux","accuthresholdstate",
                                 "accutriggerflux","accutriggerstate","accutraveltimestate","accutraveltimeflux","accutraveltimeremoved","accutraveltimefractionstate",
                                 "accutraveltimefractionflux","accutraveltimefractionremoved","acos","argorder","argorderwithid","argorderaddarealimited","argorderwithidaddarealimited",
                                 "argorderarealimited","argorderwithidarealimited","areaarea","areaaverage","areadiversity","areamajority","areamaximum","areaminimum",
                                 "areanormal","areaorder","areatotal","areauniform","asin","aspect","atan","boolean","catchment","catchmenttotal","cellarea",
                                 "celllength","clump","cos","cover","defined","directional","downstream","downstreamdist","dynwavestate","dynwaveflux",
                                 "dynamicwave","exp","extentofview","fac","idiv","if","influencesimplegauss","inversedistance","kinematic","kinwavestate",
                                 "kinwaveflux","ldd","lddcreate","lddcreatedem","ldddist","lddmask","lddrepair","ln","log10","lookupboolean","lookupnominal",
                                 "lookupordinal","lookupscalar","lookupdirectional","lookupldd","lookuplinear","lookupmapstack","maparea","mapmaximum","mapminimum","mapnormal","maptotal","mapuniform","markwhilesumle",
                                 "markwhilesumge","max","min","mod","nodirection","nominal","normal","order","ordinal","path","pit","plancurv","pred","profcurv","repeat","rounddown","roundoff","roundup",
                                 "scalar","shift","shift0","sin","slope","slopelength","spread","spreadldd","spreadlddzone","spreadmax","spreadzone","sqr","sqrt","streamorder","subcatchment",
                                 "succ","tan","time","timeinputboolean","timeinputnominal","timeinputordinal","timeinputdirectional","timeinputscalar","timeinputldd","timeinput","timeinputmodulo","timeinputsparse",
                                 "timeoutput","timeslice","uniform","uniqueid","until","upstream","view","windowtotal","windowaverage","windowdiversity","windowhighpass","windowmajority"
                                 "windowmaximum","windowminimum","windowtotal","xcoordinate","ycoordinate"
        };

        highlightingRules.append(QPair<QRegularExpression, QTextCharFormat>(
                            QRegularExpression("#[^\n]*"), commentFormat));
        highlightingRules.append(QPair<QRegularExpression, QTextCharFormat>(
                            QRegularExpression("'[^']*'"), stringFormat));
        highlightingRules.append(QPair<QRegularExpression, QTextCharFormat>(
                            QRegularExpression("\"[^\"]*\""), stringFormat));
    }
    bool dosyntax;
protected:
    void highlightBlock(const QString &text) override {
        for (const auto &rule : highlightingRules) {
            QRegularExpression expression(rule.first);
            QRegularExpressionMatchIterator i = expression.globalMatch(text);
            while (i.hasNext()) {
                QRegularExpressionMatch match = i.next();
                int index = match.capturedStart();
                int length = match.capturedLength();
                setFormat(index, length, rule.second);
            }
        }
    }

private:
    QVector<QPair<QRegularExpression, QTextCharFormat>> highlightingRules;
    bool isValidKeyword(const QString &keyword) {
        QRegularExpression regex("\\b" + QRegularExpression::escape(keyword) + "\\b");
        return regex.isValid();
    }
};

#endif // NUTSHELLHIGHLIGHTER_H
