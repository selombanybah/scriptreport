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

#include "scriptablereport.h"

#include <QtCore/QTextStream>
#include <QtGui/QPrinter>
#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QPicture>
#include <QtGui/QTextDocument>
#include <QtScript/QScriptEngine>

#include "scriptreport.h"
#include "textstreamobject.h"

/*
 * Statics
 */

static QScriptValue paperToScriptValue(QScriptEngine *engine, ScriptablePaper* const &in) {
    return engine->newQObject(in, QScriptEngine::QtOwnership, QScriptEngine::ExcludeChildObjects | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
}

static void paperFromScriptValue(const QScriptValue &object, ScriptablePaper* &out) {
    out = qobject_cast<ScriptablePaper*>(object.toQObject());
}

static QScriptValue print(QScriptContext *context, QScriptEngine *engine) {
    const QChar sp = QChar::fromLatin1(' ');
    const QChar nl = QChar::fromLatin1('\n');

    QScriptValue calleeData = context->callee().data();
    TextStreamObject *outputObject = qobject_cast<TextStreamObject*>(calleeData.toQObject());
    QTextStream *stream = outputObject->stream();
    int argumentCount = context->argumentCount();

    if (argumentCount > 0) {
        QString value = context->argument(0).toString();
        *stream << value;
    }
    for (int i = 1; i < argumentCount; i++) {
        *stream << sp;
        QString value = context->argument(i).toString();
        *stream << value;
    }
    *stream << nl;

    return engine->undefinedValue();
}

/*
 * Class
 */

ScriptableReport::ScriptableReport(ScriptReport *scriptReport, QObject *parent) :
    QObject(parent),
    QScriptable()
{
    m_sre = scriptReport;
    m_scriptablePaper = new ScriptablePaper(this);
    m_title = scriptReport->reportName();
}

QString ScriptableReport::title() const {
    return m_title;
}

void ScriptableReport::setTitle(QString title) {
    m_title = title;
}

QString ScriptableReport::page() const {
    return QString::fromLatin1("##page##");
}

QString ScriptableReport::pageCount() const {
    return QString::fromLatin1("##pageCount##");
}

bool ScriptableReport::isFinal() const {
    return m_sre->isFinal();
}

bool ScriptableReport::isEditing() const {
    return m_sre->isEditing();
}

bool ScriptableReport::isDebbuging() const {
    return m_sre->isDebugging();
}

ScriptablePaper* ScriptableReport::paper() const {
    return m_scriptablePaper;
}

void ScriptableReport::writeHeader() {
    if (m_sre->isWriteWithPrintFunctionTooEnabled()) {
        printAndWriteResult(m_sre->outputHeader());
    } else {
        writeResult(m_sre->outputHeader());
    }
}

void ScriptableReport::writeHeaderFirst() {
    if (m_sre->isWriteWithPrintFunctionTooEnabled()) {
        printAndWriteResult(m_sre->outputHeaderFirst());
    } else {
        writeResult(m_sre->outputHeaderFirst());
    }
}

void ScriptableReport::writeHeaderLast() {
    if (m_sre->isWriteWithPrintFunctionTooEnabled()) {
        printAndWriteResult(m_sre->outputHeaderLast());
    } else {
        writeResult(m_sre->outputHeaderLast());
    }
}

void ScriptableReport::writeContent() {
    if (m_sre->isWriteWithPrintFunctionTooEnabled()) {
        printAndWriteResult(m_sre->outputContent());
    } else {
        writeResult(m_sre->outputContent());
    }
}

void ScriptableReport::writeFooter() {
    if (m_sre->isWriteWithPrintFunctionTooEnabled()) {
        printAndWriteResult(m_sre->outputFooter());
    } else {
        writeResult(m_sre->outputFooter());
    }
}

void ScriptableReport::writeFooterFirst() {
    if (m_sre->isWriteWithPrintFunctionTooEnabled()) {
        printAndWriteResult(m_sre->outputFooterFirst());
    } else {
        writeResult(m_sre->outputFooterFirst());
    }
}

void ScriptableReport::writeFooterLast() {
    if (m_sre->isWriteWithPrintFunctionTooEnabled()) {
        printAndWriteResult(m_sre->outputFooterLast());
    } else {
        writeResult(m_sre->outputFooterLast());
    }
}

void ScriptableReport::loadConfigurationFrom(QPrinter &printer) {
    QString title = printer.docName();
    if (title.isEmpty()) {
        title = m_sre->reportName();
    }
    m_title = title;
    m_scriptablePaper->loadConfigurationFrom(printer);
}

void ScriptableReport::applyConfigurationTo(QPrinter &printer) {
    printer.setDocName(m_title);
    m_scriptablePaper->applyConfigurationTo(printer);
}

void ScriptableReport::initEngine(QScriptEngine &engine) {
    qScriptRegisterMetaType(&engine, paperToScriptValue, paperFromScriptValue);

    QScriptValue printFunction = engine.newFunction(print);
    printFunction.setData(engine.newQObject(m_sre->printOutput()));
    engine.globalObject().setProperty(QString::fromLatin1("print"), printFunction, QScriptValue::Undeletable);

    m_scriptablePaper->initEngine(engine);
}

void ScriptableReport::writeResult(const TextStreamObject* outputObject)
{
    QTextStream *stream = outputObject->stream();

    for (int i = 0; i < argumentCount(); ++i) {
        QString value = toString(argument(i));
        *stream << value;
    }
}

void ScriptableReport::printAndWriteResult(const TextStreamObject* outputObject)
{
    const QString begin1 = QString::fromLatin1(">>> Simple Template Result:");
    const QString begin2 = QString::fromLatin1("Begin >>>");
    const QString end1 = QString::fromLatin1("<<< Simple Template Result:");
    const QString end2 = QString::fromLatin1("End <<<");
    const QString printName = QString::fromLatin1("print");

    QTextStream *stream = outputObject->stream();
    QString name = outputObject->name();

    QScriptValue printFunction = engine()->globalObject().property(printName);

    QScriptValueList beginArgs;
    beginArgs << begin1;
    beginArgs << name;
    beginArgs << begin2;
    printFunction.call(context()->thisObject(), beginArgs);

    printFunction.call(context()->thisObject(), context()->argumentsObject());

    QScriptValueList endArgs;
    endArgs << end1;
    endArgs << name;
    endArgs << end2;
    printFunction.call(context()->thisObject(), endArgs);

    for (int i = 0; i < argumentCount(); ++i) {
        QString value = toString(argument(i));
        *stream << value;
    }
}

QString ScriptableReport::toString(QScriptValue value, QString url) {
    QString result = addImageResource(value, url);
    if (result.isNull()) {
        return value.toString();
    } else {
        return result;
    }
}

QString ScriptableReport::addImageResource(QScriptValue value, QString url) {
    QString result = insertImageResource(value, url);
    if (result.isNull()) {
        QString file = value.toString();
        if (file.isNull()) {
            return result;
        }
        QPixmap pixmap;
        if (pixmap.load(file)) {
            result = m_sre->addResource(QTextDocument::ImageResource, QVariant(pixmap), url);
            return result;
        }
        if (file.endsWith(QString::fromLatin1(".pic")) || file.endsWith(QString::fromLatin1(".PIC"))) {
            QPicture picture;
            if (picture.load(file)) {
                QPixmap image(picture.width(), picture.height());
                QPainter p;
                p.begin(&image);
                picture.play(&p);
                p.end();
                result = m_sre->addResource(QTextDocument::ImageResource, QVariant(image), url);
                return result;
            }
        }
    }

    return result;
}

Q_DECLARE_METATYPE(QPicture);

QString ScriptableReport::insertImageResource(QScriptValue value, QString url) {
    if (value.isQObject()) {
        QObject *object = value.toQObject();
        if (QWidget *w = qobject_cast<QWidget*>(object)) {
            { // fixme: need a previous render for get the real image size
                QPixmap image(w->size());
                QPainter p;
                p.begin(&image);
                w->render(&p);
                p.end();
            }
            QPixmap image(w->size());
            image.fill(QColor(0,0,0,0));
            QPainter p;
            p.begin(&image);
            w->render(&p);
            p.end();

            QString fileName = m_sre->addResource(QTextDocument::ImageResource, QVariant(image), url);
            return fileName;
        }
    } else if(value.isVariant()) {
        QVariant variant = value.toVariant();
        if (variant.canConvert<QPixmap>()) {
            QString fileName = m_sre->addResource(QTextDocument::ImageResource, variant, url);
            return fileName;
        } else if (variant.canConvert<QImage>()) {
            QString fileName = m_sre->addResource(QTextDocument::ImageResource, variant, url);
            return fileName;
        } else if (variant.canConvert<QByteArray>()) {
            QString fileName = m_sre->addResource(QTextDocument::ImageResource, variant, url);
            return fileName;
        } else if (variant.canConvert<QPicture>()) {
            QPicture picture = variant.value<QPicture>();
            QPixmap image(picture.width(), picture.height());
            image.fill(QColor(0,0,0,0));
            QPainter p;
            p.begin(&image);
            picture.play(&p);
            p.end();
            QString fileName = m_sre->addResource(QTextDocument::ImageResource, QVariant(image), url);
            return fileName;
        }
    }
    return QString();
}
