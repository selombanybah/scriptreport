#ifndef SCRIPTABLEPAPER_H
#define SCRIPTABLEPAPER_H

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtScript/QScriptable>

#include "scriptablepapersize.h"
#include "scriptablepapermargins.h"

class QPrinter;
class QScriptEngine;

class ScriptablePaper : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(QString orientation READ orientationByName WRITE setOrientationByName)
    Q_PROPERTY(ScriptablePaperSize* size READ size)
    Q_PROPERTY(ScriptablePaperMargins* margins READ margins)
    Q_ENUMS(Orientation)

    // unit property by name for validate and throw an error
public:
    enum Orientation {Portrait, Landscape};

    explicit ScriptablePaper(QObject *parent = 0);

    Orientation orientation() const;
    void setOrientation(Orientation orientation);

    QString orientationByName() const;
    void setOrientationByName(QString orientation);

    ScriptablePaperSize* size() const;
    ScriptablePaperMargins* margins() const;

    void loadConfigurationFrom(QPrinter &printer);
    void applyConfigurationTo(QPrinter &printer);
    void initEngine(QScriptEngine &engine);

private:
    Orientation m_orientation;
    ScriptablePaperSize* m_size;
    ScriptablePaperMargins* m_margins;
    bool m_isOrientationChanged;
};

Q_DECLARE_METATYPE(ScriptablePaper*)

#endif // SCRIPTABLEPAPER_H
