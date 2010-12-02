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

#ifndef PREVIEWTRANSFORMER_H
#define PREVIEWTRANSFORMER_H

#include <QtCore/QString>
#include <QtCore/QTextStream>

class PreviewTransformer
{
public:
    explicit PreviewTransformer(QTextStream *inputStream = 0, QTextStream *outputStream = 0);

    bool transform();

    QTextStream *inputStream() const;
    void setInputStream(QTextStream *inputStream);
    QTextStream *outputStream() const;
    void setOutputStream(QTextStream *outputStream);

private:
    void consume();
    void prepare();
    void writeAndConsume();
    void write(const QChar &c);
    void write(const QChar &c1, const QChar &c2);
    void write(const QString &s);


    void readHtml(QTextStream *headerFirst, QTextStream *header, QTextStream *headerLast,
                  QTextStream *content,
                  QTextStream *footerFirst, QTextStream *footer, QTextStream *footerLast);
    void readChangeContext(QTextStream *headerFirst, QTextStream *header, QTextStream *headerLast,
                           QTextStream *content,
                           QTextStream *footerFirst, QTextStream *footer, QTextStream *footerLast);
    void readComment();
    void readConditional();
    void readConditionalText();
    void readInlineConditional();
    void readInlineConditionalText();

private:
    QChar current;
    QChar next;
    QTextStream *in;
    QTextStream *out;
    QTextStream *currentStream;
};


#endif // PREVIEWTRANSFORMER_H
