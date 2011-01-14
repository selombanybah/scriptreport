/*
 * Copyright 2010 and beyond, Juan Luis Paz
 *
 * This file is part of Script Report.
 *
 * Script Report is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Script Report is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Script Report.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SCRIPTABLEREPORT_H
#define SCRIPTABLEREPORT_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtScript/QScriptable>
#include <QtScript/QScriptValue>

#include "scriptablepaper.h"

class QScriptEngine;
class QPrinter;

class ScriptReport;
class TextStreamObject;

class ScriptableReport : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString page READ page)
    Q_PROPERTY(QString pageCount READ pageCount)
    Q_PROPERTY(bool isFinal READ isFinal)
    Q_PROPERTY(bool isEditing READ isEditing)
    Q_PROPERTY(bool isDebbuging READ isDebbuging)
    Q_PROPERTY(ScriptablePaper* paper READ paper)

public:
    ScriptableReport(ScriptReport *scriptReport, QObject *parent = 0);

    QString title() const;
    void setTitle(QString title);

    QString page() const;
    QString pageCount() const;
    bool isFinal() const;
    bool isEditing() const;
    bool isDebbuging() const;
    ScriptablePaper* paper() const;

    Q_INVOKABLE void writeHeader();
    Q_INVOKABLE void writeHeaderFirst();
    Q_INVOKABLE void writeHeaderLast();
    Q_INVOKABLE void writeContent();
    Q_INVOKABLE void writeFooter();
    Q_INVOKABLE void writeFooterFirst();
    Q_INVOKABLE void writeFooterLast();

    void loadConfigurationFrom(QPrinter &printer);
    void applyConfigurationTo(QPrinter &printer);
    void initEngine(QScriptEngine &engine);

    Q_INVOKABLE QString addImageResource(QScriptValue value, QString url = QString());

private:
    void writeResult(const TextStreamObject *outputObject);
    void printAndWriteResult(const TextStreamObject *outputObject);
    QString toString(QScriptValue value, QString url = QString());
    QString insertImageResource(QScriptValue value, QString url = QString());

private:
    ScriptReport *m_sre;
    ScriptablePaper *m_scriptablePaper;
    QString m_title;
};

#endif // SCRIPTABLEREPORT_H
