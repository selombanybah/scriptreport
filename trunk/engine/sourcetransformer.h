#ifndef SOURCETRANSFORMER_H
#define SOURCETRANSFORMER_H

#include <QtCore/QString>
#include <QtCore/QTextStream>

#include "scriptreportengine_global.h"

class SCRIPTREPORTENGINE_EXPORT SourceTransformer
{
public:
    explicit SourceTransformer(QTextStream *inputStream = 0, QTextStream *outputStream = 0);

    bool transform();

    QTextStream *inputStream() const;
    void setInputStream(QTextStream *inputStream);
    QTextStream *outputStream() const;
    void setOutputStream(QTextStream *outputStream);

private:
    void consume();
    void prepare();
    void writeAndConsume();
    void writeHtmlAndConsume();
    void writeStartHtml();
    void writeEndHtml();
    void write(const QChar &c);
    void write(const QChar &c1, const QChar &c2);
    void write(const QString &s);

    void writeHtmlChar(const QChar &c);

    void readHtml();
    void readScript(bool writeEnd);
    void readInLineScript();
    void readChangeContext();
    void readHtmlComment();
    void ajust();

private:
    QChar current;
    QChar next;
    bool startHtmlWrited;
    QTextStream *in;
    QTextStream *out;
    int inLine;
    int inColumn;
    int outLine;
    int outColumn;
};

#endif // SOURCETRANSFORMER_H
