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

TextStreamObject::TextStreamObject(QString name, QIODevice::OpenMode defaultStreamMode, QObject* parent)
    : QObject(parent),
    d(new TextStreamObjectPrivate(name, defaultStreamMode))
{
}

TextStreamObject::~TextStreamObject() {
    if (d->deleteStream) {
        delete d->stream;
    }
    delete d;
}

void TextStreamObject::reset() {
    d->streamText.clear();
    QTextStream *s = new QTextStream(&d->streamText, d->streamMode);
    setStream(s, true);
}

QTextStream* TextStreamObject::stream() const {
    if (!d->stream) {
        d->stream = new QTextStream(&d->streamText, d->streamMode);
        d->deleteStream = true;
    }
    return d->stream;
}

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

QString TextStreamObject::text() const {
    return d->streamText;
}

void TextStreamObject::setText(QString text) {
    d->streamText = text;
}

QString TextStreamObject::name() const {
    return d->streamName;
}

QIODevice::OpenMode TextStreamObject::defaultStreamMode() const {
    return d->streamMode;
}

bool TextStreamObject::isDeleteStreamEmabled() const {
    return d->deleteStream;
}
