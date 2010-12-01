#ifndef SCRIPTABLEENGINE_H
#define SCRIPTABLEENGINE_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtScript/QScriptable>
#include <QtScript/QScriptValue>

class ScriptableEngine : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(QStringList availableExtensions READ availableExtensions)
    Q_PROPERTY(QStringList importedExtensions READ importedExtensions)
    Q_PROPERTY(QString qtVersion READ qtVersion)
    Q_PROPERTY(QString srVersion READ srVersion)
    Q_PROPERTY(QStringList arguments READ arguments)

public:
    explicit ScriptableEngine(QObject *parent = 0);

    QStringList availableExtensions() const;
    QStringList importedExtensions() const;
    QString qtVersion() const;
    QString srVersion() const;
    QStringList arguments() const;
    void setArguments(QStringList arguments);

    Q_INVOKABLE void importExtension();

    Q_INVOKABLE QString readFile(QString name);
    Q_INVOKABLE void load();
    Q_INVOKABLE int runCommand();

    Q_INVOKABLE QString getFromEnvironment(QString name);

    Q_INVOKABLE void installApplicationTranslatorFunctions(const QScriptValue &object = QScriptValue());

private:
    QStringList m_arguments;
};

#endif // SCRIPTABLEENGINE_H
