#ifndef SCRIPTABLEQUERY_H
#define SCRIPTABLEQUERY_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QMetaType>
#include <QtScript/QScriptable>

class QSqlQuery;

class ScriptableError;
class ScriptableRecord;

class ScriptableQuery : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(bool isValid READ isValid)
    Q_PROPERTY(bool isActive READ isActive)
    Q_PROPERTY(int at READ at)
    Q_PROPERTY(QString lastQuery READ lastQuery)
    Q_PROPERTY(QString executedQuery READ executedQuery)
    Q_PROPERTY(int numRowsAffected READ numRowsAffected)
    Q_PROPERTY(ScriptableError* lastError READ lastError)
    Q_PROPERTY(bool isSelect READ isSelect)
    Q_PROPERTY(int size READ size)
    Q_PROPERTY(int isForwardOnly READ isForwardOnly WRITE setForwardOnly)
    Q_PROPERTY(bool autoThrow READ autoThrow WRITE setAutoThrow)

public:
    explicit ScriptableQuery(QSqlQuery &query, bool autoThrow, QObject *parent = 0);
    ~ScriptableQuery();

    bool isValid() const;
    bool isActive() const;
    Q_INVOKABLE bool isNull(const QString& field) const;
    Q_INVOKABLE bool isNull(int field) const;
    int at() const;
    QString lastQuery() const;
    int numRowsAffected() const;
    ScriptableError* lastError() const;
    bool isSelect() const;
    int size() const;
//    const QSqlDriver* driver() const;
//    const QSqlResult* result() const;
    bool isForwardOnly() const;
    void setForwardOnly(bool forward);

    Q_INVOKABLE ScriptableRecord* record() const;
    Q_INVOKABLE bool exec(const QString& query);
    Q_INVOKABLE QVariant value(const QString& field) const;
    Q_INVOKABLE QVariant value(int i) const;

//    void setNumericalPrecisionPolicy(QSql::NumericalPrecisionPolicy precisionPolicy);
//    QSql::NumericalPrecisionPolicy numericalPrecisionPolicy() const;

    Q_INVOKABLE bool seek(int i, bool relative = false);
    Q_INVOKABLE bool next();
    Q_INVOKABLE bool previous();
    Q_INVOKABLE bool first();
    Q_INVOKABLE bool last();

    Q_INVOKABLE void clear();

    // prepared query support
    Q_INVOKABLE bool exec();
    Q_INVOKABLE bool execBatch(QString mode = QLatin1String("ValuesAsRows"));
    Q_INVOKABLE bool prepare(const QString& query);
    Q_INVOKABLE void bindValue(const QString& placeholder, const QVariant& val);
    Q_INVOKABLE void bindValue(int pos, const QVariant& val);
    Q_INVOKABLE void addBindValue(const QVariant& val);
//    void bindValue(const QString& placeholder, const QVariant& val,
//                   QSql::ParamType type = QSql::In);
//    void bindValue(int pos, const QVariant& val, QSql::ParamType type = QSql::In);
//    void addBindValue(const QVariant& val, QSql::ParamType type = QSql::In);
    Q_INVOKABLE  QVariant boundValue(const QString& placeholder) const;
    Q_INVOKABLE  QVariant boundValue(int pos) const;
    Q_INVOKABLE  QMap<QString, QVariant> boundValues() const;
    QString executedQuery() const;
    Q_INVOKABLE  QVariant lastInsertId() const;
    Q_INVOKABLE  void finish();
    Q_INVOKABLE  bool nextResult();

    bool autoThrow() const;
    void setAutoThrow(bool autoThrow);

private:
    inline void throwError() const;

private:
    QSqlQuery *m_query;
    bool m_autoThrow;
};

Q_DECLARE_METATYPE(ScriptableQuery*)

#endif // SCRIPTABLEQUERY_H
