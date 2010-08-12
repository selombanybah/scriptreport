#ifndef SCRIPTABLEPAPERMARGINS_H
#define SCRIPTABLEPAPERMARGINS_H

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtScript/QScriptable>

class QPrinter;

class ScriptablePaperMargins : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(QString unit READ unitByName WRITE setUnitByName)
    Q_PROPERTY(qreal top READ top WRITE setTop)
    Q_PROPERTY(qreal right READ right WRITE setRight)
    Q_PROPERTY(qreal bottom READ bottom WRITE setBottom)
    Q_PROPERTY(qreal left READ left WRITE setLeft)
    Q_ENUMS(Unit)

    // unit property by name for validate and throw an error
public:
    enum Unit {
        Millimeter,
        Point,
        Inch,
        Pica,
        Didot,
        Cicero,
        Centimeter,
        mm = Millimeter,
        cm = Centimeter,
        pt = Point,
        in = Inch,
        pc = Pica
    };

    explicit ScriptablePaperMargins(QObject *parent = 0);

    Unit unit() const;
    void setUnit(Unit unit);

    QString unitByName() const;
    void setUnitByName(QString unit);

    qreal top();
    void setTop(qreal top);

    qreal right();
    void setRight(qreal right);

    qreal bottom();
    void setBottom(qreal bottom);

    qreal left();
    void setLeft(qreal left);

    Q_INVOKABLE void setMargins(qreal top, qreal right, qreal bottom, qreal left, QString unit);
    Q_INVOKABLE void updateUnit(QString unit);

    void loadConfigurationFrom(QPrinter &printer);
    void applyConfigurationTo(QPrinter &printer);

private:
    qreal m_top;
    qreal m_right;
    qreal m_bottom;
    qreal m_left;
    Unit m_unit;
    bool m_isMarginsChanged;

};

Q_DECLARE_METATYPE(ScriptablePaperMargins*)

#endif // SCRIPTABLEPAPERMARGINS_H
