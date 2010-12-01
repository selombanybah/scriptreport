#ifndef TEXTSTREAMOBJECT_H
#define TEXTSTREAMOBJECT_H

#include "scriptreportengine_global.h"

#include <QtCore/QObject>
#include <QtCore/QTextStream>

class TextStreamObjectPrivate;

class SCRIPTREPORTENGINE_EXPORT TextStreamObject : public QObject
{
Q_OBJECT
public:
    TextStreamObject(QString name, QIODevice::OpenMode defaultStreamMode, QObject* parent = 0);
    ~TextStreamObject();

//    void reset();

    QTextStream* stream() const;
    void setStream(QTextStream* textStream, bool forDelete = false);

    QString text() const;
    void setText(QString text);

    QString name() const;
    QIODevice::OpenMode defaultStreamMode() const;
    bool isDeleteStreamEmabled() const;

private:
    TextStreamObjectPrivate *d;
};

#endif // TEXTSTREAMOBJECT_H
