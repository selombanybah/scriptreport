#include "textstreamobject.h"

class TextStreamObjectPrivate {
public:
    TextStreamObjectPrivate(QString name, QIODevice::OpenMode defaultStreamMode) :
        streamName(name),
        streamMode(defaultStreamMode),
        stream(0),
        deleteStream(false)
    {
    }

    QString streamText;
    QString streamName;
    QIODevice::OpenMode streamMode;
    QTextStream* stream;
    bool deleteStream;
};

/*!
    \class TextStreamObject
    \brief \bold QObject class for encapsulate a \bold QTextStream.

    The TextStreamObject class is an object for encapsulate and manipulate a text stream. This class allow by default
    to have a text stream to a string, and permit change the stream to another one.
*/

/*!
    \fn TextStreamObject::TextStreamObject(QString name, QIODevice::OpenMode defaultStreamMode, QObject* parent)
     Constructs a Text Stream Object with \a name, stream open mode \a defaultStreamMode and parent object \a parent.
*/
TextStreamObject::TextStreamObject(QString name, QIODevice::OpenMode defaultStreamMode, QObject* parent)
    : QObject(parent),
    d(new TextStreamObjectPrivate(name, defaultStreamMode))
{
}

/*!
    \fn TextStreamObject::~TextStreamObject()
    Destroy the Text Stream Object.
*/
TextStreamObject::~TextStreamObject() {
    if (d->deleteStream) {
        delete d->stream;
    }
    delete d;
}

//void TextStreamObject::reset() {
//    d->streamText.clear();
//    QTextStream *s = new QTextStream(&d->streamText, d->streamMode);
//    setStream(s, true);
//}

/*!
    \fn QTextStream* TextStreamObject::stream() const
    Get the stream object.
*/
QTextStream* TextStreamObject::stream() const {
    if (!d->stream) {
        d->stream = new QTextStream(&d->streamText, d->streamMode);
        d->deleteStream = true;
    }
    return d->stream;
}

/*!
    \fn void TextStreamObject::setStream(QTextStream* textStream, bool forDelete)
    Set the stream object to \a textStream, and \a forDelete idicate if the stream must be
    delete by this object.
*/
void TextStreamObject::setStream(QTextStream* textStream, bool forDelete) {
    if (d->stream == textStream) {
        return;
    }
    if (!d->stream && d->deleteStream) {
        delete d->stream;
    }
    d->stream = textStream;
    d->deleteStream = forDelete;
}

/*!
    \fn QString TextStreamObject::text() const
    Get the text used by the default stream.
*/
QString TextStreamObject::text() const {
    return d->streamText;
}

/*!
    \fn void TextStreamObject::setText(QString text)
    Set the \a text used by the default stream.
*/
void TextStreamObject::setText(QString text) {
    d->streamText = text;
}

/*!
    \fn QString TextStreamObject::name() const
    Get the stream name.
*/
QString TextStreamObject::name() const {
    return d->streamName;
}

/*!
    \fn QIODevice::OpenMode TextStreamObject::defaultStreamMode() const
    Get the default stream open mode.
*/
QIODevice::OpenMode TextStreamObject::defaultStreamMode() const {
    return d->streamMode;
}

/*!
    \fn bool TextStreamObject::isDeleteStreamEmabled() const
    Return if the stream object must be deleted by this object.
*/
bool TextStreamObject::isDeleteStreamEmabled() const {
    return d->deleteStream;
}
