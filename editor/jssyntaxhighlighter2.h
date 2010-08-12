#ifndef JSSYNTAXHIGHLIGHTER2_H
#define JSSYNTAXHIGHLIGHTER2_H

#include <QtCore/qglobal.h>
#include <QtGui/qsyntaxhighlighter.h>
#include <QtGui/qtextformat.h>

class JsSyntaxHighlighter2 : public QSyntaxHighlighter
{
public:
    JsSyntaxHighlighter2(QTextDocument *document = 0);
    ~JsSyntaxHighlighter2();

protected:
    void highlightBlock(const QString &text);

private:
    void highlightWord(int currentPos, const QString &buffer);

    enum ScriptFormats {
        ScriptTextFormat, ScriptNumberFormat,
        ScriptStringFormat, ScriptTypeFormat,
        ScriptKeywordFormat, ScriptPreprocessorFormat,
        ScriptLabelFormat, ScriptCommentFormat,
        NumScriptFormats
    };
    QTextCharFormat m_formats[NumScriptFormats];

private:
    Q_DISABLE_COPY(JsSyntaxHighlighter2)
};

#endif // JSSYNTAXHIGHLIGHTER2_H
