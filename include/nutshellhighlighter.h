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
    //HighlightingRule rule;

    QPair<QRegularExpression, QTextCharFormat> rule;
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

//        QStringList Mainkeywords = {
        const QString Mainkeywords[] = {
            // "le","div","**","and","eq","==","ge",">=","<=","gt",">","then","else","lt","<","ne","!=","not","or","xor","abs",
            // "accucapacityflux","accucapacitystate","accuflux","accufractionflux","accufractionstate","accuthresholdflux","accuthresholdstate",
            // "accutriggerflux","accutriggerstate","accutraveltimestate","accutraveltimeflux","accutraveltimeremoved","accutraveltimefractionstate",
            // "accutraveltimefractionflux","accutraveltimefractionremoved","acos","argorder","argorderwithid","argorderaddarealimited","argorderwithidaddarealimited",
            // "argorderarealimited","argorderwithidarealimited","areaarea","areaaverage","areadiversity","areamajority","areamaximum","areaminimum",
            // "areanormal","areaorder","areatotal","areauniform","asin","aspect","atan","boolean","catchment","catchmenttotal","cellarea",
            // "celllength","clump","cos","cover","defined","directional","downstream","downstreamdist","dynwavestate","dynwaveflux",
            // "dynamicwave","exp","extentofview","fac","idiv","if","influencesimplegauss","inversedistance","kinematic","kinwavestate",
            // "kinwaveflux","ldd","lddcreate","lddcreatedem","ldddist","lddmask","lddrepair","ln","log10","lookupboolean","lookupnominal",
            // "lookupordinal","lookupscalar","lookupdirectional","lookupldd","lookuplinear","lookupmapstack","maparea","mapmaximum","mapminimum","mapnormal","maptotal","mapuniform","markwhilesumle",
            // "markwhilesumge","max","min","mod","nodirection","nominal","normal","order","ordinal","path","pit","plancurv","pred","profcurv","repeat","rounddown","roundoff","roundup",
            // "scalar","shift","shift0","sin","slope","slopelength","spread","spreadldd","spreadlddzone","spreadmax","spreadzone","sqr","sqrt","streamorder","subcatchment",
            // "succ","tan","time","timeinputboolean","timeinputnominal","timeinputordinal","timeinputdirectional","timeinputscalar","timeinputldd","timeinput","timeinputmodulo","timeinputsparse",
            // "timeoutput","timeslice","uniform","uniqueid","until","upstream","view","windowtotal","windowaverage","windowdiversity","windowhighpass","windowmajority"
            // "windowmaximum","windowminimum","windowtotal","xcoordinate","ycoordinate"
            QStringLiteral("\\ble\\b"),
            QStringLiteral("\\bdiv\\b"),
            QStringLiteral("\\b**\\b"),
            QStringLiteral("\\band\\b"),
            QStringLiteral("\\beq\\b"),
            QStringLiteral("\\b==\\b"),
            QStringLiteral("\\bge\\b"),
            QStringLiteral("\\b>=\\b"),
            QStringLiteral("\\b<=\\b"),
            QStringLiteral("\\bgt\\b"),
            QStringLiteral("\\b>\\b"),
            QStringLiteral("\\bthen\\b"),
            QStringLiteral("\\belse\\b"),
            QStringLiteral("\\blt\\b"),
            QStringLiteral("\\b<\\b"),
            QStringLiteral("\\bne\\b"),
            QStringLiteral("\\b!=\\b"),
            QStringLiteral("\\bnot\\b"),
            QStringLiteral("\\bor\\b"),
            QStringLiteral("\\bxor\\b"),
            QStringLiteral("\\babs\\b"),
            QStringLiteral("\\baccucapacityflux\\b"),
            QStringLiteral("\\baccucapacitystate\\b"),
            QStringLiteral("\\baccuflux\\b"),
            QStringLiteral("\\baccufractionflux\\b"),
            QStringLiteral("\\baccufractionstate\\b"),
            QStringLiteral("\\baccuthresholdflux\\b"),
            QStringLiteral("\\baccuthresholdstate\\b"),
            QStringLiteral("\\baccutriggerflux\\b"),
            QStringLiteral("\\baccutriggerstate\\b"),
            QStringLiteral("\\baccutraveltimestate\\b"),
            QStringLiteral("\\baccutraveltimeflux\\b"),
            QStringLiteral("\\baccutraveltimeremoved\\b"),
            QStringLiteral("\\baccutraveltimefractionstate\\b"),
            QStringLiteral("\\baccutraveltimefractionflux\\b"),
            QStringLiteral("\\baccutraveltimefractionremoved\\b"),
            QStringLiteral("\\bacos\\b"),
            QStringLiteral("\\bargorder\\b"),
            QStringLiteral("\\bargorderwithid\\b"),
            QStringLiteral("\\bargorderaddarealimited\\b"),
            QStringLiteral("\\bargorderwithidaddarealimited\\b"),
            QStringLiteral("\\bargorderarealimited\\b"),
            QStringLiteral("\\bargorderwithidarealimited\\b"),
            QStringLiteral("\\bareaarea\\b"),
            QStringLiteral("\\bareaaverage\\b"),
            QStringLiteral("\\bareadiversity\\b"),
            QStringLiteral("\\bareamajority\\b"),
            QStringLiteral("\\bareamaximum\\b"),
            QStringLiteral("\\bareaminimum\\b"),
            QStringLiteral("\\bareanormal\\b"),
            QStringLiteral("\\bareaorder\\b"),
            QStringLiteral("\\bareatotal\\b"),
            QStringLiteral("\\bareauniform\\b"),
            QStringLiteral("\\basin\\b"),
            QStringLiteral("\\baspect\\b"),
            QStringLiteral("\\batan\\b"),
            QStringLiteral("\\bboolean\\b"),
            QStringLiteral("\\bcatchment\\b"),
            QStringLiteral("\\bcatchmenttotal\\b"),
            QStringLiteral("\\bcellarea\\b"),
            QStringLiteral("\\bcelllength\\b"),
            QStringLiteral("\\bclump\\b"),
            QStringLiteral("\\bcos\\b"),
            QStringLiteral("\\bcover\\b"),
            QStringLiteral("\\bdefined\\b"),
            QStringLiteral("\\bdirectional\\b"),
            QStringLiteral("\\bdownstream\\b"),
            QStringLiteral("\\bdownstreamdist\\b"),
            QStringLiteral("\\bdynwavestate\\b"),
            QStringLiteral("\\bdynwaveflux\\b"),
            QStringLiteral("\\bdynamicwave\\b"),
            QStringLiteral("\\bexp\\b"),
            QStringLiteral("\\bextentofview\\b"),
            QStringLiteral("\\bfac\\b"),
            QStringLiteral("\\bidiv\\b"),
            QStringLiteral("\\bif\\b"),
            QStringLiteral("\\binfluencesimplegauss\\b"),
            QStringLiteral("\\binversedistance\\b"),
            QStringLiteral("\\bkinematic\\b"),
            QStringLiteral("\\bkinwavestate\\b"),
            QStringLiteral("\\bkinwaveflux\\b"),
            QStringLiteral("\\bldd\\b"),
            QStringLiteral("\\blddcreate\\b"),
            QStringLiteral("\\blddcreatedem\\b"),
            QStringLiteral("\\bldddist\\b"),
            QStringLiteral("\\blddmask\\b"),
            QStringLiteral("\\blddrepair\\b"),
            QStringLiteral("\\bln\\b"),
            QStringLiteral("\\blog10\\b"),
            QStringLiteral("\\blookupboolean\\b"),
            QStringLiteral("\\blookupnominal\\b"),
            QStringLiteral("\\blookupordinal\\b"),
            QStringLiteral("\\blookupscalar\\b"),
            QStringLiteral("\\blookupdirectional\\b"),
            QStringLiteral("\\blookupldd\\b"),
            QStringLiteral("\\blookuplinear\\b"),
            QStringLiteral("\\blookupmapstack\\b"),
            QStringLiteral("\\bmaparea\\b"),
            QStringLiteral("\\bmapmaximum\\b"),
            QStringLiteral("\\bmapminimum\\b"),
            QStringLiteral("\\bmapnormal\\b"),
            QStringLiteral("\\bmaptotal\\b"),
            QStringLiteral("\\bmapuniform\\b"),
            QStringLiteral("\\bmarkwhilesumle\\b"),
            QStringLiteral("\\bmarkwhilesumge\\b"),
            QStringLiteral("\\bmax\\b"),
            QStringLiteral("\\bmin\\b"),
            QStringLiteral("\\bmod\\b"),
            QStringLiteral("\\bnodirection\\b"),
            QStringLiteral("\\bnominal\\b"),
            QStringLiteral("\\bnormal\\b"),
            QStringLiteral("\\border\\b"),
            QStringLiteral("\\bordinal\\b"),
            QStringLiteral("\\bpath\\b"),
            QStringLiteral("\\bpit\\b"),
            QStringLiteral("\\bplancurv\\b"),
            QStringLiteral("\\bpred\\b"),
            QStringLiteral("\\bprofcurv\\b"),
            QStringLiteral("\\brepeat\\b"),
            QStringLiteral("\\brounddown\\b"),
            QStringLiteral("\\broundoff\\b"),
            QStringLiteral("\\broundup\\b"),
            QStringLiteral("\\bscalar\\b"),
            QStringLiteral("\\bshift\\b"),
            QStringLiteral("\\bshift0\\b"),
            QStringLiteral("\\bsin\\b"),
            QStringLiteral("\\bslope\\b"),
            QStringLiteral("\\bslopelength\\b"),
            QStringLiteral("\\bspread\\b"),
            QStringLiteral("\\bspreadldd\\b"),
            QStringLiteral("\\bspreadlddzone\\b"),
            QStringLiteral("\\bspreadmax\\b"),
            QStringLiteral("\\bspreadzone\\b"),
            QStringLiteral("\\bsqr\\b"),
            QStringLiteral("\\bsqrt\\b"),
            QStringLiteral("\\bstreamorder\\b"),
            QStringLiteral("\\bsubcatchment\\b"),
            QStringLiteral("\\bsucc\\b"),
            QStringLiteral("\\btan\\b"),
            QStringLiteral("\\btime\\b"),
            QStringLiteral("\\btimeinputboolean\\b"),
            QStringLiteral("\\btimeinputnominal\\b"),
            QStringLiteral("\\btimeinputordinal\\b"),
            QStringLiteral("\\btimeinputdirectional\\b"),
            QStringLiteral("\\btimeinputscalar\\b"),
            QStringLiteral("\\btimeinputldd\\b"),
            QStringLiteral("\\btimeinput\\b"),
            QStringLiteral("\\btimeinputmodulo\\b"),
            QStringLiteral("\\btimeinputsparse\\b"),
            QStringLiteral("\\btimeoutput\\b"),
            QStringLiteral("\\btimeslice\\b"),
            QStringLiteral("\\buniform\\b"),
            QStringLiteral("\\buniqueid\\b"),
            QStringLiteral("\\buntil\\b"),
            QStringLiteral("\\bupstream\\b"),
            QStringLiteral("\\bview\\b"),
            QStringLiteral("\\bwindowtotal\\b"),
            QStringLiteral("\\bwindowaverage\\b"),
            QStringLiteral("\\bwindowdiversity\\b"),
            QStringLiteral("\\bwindowhighpass\\b"),
            QStringLiteral("\\bwindowmajority\\b"),
            QStringLiteral("\\bwindowmaximum\\b"),
            QStringLiteral("\\bwindowminimum\\b"),
            QStringLiteral("\\bwindowtotal\\b"),
            QStringLiteral("\\bxcoordinate\\b"),
            QStringLiteral("\\bycoordinate\\b")
        };
        // for (const QString &keyword : Mainkeywords) {
        //     rule.pattern = QRegularExpression(QStringLiteral("\\b" + keyword + "\\b"));
        //     rule.format = KeywordFormat;
        //     highlightingRules.append(rule);
        // }
        // for (const QString &keyword : PCRkeywords) {
        //     rule.pattern = QRegularExpression(QStringLiteral("\\b" + keyword + "\\b"));
        //     rule.format = PCRFormat;
        //     highlightingRules.append(rule);
        // }
        // for (const QString &keyword : Reportkeywords) {
        //     rule.pattern = QRegularExpression(QStringLiteral("\\b" + keyword + "\\b"));
        //     rule.format = ReportFormat;
        //     highlightingRules.append(rule);
        // }
        for (const QString &keyword : Mainkeywords) {
            if (isValidKeyword(keyword)) {
                QPair<QRegularExpression, QTextCharFormat> ppp;
                ppp.first = QRegularExpression(keyword);
                ppp.second = keywordFormat;
                highlightingRules.append(ppp);
           }
         }
        for (const QString &keyword : PCRkeywords) {
            QPair<QRegularExpression, QTextCharFormat> ppp;
            ppp.first = QRegularExpression("\\b" + keyword + "\\b");
            ppp.second = PCRFormat;
            highlightingRules.append(ppp);
        }
        for (const QString &keyword : Reportkeywords) {
            QPair<QRegularExpression, QTextCharFormat> ppp;
            ppp.first = QRegularExpression("\\b" + keyword + "\\b");
            ppp.second = ReportFormat;
            highlightingRules.append(ppp);
        }
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
    // struct HighlightingRule
    // {
    //     QRegularExpression pattern;
    //     QTextCharFormat format;
    // };
    // QList<HighlightingRule> highlightingRules;

    QVector<QPair<QRegularExpression, QTextCharFormat>> highlightingRules;

    bool isValidKeyword(const QString &keyword) {
        QRegularExpression regex("\\b" + QRegularExpression::escape(keyword) + "\\b");
        return regex.isValid();
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

        // for (const QString &keyword : Mainkeywords) {
        //     if (isValidKeyword(keyword)) {
        //         highlightingRules.append(QPair<QRegularExpression, QTextCharFormat>(
        //         QRegularExpression("\\b" + keyword + "\\b"), keywordFormat));
        //     }
        // }
        // for (const QString &keyword : Reportkeywords) {
        //     highlightingRules.append(QPair<QRegularExpression, QTextCharFormat>(
        //         QRegularExpression("\\b" + keyword + "\\b"), ReportFormat));
        // }
        // for (const QString &keyword : PYkeywords) {
        //     highlightingRules.append(QPair<QRegularExpression, QTextCharFormat>(
        //         QRegularExpression("\\b" + keyword + "\\b"), PYFormat));
        // }
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
