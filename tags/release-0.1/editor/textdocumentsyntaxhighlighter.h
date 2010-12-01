#ifndef TEXTDOCUMENTSYNTAXHIGHLIGHTER_H
#define TEXTDOCUMENTSYNTAXHIGHLIGHTER_H

#include "syntaxhighlighter.h"
#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtCore/QSet>

class TextDocumentSyntaxHighlighter : public SyntaxHighlighter
{
    Q_OBJECT

public:
    explicit TextDocumentSyntaxHighlighter(QObject *parent, OperationMode operationMode = ScriptReportTemplateMode);
    explicit TextDocumentSyntaxHighlighter(QTextDocument *parent, OperationMode operationMode = ScriptReportTemplateMode);
    explicit TextDocumentSyntaxHighlighter(QTextEdit *parent, OperationMode operationMode = ScriptReportTemplateMode);

protected:
    void process(const QString &text, Token tokens[], int &customState, bool &reprocessFutureToken);
    void initHighlighting(const QString &text, const int previousState, Token tokens[], int &customState);
    int getFinalState(const QString &text, Token tokens[], int customState);

private:
    void init();

private:
    QHash<TokenType, QTextCharFormat> m_formats;
    QHash<TokenType, QTextCharFormat> m_backgrounds;
    QTextCharFormat m_srtValidSectionFormat;


    QHash<QString, QSet<QString> > m_tagAttributes;
    QTextCharFormat m_validTagAttributteFormat;

    QSet<QString> m_validTags;
    QStringList m_tagStatus;
    QTextCharFormat m_validTagFormat;

    QList<QRegExp> m_jsKeywordPatterns;
    QTextCharFormat m_jsKeywordFormat;
    QList<QRegExp> m_cssKeywordPatterns;
    QTextCharFormat m_cssKeywordFormat;

};

#endif // TEXTDOCUMENTSYNTAXHIGHLIGHTER_H
