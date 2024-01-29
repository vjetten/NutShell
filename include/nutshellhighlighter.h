#ifndef NUTSHELLHIGHLIGHTER_H
#define NUTSHELLHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

//class Highlighter : public QSyntaxHighlighter
//{
//	Q_OBJECT

//public:
//	Highlighter(QTextDocument *parent = 0);
//	bool dosyntax;

//protected:
//	void highlightBlock(const QString &text);

//private:
//	struct HighlightingRule
//	{
//		QRegExp pattern;
//		QTextCharFormat format;
//	};
//	QVector<HighlightingRule> highlightingRules;

//	QRegExp commentStartExpression;
//	QRegExp commentEndExpression;

//	QTextCharFormat keywordFormat;
//	QTextCharFormat singleLineCommentFormat;
//	QTextCharFormat quotationFormat;

//};


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

        QStringList keywords = {"le","div","**","and","eq","==","ge",">=","<=","gt",">","then","else","lt","<","ne","!=","not","or","xor","abs",
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

        for (const QString &keyword : keywords) {
            highlightingRules.append(QPair<QRegExp, QTextCharFormat>(QRegExp("\\b" + keyword + "\\b"), keywordFormat));
        }
        for (const QString &keyword : PCRkeywords) {
            highlightingRules.append(QPair<QRegExp, QTextCharFormat>(QRegExp("\\b" + keyword + "\\b"), PCRFormat));
        }
        for (const QString &keyword : Reportkeywords) {
            highlightingRules.append(QPair<QRegExp, QTextCharFormat>(QRegExp("\\b" + keyword + "\\b"), ReportFormat));
        }
        highlightingRules.append(QPair<QRegExp, QTextCharFormat>(QRegExp("#[^\n]*"), commentFormat));
        highlightingRules.append(QPair<QRegExp, QTextCharFormat>(QRegExp("'[^']*'"), stringFormat));
        highlightingRules.append(QPair<QRegExp, QTextCharFormat>(QRegExp("\"[^\"]*\""), stringFormat));
    }
    bool dosyntax;
protected:
    void highlightBlock(const QString &text) override {
        for (const auto &rule : highlightingRules) {
            QRegExp expression(rule.first);
            int index = expression.indexIn(text);
            while (index >= 0) {
                int length = expression.matchedLength();
                setFormat(index, length, rule.second);
                index = expression.indexIn(text, index + length);
            }
        }
    }

private:
    QVector<QPair<QRegExp, QTextCharFormat>> highlightingRules;
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
        QStringList keywords = {"le","div","**","and","eq","==","ge",">=","<=","gt",">","then","else","lt","<","ne","!=","not","or","xor","abs",
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

        for (const QString &keyword : keywords) {
            highlightingRules.append(QPair<QRegExp, QTextCharFormat>(QRegExp("\\b" + keyword + "\\b"), keywordFormat));
        }
        for (const QString &keyword : Reportkeywords) {
            highlightingRules.append(QPair<QRegExp, QTextCharFormat>(QRegExp("\\b" + keyword + "\\b"), ReportFormat));
        }
        for (const QString &keyword : PYkeywords) {
            highlightingRules.append(QPair<QRegExp, QTextCharFormat>(QRegExp("\\b" + keyword + "\\b"), PYFormat));
        }
        highlightingRules.append(QPair<QRegExp, QTextCharFormat>(QRegExp("#[^\n]*"), commentFormat));
        highlightingRules.append(QPair<QRegExp, QTextCharFormat>(QRegExp("'[^']*'"), stringFormat));
        highlightingRules.append(QPair<QRegExp, QTextCharFormat>(QRegExp("\"[^\"]*\""), stringFormat));
    }
    bool dosyntax;
protected:
    void highlightBlock(const QString &text) override {
        for (const auto &rule : highlightingRules) {
            QRegExp expression(rule.first);
            int index = expression.indexIn(text);
            while (index >= 0) {
                int length = expression.matchedLength();
                setFormat(index, length, rule.second);
                index = expression.indexIn(text, index + length);
            }
        }
    }

private:
    QVector<QPair<QRegExp, QTextCharFormat>> highlightingRules;
};

#endif // NUTSHELLHIGHLIGHTER_H
