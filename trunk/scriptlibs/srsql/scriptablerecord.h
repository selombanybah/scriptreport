#ifndef SCRIPTABLERECORD_H
#define SCRIPTABLERECORD_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QMetaType>
#include <QtCore/QStringList>
#include <QtScript/QScriptable>

class QSqlRecord;

class ScriptableRecord : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(bool isEmpty READ isEmpty)
    Q_PROPERTY(int count READ count)
    Q_PROPERTY(bool autoThrow READ autoThrow WRITE setAutoThrow)

public:
    explicit ScriptableRecord(QSqlRecord &record, bool autoThrow, QObject *parent = 0);
    ~ScriptableRecord();

    Q_INVOKABLE QVariant value(int i) const;
    Q_INVOKABLE QVariant value(const QString& name) const;

    Q_INVOKABLE bool isNull(int i) const;
    Q_INVOKABLE bool isNull(const QString& name) const;

    Q_INVOKABLE int indexOf(const QString &name) const;
    Q_INVOKABLE QString fieldName(int i) const;

//    QSqlField field(int i) const;
//    QSqlField field(const QString &name) const;

    Q_INVOKABLE bool isGenerated(int i) const;
    Q_INVOKABLE bool isGenerated(const QString& name) const;

    bool isEmpty() const;
    Q_INVOKABLE bool contains(const QString& name) const;
    int count() const;

    bool autoThrow() const;
    void setAutoThrow(bool autoThrow);

private:
    QSqlRecord *m_record;
    bool m_autoThrow;
};

Q_DECLARE_METATYPE(ScriptableRecord*)

#endif // SCRIPTABLERECORD_H
