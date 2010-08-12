#ifndef SCRIPTABLEERROR_H
#define SCRIPTABLEERROR_H

#include <QObject>
#include <QtCore/QMetaType>
#include <QtScript/QScriptable>

class QSqlError;

class ScriptableError : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(QString databaseText READ databaseText)
    Q_PROPERTY(QString driverText READ driverText)
    Q_PROPERTY(int number READ number)
    Q_PROPERTY(QString text READ text)
    Q_PROPERTY(QString type READ type)

public:
    explicit ScriptableError(QSqlError &error, QObject *parent = 0);
    ~ScriptableError();

    QString databaseText() const;
    QString driverText() const;
    int number() const;
    QString text() const;
    QString type() const;
    Q_INVOKABLE QString toString() const;

private:
    QSqlError *m_error;

};

Q_DECLARE_METATYPE(ScriptableError*)

#endif // SCRIPTABLEERROR_H
