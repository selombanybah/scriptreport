#ifndef SCRIPTABLETRANSLATOR_H
#define SCRIPTABLETRANSLATOR_H

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtCore/QTranslator>
#include <QtScript/QScriptable>

class ScriptableTranslator : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(bool isEmpty READ isEmpty)

public:
    explicit ScriptableTranslator(QTranslator *translator, QObject *parent = 0);
    ~ScriptableTranslator();

    Q_INVOKABLE QString translate(const QString context, const QString sourceText, const QString disambiguation = QString()) const;
    Q_INVOKABLE QString translate(const QString context, const QString sourceText, const QString disambiguation, int n) const;

    bool isEmpty() const;

    QTranslator *translator() const;

    bool autoRemoveApplicationTraslator() const;
    void setAutoRemoveApplicationTraslator(bool autoRemove);
private:
    QTranslator *m_translator;
    bool m_autoRemoveApplicationTraslator;
};

Q_DECLARE_METATYPE(ScriptableTranslator*)

#endif // SCRIPTABLETRANSLATOR_H
