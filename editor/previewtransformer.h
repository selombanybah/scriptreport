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


    void readHtml(QTextStream *header, QTextStream *content, QTextStream *footer);
    void readChangeContext(QTextStream *header, QTextStream *content, QTextStream *footer);
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
