/*
 * Copyright 2010 and beyond, Juan Luis Paz
 *
 * This file is part of Script Report.
 *
 * Script Report is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Script Report is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Script Report.  If not, see <http://www.gnu.org/licenses/>.
 */

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
