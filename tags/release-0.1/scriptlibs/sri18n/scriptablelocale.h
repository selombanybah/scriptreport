#ifndef SCRIPTABLELOCALE_H
#define SCRIPTABLELOCALE_H

#include <QtCore/QDate>
#include <QtCore/QDateTime>
#include <QtCore/QTime>
#include <QtCore/QLocale>
#include <QtCore/QObject>
#include <QtCore/QMetaType>

class ScriptableLocale : public QObject
{
    Q_OBJECT
    Q_ENUMS(FormatType)
    Q_PROPERTY(QString language READ language)
    Q_PROPERTY(QString country READ country)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString decimalPoint READ decimalPoint)
    Q_PROPERTY(QString groupSeparator READ groupSeparator)
    Q_PROPERTY(QString percent READ percent)
    Q_PROPERTY(QString zeroDigit READ zeroDigit)
    Q_PROPERTY(QString negativeSign READ negativeSign)
    Q_PROPERTY(QString positiveSign READ positiveSign)
    Q_PROPERTY(QString exponential READ exponential)
    Q_PROPERTY(QString amText READ amText)
    Q_PROPERTY(QString pmText READ pmText)
#if QT_VERSION >= 0x040700
    Q_PROPERTY(bool isRightToLeft READ isRightToLeft)
    Q_PROPERTY(bool isLeftToRight READ isLeftToRight)
#endif
    Q_PROPERTY(bool isMetricSystem READ isMetricSystem)
    Q_PROPERTY(bool isImperialSystem READ isImperialSystem)
    Q_PROPERTY(bool omitGroupSeparator READ omitGroupSeparator WRITE setOmitGroupSeparator)
    Q_PROPERTY(bool rejectGroupSeparator READ rejectGroupSeparator WRITE setRejectGroupSeparator)

public:
    enum FormatType { LongFormat, ShortFormat, NarrowFormat };

    explicit ScriptableLocale(QLocale locale, QObject *parent = 0);

    QString language() const;
    QString country() const;
    QString name() const;

    Q_INVOKABLE short toShort(const QString &s, bool *ok = 0, int base = 0) const;
    Q_INVOKABLE ushort toUShort(const QString &s, bool *ok = 0, int base = 0) const;
    Q_INVOKABLE int toInt(const QString &s, bool *ok = 0, int base = 0) const;
    Q_INVOKABLE uint toUInt(const QString &s, bool *ok = 0, int base = 0) const;
    Q_INVOKABLE qlonglong toLongLong(const QString &s, bool *ok = 0, int base = 0) const;
    Q_INVOKABLE qlonglong toULongLong(const QString &s, bool *ok = 0, int base = 0) const;
    Q_INVOKABLE float toFloat(const QString &s, bool *ok = 0) const;
    Q_INVOKABLE double toDouble(const QString &s, bool *ok = 0) const;

    Q_INVOKABLE QString toString(qlonglong i) const;
    Q_INVOKABLE QString toString(qulonglong i) const;
    Q_INVOKABLE QString toString(short i) const;
    Q_INVOKABLE QString toString(ushort i) const;
    Q_INVOKABLE QString toString(int i) const;
    Q_INVOKABLE QString toString(uint i) const;
    Q_INVOKABLE QString toString(double i, char f = 'g', int prec = 6) const;
    Q_INVOKABLE QString toString(float i, char f = 'g', int prec = 6) const;
    Q_INVOKABLE QString toString(const QDate &date, const QString &formatStr = QLatin1String("LongFormat")) const;
    Q_INVOKABLE QString toString(const QTime &time, const QString &formatStr = QLatin1String("LongFormat")) const;
    Q_INVOKABLE QString toString(const QDateTime &dateTime, const QString &format = QLatin1String("LongFormat")) const;

    Q_INVOKABLE QString dateFormat(const QString &format = QLatin1String("LongFormat")) const;
    Q_INVOKABLE QString timeFormat(const QString &format = QLatin1String("LongFormat")) const;
    Q_INVOKABLE QString dateTimeFormat(const QString &format = QLatin1String("LongFormat")) const;
#ifndef QT_NO_DATESTRING
    Q_INVOKABLE QDate toDate(const QString &string, const QString &format = QLatin1String("LongFormat")) const;
    Q_INVOKABLE QTime toTime(const QString &string, const QString &format = QLatin1String("LongFormat")) const;
    Q_INVOKABLE QDateTime toDateTime(const QString &string, const QString &format = QLatin1String("LongFormat")) const;
#endif

    QChar decimalPoint() const;
    QChar groupSeparator() const;
    QChar percent() const;
    QChar zeroDigit() const;
    QChar negativeSign() const;
    QChar positiveSign() const;
    QChar exponential() const;

    Q_INVOKABLE QString monthName(int i, const QString &format = QLatin1String("LongFormat")) const;
    Q_INVOKABLE QString standaloneMonthName(int i, const QString &format = QLatin1String("LongFormat")) const;
    Q_INVOKABLE QString dayName(int i, const QString &format = QLatin1String("LongFormat")) const;
    Q_INVOKABLE QString standaloneDayName(int i, const QString &format = QLatin1String("LongFormat")) const;

    QString amText() const;
    QString pmText() const;

#if QT_VERSION >= 0x040700
    bool isRightToLeft() const;
    bool isLeftToRight() const;
#endif

    bool isMetricSystem() const;
    bool isImperialSystem() const;

    bool omitGroupSeparator() const;
    void setOmitGroupSeparator(bool value);
    bool rejectGroupSeparator() const;
    void setRejectGroupSeparator(bool value);

    Q_INVOKABLE bool equals(const ScriptableLocale &other) const;

    QLocale locale() const;

    static QString languageName(QLocale::Language language);
    static int languageIndex(QString language);
    static QString countryName(QLocale::Country country);
    static int countryIndex(QString country);
    static int formatTypeIndex(QString formatType);

private:
    QLocale m_locale;
};

Q_DECLARE_METATYPE(ScriptableLocale*)

#endif // SCRIPTABLELOCALE_H
