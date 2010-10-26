#include "scriptabletranslator.h"

#include <QtCore/QCoreApplication>

ScriptableTranslator::ScriptableTranslator(QTranslator *translator, QObject *parent) :
    QObject(parent), QScriptable(),
    m_translator(translator),
    m_autoRemoveApplicationTraslator(false)
{
}

ScriptableTranslator::~ScriptableTranslator() {
    if (m_autoRemoveApplicationTraslator) {
        QCoreApplication::removeTranslator(m_translator);
    }
}

QString ScriptableTranslator::translate(const QString context, const QString sourceText, const QString disambiguation) const {
    if (disambiguation.isEmpty()) {
        return m_translator->translate(context.toLatin1().constData(), sourceText.toLatin1().constData());
    } else {
        return m_translator->translate(context.toLatin1().constData(), sourceText.toLatin1().constData(), disambiguation.toLatin1().constData());
    }
}

QString ScriptableTranslator::translate(const QString context, const QString sourceText, const QString disambiguation, int n) const {
    return m_translator->translate(context.toLatin1().constData(), sourceText.toLatin1().constData(), disambiguation.toLatin1().constData(), n);
}

bool ScriptableTranslator::isEmpty() const {
    return m_translator->isEmpty();
}

QTranslator *ScriptableTranslator::translator() const {
    return m_translator;
}

bool ScriptableTranslator::autoRemoveApplicationTraslator() const {
    return m_autoRemoveApplicationTraslator;
}

void ScriptableTranslator::setAutoRemoveApplicationTraslator(bool autoRemove) {
    m_autoRemoveApplicationTraslator = autoRemove;
}
