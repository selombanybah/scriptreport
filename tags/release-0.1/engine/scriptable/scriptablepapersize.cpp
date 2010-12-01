#include "scriptablepapersize.h"

#include <QtCore/QMetaEnum>
#include <QtGui/QPrinter>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptValue>

/*
 * Statics
 */

static int getUnitValue(QString unit) {
    const QMetaObject &mo = ScriptablePaperSize::staticMetaObject;
    int index = mo.indexOfEnumerator("Unit");
    QMetaEnum metaEnum = mo.enumerator(index);

    return metaEnum.keyToValue(unit.toLatin1().constData());
}

static qreal unitPointConversionFactor(ScriptablePaperSize::Unit unit) {
    switch(unit) {
    case ScriptablePaperSize::Millimeter:
        return 2.83464566929;
    case ScriptablePaperSize::Centimeter:
        return 28.3464566929;
    case ScriptablePaperSize::Point:
        return 1.0;
    case ScriptablePaperSize::Inch:
        return 72.0;
    case ScriptablePaperSize::Pica:
        return 12;
    case ScriptablePaperSize::Didot:
        return 1.065826771;
    case ScriptablePaperSize::Cicero:
        return 12.789921252;
    }
    return 1.0;
}

QPrinter::Unit toQPrinter(ScriptablePaperSize::Unit unit) {
    switch(unit) {
    case ScriptablePaperSize::Centimeter:
        qCritical("Centimeter is not a QPrinter unit");
    case ScriptablePaperSize::Millimeter:
        return QPrinter::Millimeter;
    case ScriptablePaperSize::Point:
        return QPrinter::Point;
    case ScriptablePaperSize::Inch:
        return QPrinter::Inch;
    case ScriptablePaperSize::Pica:
        return QPrinter::Pica;
    case ScriptablePaperSize::Didot:
        return QPrinter::Didot;
    case ScriptablePaperSize::Cicero:
        return QPrinter::Cicero;
    }
    return QPrinter::Point;
}

ScriptablePaperSize::Size fromQPrinter(QPrinter::PaperSize size) {
    switch(size) {
    case QPrinter::A4:
        return ScriptablePaperSize::A4;
    case QPrinter::B5:
        return ScriptablePaperSize::B5;
    case QPrinter::Letter:
        return ScriptablePaperSize::Letter;
    case QPrinter::Legal:
        return ScriptablePaperSize::Legal;
    case QPrinter::Executive:
        return ScriptablePaperSize::Executive;
    case QPrinter::A0:
        return ScriptablePaperSize::A0;
    case QPrinter::A1:
        return ScriptablePaperSize::A1;
    case QPrinter::A2:
        return ScriptablePaperSize::A2;
    case QPrinter::A3:
        return ScriptablePaperSize::A3;
    case QPrinter::A5:
        return ScriptablePaperSize::A5;
    case QPrinter::A6:
        return ScriptablePaperSize::A6;
    case QPrinter::A7:
        return ScriptablePaperSize::A7;
    case QPrinter::A8:
        return ScriptablePaperSize::A8;
    case QPrinter::A9:
        return ScriptablePaperSize::A9;
    case QPrinter::B0:
        return ScriptablePaperSize::B0;
    case QPrinter::B1:
        return ScriptablePaperSize::B1;
    case QPrinter::B10:
        return ScriptablePaperSize::B10;
    case QPrinter::B2:
        return ScriptablePaperSize::B2;
    case QPrinter::B3:
        return ScriptablePaperSize::B3;
    case QPrinter::B4:
        return ScriptablePaperSize::B4;
    case QPrinter::B6:
        return ScriptablePaperSize::B6;
    case QPrinter::B7:
        return ScriptablePaperSize::B7;
    case QPrinter::B8:
        return ScriptablePaperSize::B8;
    case QPrinter::B9:
        return ScriptablePaperSize::B9;
    case QPrinter::C5E:
        return ScriptablePaperSize::C5E;
    case QPrinter::Comm10E:
        return ScriptablePaperSize::Comm10E;
    case QPrinter::DLE:
        return ScriptablePaperSize::DLE;
    case QPrinter::Folio:
        return ScriptablePaperSize::Folio;
    case QPrinter::Ledger:
        return ScriptablePaperSize::Ledger;
    case QPrinter::Tabloid:
        return ScriptablePaperSize::Tabloid;
    case QPrinter::Custom:
        return ScriptablePaperSize::Custom;
    }
    return ScriptablePaperSize::Custom;
}

QPrinter::PaperSize toQPrinter(ScriptablePaperSize::Size size) {
    switch(size) {
    case ScriptablePaperSize::A4:
        return QPrinter::A4;
    case ScriptablePaperSize::B5:
        return QPrinter::B5;
    case ScriptablePaperSize::Letter:
        return QPrinter::Letter;
    case ScriptablePaperSize::Legal:
        return QPrinter::Legal;
    case ScriptablePaperSize::Executive:
        return QPrinter::Executive;
    case ScriptablePaperSize::A0:
        return QPrinter::A0;
    case ScriptablePaperSize::A1:
        return QPrinter::A1;
    case ScriptablePaperSize::A2:
        return QPrinter::A2;
    case ScriptablePaperSize::A3:
        return QPrinter::A3;
    case ScriptablePaperSize::A5:
        return QPrinter::A5;
    case ScriptablePaperSize::A6:
        return QPrinter::A6;
    case ScriptablePaperSize::A7:
        return QPrinter::A7;
    case ScriptablePaperSize::A8:
        return QPrinter::A8;
    case ScriptablePaperSize::A9:
        return QPrinter::A9;
    case ScriptablePaperSize::B0:
        return QPrinter::B0;
    case ScriptablePaperSize::B1:
        return QPrinter::B1;
    case ScriptablePaperSize::B10:
        return QPrinter::B10;
    case ScriptablePaperSize::B2:
        return QPrinter::B2;
    case ScriptablePaperSize::B3:
        return QPrinter::B3;
    case ScriptablePaperSize::B4:
        return QPrinter::B4;
    case ScriptablePaperSize::B6:
        return QPrinter::B6;
    case ScriptablePaperSize::B7:
        return QPrinter::B7;
    case ScriptablePaperSize::B8:
        return QPrinter::B8;
    case ScriptablePaperSize::B9:
        return QPrinter::B9;
    case ScriptablePaperSize::C5E:
        return QPrinter::C5E;
    case ScriptablePaperSize::Comm10E:
        return QPrinter::Comm10E;
    case ScriptablePaperSize::DLE:
        return QPrinter::DLE;
    case ScriptablePaperSize::Folio:
        return QPrinter::Folio;
    case ScriptablePaperSize::Ledger:
        return QPrinter::Ledger;
    case ScriptablePaperSize::Tabloid:
        return QPrinter::Tabloid;
    case ScriptablePaperSize::Custom:
        return QPrinter::Custom;
    }
    return QPrinter::Custom;
}

/*
 * Class
 */

ScriptablePaperSize::ScriptablePaperSize(QObject *parent) :
    QObject(parent), QScriptable()
{
}

ScriptablePaperSize::Size ScriptablePaperSize::size() const {
    return m_size;
}

void ScriptablePaperSize::setSize(ScriptablePaperSize::Size size) {
    if (size >= A4 && size <= Custom) {
        m_size = size;
        m_isSizeChanged = true;
        m_isUpdateHeightAndWidthNeeded = true;
    } else {
        QString arg = argument(0).toString();
        context()->throwError(tr("%1 is not a valid size.").arg(arg));
    }
}

QString ScriptablePaperSize::sizeByName() const {
    const QMetaObject &mo = ScriptablePaperSize::staticMetaObject;
    int index = mo.indexOfEnumerator("Size");
    QMetaEnum metaEnum = mo.enumerator(index);

    return QString::fromLatin1(metaEnum.valueToKey(m_size));
}

void ScriptablePaperSize::setSizeByName(QString size) {
    const QMetaObject &mo = ScriptablePaperSize::staticMetaObject;
    int index = mo.indexOfEnumerator("Size");
    QMetaEnum metaEnum = mo.enumerator(index);

    int s = metaEnum.keyToValue(size.toLatin1().constData());
    if (s == -1) {
        context()->throwError(tr("%1 is not a valid unit.").arg(size));
    } else {
        m_size = (Size) s;
        m_isSizeChanged = true;
        m_isUpdateHeightAndWidthNeeded = true;
    }
}

ScriptablePaperSize::Unit ScriptablePaperSize::unit() const {
    return m_unit;
}

void ScriptablePaperSize::setUnit(ScriptablePaperSize::Unit unit) {
    if (unit >= Millimeter && unit <= Centimeter) {
        m_unit = unit;
        m_isSizeChanged = true;
    } else {
        QString arg = argument(0).toString();
        context()->throwError(tr("%1 is not a valid unit.").arg(arg));
    }
}

QString ScriptablePaperSize::unitByName() const {
    const QMetaObject &mo = ScriptablePaperSize::staticMetaObject;
    int index = mo.indexOfEnumerator("Unit");
    QMetaEnum metaEnum = mo.enumerator(index);

    return QString::fromLatin1(metaEnum.valueToKey(m_unit));
}

void ScriptablePaperSize::setUnitByName(QString unit) {
    int u = getUnitValue(unit);
    if (u == -1) {
        context()->throwError(tr("%1 is not a valid unit.").arg(unit));
    } else {
        m_unit = (Unit) u;
        m_isSizeChanged = true;
    }
}

qreal ScriptablePaperSize::height() /*const*/ {
    if (m_isUpdateHeightAndWidthNeeded) {
        updateHeightAndWidth();
    }
    return m_height;
}

void ScriptablePaperSize::setHeight(qreal height) {
    m_height = height;
    m_size = Custom;
    m_isSizeChanged = true;
}

qreal ScriptablePaperSize::width() /*const*/ {
    if (m_isUpdateHeightAndWidthNeeded) {
        updateHeightAndWidth();
    }
    return m_width;
}

void ScriptablePaperSize::setWidth(qreal width) {
    m_width = width;
    m_size = Custom;
    m_isSizeChanged = true;
}

void ScriptablePaperSize::setSize(qreal height, qreal width, QString unit) {
    int i = getUnitValue(unit);
    if (i == -1) {
        return;
    }
    m_unit = (Unit) i;
    m_height = height;
    m_width = width;
    m_size = Custom;
    m_isSizeChanged = true;
}

void ScriptablePaperSize::updateUnit(QString unit) {
    // to point unit
    int i = getUnitValue(unit);
    if (i == -1) {
        return;
    }
    Unit u = (Unit) i;
    qreal factor = unitPointConversionFactor(m_unit);
    m_height = m_height * factor;
    m_width  = m_width  * factor;
    // to new unit
    factor = unitPointConversionFactor(u);
    m_height = m_height / factor;
    m_width  = m_width  / factor;
    m_unit = u;
}

void ScriptablePaperSize::loadConfigurationFrom(QPrinter &printer) {
    m_size = fromQPrinter(printer.paperSize());
    QSizeF sizef = printer.paperSize(QPrinter::Point);
    m_height = sizef.height();
    m_width  = sizef.width();
    m_unit   = Point;
    m_isSizeChanged = false;
    m_isUpdateHeightAndWidthNeeded = false;
}

void ScriptablePaperSize::applyConfigurationTo(QPrinter &printer) {
    if (m_isSizeChanged) {
        if (m_size == Custom) {
            if (m_unit == Centimeter) {
                QSizeF sizef(m_width * 10, m_height * 10);
                printer.setPaperSize(sizef, QPrinter::Millimeter);
            } else {
                QSizeF sizef(m_width, m_height);
                printer.setPaperSize(sizef, toQPrinter(m_unit));
            }
        } else {
            printer.setPaperSize(toQPrinter(m_size));
        }
    }
}

void ScriptablePaperSize::updateHeightAndWidth() {
    QPrinter printer;
    printer.setPageSize(toQPrinter(m_size));
    QSizeF sizef = printer.paperSize(toQPrinter(m_unit));
    m_height = sizef.height();
    m_width  = sizef.width();
    m_isUpdateHeightAndWidthNeeded = false;
}
