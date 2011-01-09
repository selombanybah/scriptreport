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

#ifndef SCRIPTREPORT_H
#define SCRIPTREPORT_H

#include "scriptreportengine_global.h"

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

class QPrinter;
class QScriptEngine;
class QTextStream;

class TextStreamObject;
class ScriptReportEngine;

class QPixmap;

class ScriptReportPrivate;
class SCRIPTREPORTENGINE_EXPORT ScriptReport : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isEditing READ isEditing WRITE setEditing)
    Q_PROPERTY(bool isFinal READ isFinal WRITE setFinal)
    Q_PROPERTY(bool isDebugging READ isDebugging WRITE setDebugging)
    Q_PROPERTY(bool isPrintErrorEnabled READ isPrintErrorEnabled WRITE setPrintErrorEnabled)
    Q_PROPERTY(QStringList arguments READ arguments WRITE setArguments)
    Q_PROPERTY(QString previousScript READ previousScript WRITE setPreviousScript)
    Q_PROPERTY(QString reportName READ reportName WRITE setReportName)
    Q_PROPERTY(bool isWriteWithPrintFunctionTooEnabled READ isWriteWithPrintFunctionTooEnabled WRITE setWriteWithPrintFunctionTooEnabled)

    Q_PROPERTY(QString intermediateCode READ intermediateCode)
    Q_PROPERTY(bool hasUncaughtException READ hasUncaughtException)
    Q_PROPERTY(QString errorMessage READ errorMessage)

public:
    explicit ScriptReport(QObject *parent = 0);
    explicit ScriptReport(QString reportName, QObject *parent = 0);
    ScriptReport(QTextStream *inputStream, QString reportName, QObject *parent = 0);
    ScriptReport(QString input, QString reportName, QObject *parent = 0);
    ~ScriptReport();

    bool isEditing() const;
    void setEditing(bool editing);
    bool isFinal() const;
    void setFinal(bool final);
    bool isDebugging() const;
    void setDebugging(bool debugging);
    bool isPrintErrorEnabled() const;
    void setPrintErrorEnabled(bool isPrintErrorEnabled);

    QStringList arguments() const;
    void setArguments(QStringList arguments);
    QString previousScript() const;
    void setPreviousScript(QString previousScript);

    QString reportName() const;
    void setReportName(QString reportName);

    bool isWriteWithPrintFunctionTooEnabled() const;
    void setWriteWithPrintFunctionTooEnabled(bool isWriteWithPrintFunctionTooEnabled);

    TextStreamObject* input() const;
    const TextStreamObject* outputHeader() const;
    const TextStreamObject* outputHeaderFirst() const;
    const TextStreamObject* outputHeaderLast() const;
    const TextStreamObject* outputContent() const;
    const TextStreamObject* outputFooter() const;
    const TextStreamObject* outputFooterFirst() const;
    const TextStreamObject* outputFooterLast() const;
    TextStreamObject* printOutput() const;

    QScriptEngine* scriptEngine() /*const*/;
//   void setScriptEngine(QScriptEngine *socriptEngine);

    ScriptReportEngine* scriptReportEngine() /*const*/;

    QString intermediateCode() const;
    bool hasUncaughtException() const;
    QString errorMessage() const;

    void loadPrintConfiguration(QPrinter *printer);

    QString addResource(int type, const QVariant &resource, QString url = QString());
    void addResource(int type, const QVariant & resource, const QUrl & url);
    QMap<QUrl, QPair<int, QVariant> > resources() const;

public slots:
    void updateIntermediateCode();
    void run();
    void print(QPrinter *printer);

protected:
    void initEngine();

private:
    ScriptReportPrivate *d;
};

#endif // SCRIPTREPORT_H
