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
    Q_PROPERTY(QString scriptReportVersion READ scriptReportVersion)
    Q_PROPERTY(QString qtVersion READ qtVersion)

public:
    explicit ScriptableEngine(QObject *parent = 0);

    QStringList availableExtensions();
    QStringList importedExtensions();
    QString scriptReportVersion();
    QString qtVersion();

    Q_INVOKABLE void importExtension(QString name);
    Q_INVOKABLE void installApplicationTranslatorFunctions(const QScriptValue &object = QScriptValue());

private:

};

#endif // SCRIPTABLEENGINE_H
