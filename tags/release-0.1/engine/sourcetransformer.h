#ifndef SOURCETRANSFORMER_H
#define SOURCETRANSFORMER_H

#include <QtCore/QString>
#include <QtCore/QTextStream>

#include "scriptreportengine_global.h"

class SourceTransformerPrivate;

class SCRIPTREPORTENGINE_EXPORT SourceTransformer
{
public:
    explicit SourceTransformer(QTextStream *inputStream = 0, QTextStream *outputStream = 0);
    ~SourceTransformer();

    bool transform();

    QTextStream *inputStream() const;
    void setInputStream(QTextStream *inputStream);
    QTextStream *outputStream() const;
    void setOutputStream(QTextStream *outputStream);

private:
    SourceTransformerPrivate *d;
};

#endif // SOURCETRANSFORMER_H
