#include "textstreamobject.h"

TextStreamObject::TextStreamObject(QString name, QIODevice::OpenMode defaultStreamMode, QObject* parent)
    : QObject(parent),
    m_streamName(name),
    m_streamMode(defaultStreamMode),
    m_stream(0),
    m_deleteStream(false)
{
}

TextStreamObject::~TextStreamObject() {
    if (m_deleteStream) {
        delete m_stream;
    }
}

void TextStreamObject::reset() {
    m_streamText.clear();
    QTextStream *s = new QTextStream(&m_streamText, m_streamMode);
    setStream(s, true);
}

QTextStream* TextStreamObject::stream() {
    if (!m_stream) {
        m_stream = new QTextStream(&m_streamText, m_streamMode);
        m_deleteStream = true;
    }
    return m_stream;
}

void TextStreamObject::setStream(QTextStream* textStream, bool forDelete) {
    if (m_stream == textStream) {
        return;
    }
    if (!m_stream && m_deleteStream) {
        delete m_stream;
    }
    m_stream = textStream;
    m_deleteStream = forDelete;
}

QString TextStreamObject::text() {
    return m_streamText;
}

void TextStreamObject::setText(QString text) {
    m_streamText = text;
}

QString TextStreamObject::name() {
    return m_streamName;
}

QIODevice::OpenMode TextStreamObject::defaultStreamMode() {
    return m_streamMode;
}

bool TextStreamObject::isDeleteStreamEmabled() {
    return m_deleteStream;
}
