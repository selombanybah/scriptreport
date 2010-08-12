#ifndef TEXTSTREAMOBJECT_H
#define TEXTSTREAMOBJECT_H

#include "scriptreportengine_global.h"

#include <QtCore/QObject>
#include <QtCore/QTextStream>

class SCRIPTREPORTENGINE_EXPORT TextStreamObject : public QObject
{
Q_OBJECT
public:
    TextStreamObject(QString name, QIODevice::OpenMode defaultStreamMode, QObject* parent = 0);
    ~TextStreamObject();

    void reset();

    QTextStream* stream();
    void setStream(QTextStream* textStream, bool forDelete = false);

    QString text();
    void setText(QString text);

    QString name();
    QIODevice::OpenMode defaultStreamMode();
    bool isDeleteStreamEmabled();

private:
    QString m_streamText;
    QString m_streamName;
    QIODevice::OpenMode m_streamMode;
    QTextStream* m_stream;
    bool m_deleteStream;
};

#endif // TEXTSTREAMOBJECT_H
