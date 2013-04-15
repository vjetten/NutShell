/*
 * NutShellHighlighter
 * v 1.x
 * Syntax highlighting for PCRaster, adapted from highlighter example of Qt
 * Author: VJ 121002
 */

#include "nutshellqt.h"
//! Synax highlighter rules for pcraster operations and sections
//---------------------------------------------------------------
Highlighter::Highlighter(QTextDocument *parent)
   : QSyntaxHighlighter(parent)
{
   dosyntax = true;

   HighlightingRule rule;

   //strings
   quotationFormat.setForeground(Qt::darkGreen);
   rule.pattern = QRegExp("\".*\"");
   rule.format = quotationFormat;
   highlightingRules.append(rule);

   //arg replacement
   quotationFormat.setForeground(Qt::darkGreen);
   keywordFormat.setFontWeight(QFont::Black);
   rule.pattern = QRegExp("\\$[A-Za-z0-9\\{\\}_]+");
   rule.format = quotationFormat;
   highlightingRules.append(rule);

   // report
   keywordFormat.setForeground(Qt::red);
   keywordFormat.setFontWeight(QFont::Bold);
   QStringList reportPatterns;
   reportPatterns  << "\\breport\\b";
   foreach (const QString &pattern, reportPatterns) {
      rule.pattern = QRegExp(pattern);
      rule.format = keywordFormat;
      highlightingRules.append(rule);
   }

   //sections
   keywordFormat.setBackground(QColor(132,132,132,255));
   //keywordFormat.setBackground(QColor(0,110,161,255));
   keywordFormat.setForeground(QColor(Qt::yellow));//.lighter(140));//QColor(255,255,65,255));
   keywordFormat.setFontWeight(QFont::Bold);
   QStringList sectionPatterns;
   sectionPatterns  << "\\bbinding\\b"
                    << "\\binitial\\b"
                    << "\\btimer\\b"
                    << "\\bdynamic\\b"
                    << "\\bareamap\\b";
   foreach (const QString &pattern, sectionPatterns) {
      rule.pattern = QRegExp(pattern);
      rule.format = keywordFormat;
      highlightingRules.append(rule);
   }
   keywordFormat.clearBackground();

   keywordFormat.setForeground(QColor(80,130,190,255));
   keywordFormat.setFontWeight(QFont::Bold);
   QStringList loopPatterns;
   loopPatterns  << "\\brepeat\\b"
                    << "\\buntil\\b";
   foreach (const QString &pattern, loopPatterns) {
      rule.pattern = QRegExp(pattern);
      rule.format = keywordFormat;
      highlightingRules.append(rule);
   }
   keywordFormat.clearBackground();


   // all pcrcalc functions
   //keywordFormat.setForeground(Qt::darkBlue);
   //keywordFormat.setFontWeight(QFont::Black);
   keywordFormat.setForeground(Qt::black);
   keywordFormat.setFontWeight(QFont::Bold);
   QStringList keywordPatterns;
   // PCRaster function name followed by 0 or more whitespaces and a '('
   // checked against omine operations list 02 10 2012
   keywordPatterns  <<
                       "\\ble\\b"<<
                       "\\b<=\\b"<<
                       "\\bdiv\\b"<<
                       "\\b**\\b"<<
                       "\\band\\b"<<
                       "\\beq\\b"<<
                       "\\b==\\b"<<
                       "\\bge\\b"<<
                       "\\b>=\\b"<<
                       "\\bgt\\b"<<
                       "\\b>\\b"<<
                       "\\bthen\\b"<<
                       "\\belse\\b"<<
                       "\\blt\\b"<<
                       "\\b<\\b"<<
                       "\\bne\\b"<<
                       "\\b!=\\b"<<
                       "\\bnot\\b"<<
                       "\\bor\\b"<<
                       "\\bxor\\b"<<
                       "\\babs+\\s*(?=\\()"<<
                       "\\baccucapacityflux+\\s*(?=\\()"<<
                       "\\baccucapacitystate+\\s*(?=\\()"<<
                       "\\baccuflux+\\s*(?=\\()"<<
                       "\\baccufractionflux+\\s*(?=\\()"<<
                       "\\baccufractionstate+\\s*(?=\\()"<<
                       "\\baccuthresholdflux+\\s*(?=\\()"<<
                       "\\baccuthresholdstate+\\s*(?=\\()"<<
                       "\\baccutriggerflux+\\s*(?=\\()"<<
                       "\\baccutriggerstate+\\s*(?=\\()"<<
                       "\\baccutraveltimestate+\\s*(?=\\()"<<
                       "\\baccutraveltimeflux+\\s*(?=\\()"<<
                       "\\baccutraveltimeremoved+\\s*(?=\\()"<<
                       "\\baccutraveltimefractionstate+\\s*(?=\\()"<<
                       "\\baccutraveltimefractionflux+\\s*(?=\\()"<<
                       "\\baccutraveltimefractionremoved+\\s*(?=\\()"<<
                       "\\bacos+\\s*(?=\\()"<<
                       "\\bargorder+\\s*(?=\\()"<<
                       "\\bargorderwithid+\\s*(?=\\()"<<
                       "\\bargorderaddarealimited+\\s*(?=\\()"<<
                       "\\bargorderwithidaddarealimited+\\s*(?=\\()"<<
                       "\\bargorderarealimited+\\s*(?=\\()"<<
                       "\\bargorderwithidarealimited+\\s*(?=\\()"<<
                       "\\bareaarea+\\s*(?=\\()"<<
                       "\\bareaaverage+\\s*(?=\\()"<<
                       "\\bareadiversity+\\s*(?=\\()"<<
                       "\\bareamajority+\\s*(?=\\()"<<
                       "\\bareamaximum+\\s*(?=\\()"<<
                       "\\bareaminimum+\\s*(?=\\()"<<
                       "\\bareanormal+\\s*(?=\\()"<<
                       "\\bareaorder+\\s*(?=\\()"<<
                       "\\bareatotal+\\s*(?=\\()"<<
                       "\\bareauniform+\\s*(?=\\()"<<
                       "\\basin+\\s*(?=\\()"<<
                       "\\baspect+\\s*(?=\\()"<<
                       "\\batan+\\s*(?=\\()"<<
                       "\\bboolean+\\s*(?=\\()"<<
                       "\\bcatchment+\\s*(?=\\()"<<
                       "\\bcatchmenttotal+\\s*(?=\\()"<<
                       "\\bcellarea+\\s*(?=\\()"<<
                       "\\bcelllength+\\s*(?=\\()"<<
                       "\\bclump+\\s*(?=\\()"<<
                       "\\bcos+\\s*(?=\\()"<<
                       "\\bcover+\\s*(?=\\()"<<
                       "\\bdefined+\\s*(?=\\()"<<
                       "\\bdirectional+\\s*(?=\\()"<<
                       "\\bdownstream+\\s*(?=\\()"<<
                       "\\bdownstreamdist+\\s*(?=\\()"<<
                       "\\bdynwavestate+\\s*(?=\\()"<<
                       "\\bdynwaveflux+\\s*(?=\\()"<<
                       "\\bdynamicwave+\\s*(?=\\()"<<
                       "\\bexp+\\s*(?=\\()"<<
                       "\\bextentofview+\\s*(?=\\()"<<
                       "\\bfac+\\s*(?=\\()"<<
                       "\\bidiv+\\s*(?=\\()"<<
                       "\\bif+\\s*(?=\\()"<<
                       "\\binfluencesimplegauss+\\s*(?=\\()"<<
                       "\\binversedistance+\\s*(?=\\()"<<
                       "\\bkinematic+\\s*(?=\\()"<<
                       "\\bkinwavestate+\\s*(?=\\()"<<
                       "\\bkinwaveflux+\\s*(?=\\()"<<
                       "\\bldd+\\s*(?=\\()"<<
                       "\\blddcreate+\\s*(?=\\()"<<
                       "\\blddcreatedem+\\s*(?=\\()"<<
                       "\\bldddist+\\s*(?=\\()"<<
                       "\\blddmask+\\s*(?=\\()"<<
                       "\\blddrepair+\\s*(?=\\()"<<
                       "\\bln+\\s*(?=\\()"<<
                       "\\blog10+\\s*(?=\\()"<<
                       "\\blookupboolean+\\s*(?=\\()"<<
                       "\\blookupnominal+\\s*(?=\\()"<<
                       "\\blookupordinal+\\s*(?=\\()"<<
                       "\\blookupscalar+\\s*(?=\\()"<<
                       "\\blookupdirectional+\\s*(?=\\()"<<
                       "\\blookupldd+\\s*(?=\\()"<<
                       "\\blookuplinear+\\s*(?=\\()"<<
                       "\\blookupmapstack+\\s*(?=\\()"<<
                       "\\bmaparea+\\s*(?=\\()"<<
                       "\\bmapmaximum+\\s*(?=\\()"<<
                       "\\bmapminimum+\\s*(?=\\()"<<
                       "\\bmapnormal+\\s*(?=\\()"<<
                       "\\bmaptotal+\\s*(?=\\()"<<
                       "\\bmapuniform+\\s*(?=\\()"<<
                       "\\bmarkwhilesumle+\\s*(?=\\()"<<
                       "\\bmarkwhilesumge+\\s*(?=\\()"<<
                       "\\bmax+\\s*(?=\\()"<<
                       "\\bmin+\\s*(?=\\()"<<
                       "\\bmod+\\s*(?=\\()"<<
                       "\\bnodirection+\\s*(?=\\()"<<
                       "\\bnominal+\\s*(?=\\()"<<
                       "\\bnormal+\\s*(?=\\()"<<
                       "\\border+\\s*(?=\\()"<<
                       "\\bordinal+\\s*(?=\\()"<<
                       "\\bpath+\\s*(?=\\()"<<
                       "\\bpit+\\s*(?=\\()"<<
                       "\\bplancurv+\\s*(?=\\()"<<
                       "\\bpred+\\s*(?=\\()"<<
                       "\\bprofcurv+\\s*(?=\\()"<<
                       //"\\brepeat+\\s*(?=\\()"<<
                       "\\brounddown+\\s*(?=\\()"<<
                       "\\broundoff+\\s*(?=\\()"<<
                       "\\broundup+\\s*(?=\\()"<<
                       "\\bscalar+\\s*(?=\\()"<<
                       "\\bshift+\\s*(?=\\()"<<
                       "\\bshift0+\\s*(?=\\()"<<
                       "\\bsin+\\s*(?=\\()"<<
                       "\\bslope+\\s*(?=\\()"<<
                       "\\bslopelength+\\s*(?=\\()"<<
                       "\\bspread+\\s*(?=\\()"<<
                       "\\bspreadldd+\\s*(?=\\()"<<
                       "\\bspreadlddzone+\\s*(?=\\()"<<
                       "\\bspreadmax+\\s*(?=\\()"<<
                       "\\bspreadzone+\\s*(?=\\()"<<
                       "\\bsqr+\\s*(?=\\()"<<
                       "\\bsqrt+\\s*(?=\\()"<<
                       "\\bstreamorder+\\s*(?=\\()"<<
                       "\\bsubcatchment+\\s*(?=\\()"<<
                       "\\bsucc+\\s*(?=\\()"<<
                       "\\btan+\\s*(?=\\()"<<
                       "\\btime+\\s*(?=\\()"<<
                       "\\btimeinputboolean+\\s*(?=\\()"<<
                       "\\btimeinputnominal+\\s*(?=\\()"<<
                       "\\btimeinputordinal+\\s*(?=\\()"<<
                       "\\btimeinputdirectional+\\s*(?=\\()"<<
                       "\\btimeinputscalar+\\s*(?=\\()"<<
                       "\\btimeinputldd+\\s*(?=\\()"<<
                       "\\btimeinput+\\s*(?=\\()"<<
                       "\\btimeinputmodulo+\\s*(?=\\()"<<
                       "\\btimeinputsparse+\\s*(?=\\()"<<
                       "\\btimeoutput+\\s*(?=\\()"<<
                       "\\btimeslice+\\s*(?=\\()"<<
                       "\\buniform+\\s*(?=\\()"<<
                       "\\buniqueid+\\s*(?=\\()"<<
                       //"\\buntil+\\s*(?=\\()"<<
                       "\\bupstream+\\s*(?=\\()"<<
                       "\\bview+\\s*(?=\\()"<<
                       "\\bwindow4total+\\s*(?=\\()"<<
                       "\\bwindowaverage+\\s*(?=\\()"<<
                       "\\bwindowdiversity+\\s*(?=\\()"<<
                       "\\bwindowhighpass+\\s*(?=\\()"<<
                       "\\bwindowmajority+\\s*(?=\\()"<<
                       "\\bwindowmaximum+\\s*(?=\\()"<<
                       "\\bwindowminimum+\\s*(?=\\()"<<
                       "\\bwindowtotal+\\s*(?=\\()"<<
                       "\\bxcoordinate+\\s*(?=\\()"<<
                       "\\bycoordinate+\\s*(?=\\()";

   foreach (const QString &pattern, keywordPatterns) {
      rule.pattern = QRegExp(pattern);
      rule.format = keywordFormat;
      highlightingRules.append(rule);
   }

   //comments last else keywords in comments are highlighted
   singleLineCommentFormat.setForeground(QColor(115,115,115,255));
   singleLineCommentFormat.setFontItalic(true);
   rule.pattern = QRegExp("#[^\n]*");
   rule.format = singleLineCommentFormat;
   highlightingRules.append(rule);

   // global option inline
   singleLineCommentFormat.setForeground(Qt::darkMagenta);
   singleLineCommentFormat.setFontWeight(QFont::Black);
   singleLineCommentFormat.setFontItalic(false);
   rule.pattern = QRegExp("#! [^\n]*");
   rule.format = singleLineCommentFormat;
   highlightingRules.append(rule);

}
//---------------------------------------------------------------
void Highlighter::highlightBlock(const QString &text)
{
   if (!dosyntax)
      return;

   foreach (const HighlightingRule &rule, highlightingRules) {
      QRegExp expression(rule.pattern);
      int index = expression.indexIn(text);
      while (index >= 0) {
         int length = expression.matchedLength();
         setFormat(index, length, rule.format);
         index = expression.indexIn(text, index + length);
      }
   }
}
//---------------------------------------------------------------

