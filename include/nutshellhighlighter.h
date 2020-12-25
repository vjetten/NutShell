#ifndef NUTSHELLHIGHLIGHTER_H
#define NUTSHELLHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class Highlighter : public QSyntaxHighlighter
{
	Q_OBJECT

public:
	Highlighter(QTextDocument *parent = 0);
	bool dosyntax;

protected:
	void highlightBlock(const QString &text);

private:
	struct HighlightingRule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};
	QVector<HighlightingRule> highlightingRules;

	QRegExp commentStartExpression;
	QRegExp commentEndExpression;

	QTextCharFormat keywordFormat;
	QTextCharFormat singleLineCommentFormat;
	QTextCharFormat quotationFormat;

};


#endif // NUTSHELLHIGHLIGHTER_H
