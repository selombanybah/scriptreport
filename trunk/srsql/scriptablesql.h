#ifndef SCRIPTABLESQL_H
#define SCRIPTABLESQL_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtScript/QScriptable>
#include <QtCore/QMap>

class QScriptEngine;
class ScriptableDatabase;

class ScriptableSql : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(QStringList connectionNames READ connectionNames)
    Q_PROPERTY(QStringList drivers READ drivers)
    Q_PROPERTY(bool autoThrow READ autoThrow WRITE setAutoThrow)

public:
    explicit ScriptableSql(QObject *parent = 0);
    ~ScriptableSql();

    QStringList connectionNames();
    QStringList drivers();

    bool autoThrow() const;
    void setAutoThrow(bool autoThrow);

    Q_INVOKABLE bool contains(const QString &connectionName = QString());
    Q_INVOKABLE bool isDriverAvailable(const QString &name);
    Q_INVOKABLE void removeDatabase(const QString &connectionName);

    Q_INVOKABLE ScriptableDatabase* addDatabase(const QString &type, const QString &connectionName = QLatin1String(defaultConnection));
    Q_INVOKABLE ScriptableDatabase* cloneDatabase(const ScriptableDatabase &other, const QString &connectionName);
    Q_INVOKABLE ScriptableDatabase* database(const QString &connectionName = QLatin1String(defaultConnection), bool open = true);

private:
    QT_STATIC_CONST char *defaultConnection;
    QMap<QString, ScriptableDatabase*> m_connections;
    bool m_autoThrow;
};

#endif // SCRIPTABLESQL_H
