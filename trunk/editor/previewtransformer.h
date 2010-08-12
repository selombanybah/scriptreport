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


    void readHtml();
    void readChangeContext();
    void readComment();

private:
    QChar current;
    QChar next;
    QTextStream *in;
    QTextStream *out;
};


#endif // PREVIEWTRANSFORMER_H
