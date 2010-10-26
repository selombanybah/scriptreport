#include "scriptablelocale.h"

#include <QtCore/QMetaEnum>

ScriptableLocale::ScriptableLocale(QLocale locale, QObject *parent) :
    QObject(parent),
    m_locale(locale)
{
}

QString ScriptableLocale::languageName(QLocale::Language language) {
    const QMetaObject &mo = QLocale::staticMetaObject;
    int index = mo.indexOfEnumerator("Language");
    QMetaEnum metaEnum = mo.enumerator(index);

    return QString::fromLatin1(metaEnum.valueToKey(language));
}

int ScriptableLocale::languageIndex(QString language) {
    const QMetaObject &mo = ScriptableLocale::staticMetaObject;
    int index = mo.indexOfEnumerator("Language");
    QMetaEnum metaEnum = mo.enumerator(index);

    return metaEnum.keyToValue(language.toLatin1().constData());
}

QString ScriptableLocale::countryName(QLocale::Country country) {
    const QMetaObject &mo = QLocale::staticMetaObject;
    int index = mo.indexOfEnumerator("Country");
    QMetaEnum metaEnum = mo.enumerator(index);

    return QString::fromLatin1(metaEnum.valueToKey(country));
}

int ScriptableLocale::countryIndex(QString country) {
    const QMetaObject &mo = ScriptableLocale::staticMetaObject;
    int index = mo.indexOfEnumerator("Country");
    QMetaEnum metaEnum = mo.enumerator(index);

    return metaEnum.keyToValue(country.toLatin1().constData());
}

int ScriptableLocale::formatTypeIndex(QString formatType) {
    const QMetaObject &mo = ScriptableLocale::staticMetaObject;
    int index = mo.indexOfEnumerator("FormatType");
    QMetaEnum metaEnum = mo.enumerator(index);

    return metaEnum.keyToValue(formatType.toLatin1().constData());
}

QString ScriptableLocale::language() const {
    return languageName(m_locale.language());
}

QString ScriptableLocale::country() const {
    return countryName(m_locale.country());
}

QString ScriptableLocale::name() const {
    return m_locale.name();
}

short ScriptableLocale::toShort(const QString &s, bool *ok, int base) const {
    return m_locale.toShort(s, ok, base);
}

ushort ScriptableLocale::toUShort(const QString &s, bool *ok, int base) const {
    return m_locale.toUShort(s, ok, base);
}

int ScriptableLocale::toInt(const QString &s, bool *ok, int base) const {
    return m_locale.toInt(s, ok, base);
}

uint ScriptableLocale::toUInt(const QString &s, bool *ok, int base) const {
    return m_locale.toUInt(s, ok, base);
}

qlonglong ScriptableLocale::toLongLong(const QString &s, bool *ok, int base) const {
    return m_locale.toLongLong(s, ok, base);
}

qlonglong ScriptableLocale::toULongLong(const QString &s, bool *ok, int base) const {
    return m_locale.toULongLong(s, ok, base);
}

float ScriptableLocale::toFloat(const QString &s, bool *ok) const {
    return m_locale.toFloat(s, ok);
}

double ScriptableLocale::toDouble(const QString &s, bool *ok) const {
    return m_locale.toDouble(s, ok);
}

QString ScriptableLocale::toString(qlonglong i) const {
    return m_locale.toString(i);
}

QString ScriptableLocale::toString(qulonglong i) const {
    return m_locale.toString(i);
}

QString ScriptableLocale::toString(short i) const {
    return m_locale.toString(i);
}

QString ScriptableLocale::toString(ushort i) const {
    return m_locale.toString(i);
}

QString ScriptableLocale::toString(int i) const {
    return m_locale.toString(i);
}

QString ScriptableLocale::toString(uint i) const {
    return m_locale.toString(i);
}

QString ScriptableLocale::toString(double i, char f, int prec) const {
    return m_locale.toString(i, f, prec);
}

QString ScriptableLocale::toString(float i, char f, int prec) const {
    return m_locale.toString(i, f, prec);
}

QString ScriptableLocale::toString(const QDate &date, const QString &formatStr) const {
    int index = formatTypeIndex(formatStr);
    if (index >= 0) {
        return m_locale.toString(date, (QLocale::FormatType) index);
    } else {
        return m_locale.toString(date, formatStr);
    }
}

QString ScriptableLocale::toString(const QTime &time, const QString &formatStr) const {
    int index = formatTypeIndex(formatStr);
    if (index >= 0) {
        return m_locale.toString(time, (QLocale::FormatType) index);
    } else {
        return m_locale.toString(time, formatStr);
    }
}

QString ScriptableLocale::toString(const QDateTime &dateTime, const QString &format) const {
    int index = formatTypeIndex(format);
    if (index >= 0) {
        return m_locale.toString(dateTime, (QLocale::FormatType) index);
    } else {
        return m_locale.toString(dateTime, format);
    }
}

QString ScriptableLocale::dateFormat(const QString &format) const {
    int index = formatTypeIndex(format);
    if (index >= 0) {
        return m_locale.dateFormat((QLocale::FormatType) index);
    } else {
        return QString();
    }
}

QString ScriptableLocale::timeFormat(const QString &format) const {
    int index = formatTypeIndex(format);
    if (index >= 0) {
        return m_locale.timeFormat((QLocale::FormatType) index);
    } else {
        return QString();
    }
}

QString ScriptableLocale::dateTimeFormat(const QString &format) const {
    int index = formatTypeIndex(format);
    if (index >= 0) {
        return m_locale.dateTimeFormat((QLocale::FormatType) index);
    } else {
        return QString();
    }
}

#ifndef QT_NO_DATESTRING
QDate ScriptableLocale::toDate(const QString &string, const QString &format) const {
    int index = formatTypeIndex(format);
    if (index >= 0) {
        return m_locale.toDate(string, (QLocale::FormatType) index);
    } else {
        return m_locale.toDate(string, format);
    }
}

QTime ScriptableLocale::toTime(const QString &string, const QString &format) const {
    int index = formatTypeIndex(format);
    if (index >= 0) {
        return m_locale.toTime(string, (QLocale::FormatType) index);
    } else {
        return m_locale.toTime(string, format);
    }
}

QDateTime ScriptableLocale::toDateTime(const QString &string, const QString &format) const {
    int index = formatTypeIndex(format);
    if (index >= 0) {
        return m_locale.toDateTime(string, (QLocale::FormatType) index);
    } else {
        return m_locale.toDateTime(string, format);
    }
}
#endif

QChar ScriptableLocale::decimalPoint() const {
    return m_locale.decimalPoint();
}

QChar ScriptableLocale::groupSeparator() const {
    return m_locale.groupSeparator();
}

QChar ScriptableLocale::percent() const {
    return m_locale.percent();
}

QChar ScriptableLocale::zeroDigit() const {
    return m_locale.zeroDigit();
}

QChar ScriptableLocale::negativeSign() const {
    return m_locale.negativeSign();
}

QChar ScriptableLocale::positiveSign() const {
    return m_locale.positiveSign();
}

QChar ScriptableLocale::exponential() const {
    return m_locale.exponential();
}

QString ScriptableLocale::monthName(int i, const QString &format) const {
    int index = formatTypeIndex(format);
    if (index >= 0) {
        return m_locale.monthName(i, (QLocale::FormatType) index);
    } else {
        return QString();
    }
}

QString ScriptableLocale::standaloneMonthName(int i, const QString &format) const {
    int index = formatTypeIndex(format);
    if (index >= 0) {
        return m_locale.standaloneMonthName(i, (QLocale::FormatType) index);
    } else {
        return QString();
    }
}

QString ScriptableLocale::dayName(int i, const QString &format) const {
    int index = formatTypeIndex(format);
    if (index >= 0) {
        return m_locale.dayName(i, (QLocale::FormatType) index);
    } else {
        return QString();
    }
}

QString ScriptableLocale::standaloneDayName(int i, const QString &format) const {
    int index = formatTypeIndex(format);
    if (index >= 0) {
        return m_locale.standaloneDayName(i, (QLocale::FormatType) index);
    } else {
        return QString();
    }
}

QString ScriptableLocale::amText() const {
    return m_locale.amText();
}

QString ScriptableLocale::pmText() const {
    return m_locale.pmText();
}

#if QT_VERSION >= 0x040700
bool ScriptableLocale::isRightToLeft() const {
    return m_locale.textDirection() == Qt::RightToLeft;
}

bool ScriptableLocale::isLeftToRight() const {
    return m_locale.textDirection() == Qt::LeftToRight;
}
#endif

bool ScriptableLocale::isMetricSystem() const {
    return m_locale.measurementSystem() == QLocale::MetricSystem;
}

bool ScriptableLocale::isImperialSystem() const {
    return m_locale.measurementSystem() == QLocale::ImperialSystem;
}

bool ScriptableLocale::omitGroupSeparator() const {
    return m_locale.numberOptions() & QLocale::OmitGroupSeparator;
}

void ScriptableLocale::setOmitGroupSeparator(bool value) {
    if (value) {
        m_locale.setNumberOptions(m_locale.numberOptions() | QLocale::OmitGroupSeparator);
    } else {
        m_locale.setNumberOptions(m_locale.numberOptions() & !QLocale::OmitGroupSeparator);
    }
}

bool ScriptableLocale::rejectGroupSeparator() const {
    return m_locale.numberOptions() & QLocale::RejectGroupSeparator;
}

void ScriptableLocale::setRejectGroupSeparator(bool value) {
    if (value) {
        m_locale.setNumberOptions(m_locale.numberOptions() | QLocale::RejectGroupSeparator);
    } else {
        m_locale.setNumberOptions(m_locale.numberOptions() & !QLocale::RejectGroupSeparator);
    }
}

bool ScriptableLocale::equals(const ScriptableLocale &other) const {
    return m_locale == other.m_locale;
}

QLocale ScriptableLocale::locale() const {
    return m_locale;
}
