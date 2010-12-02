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

#ifndef SHELLFORM_H
#define SHELLFORM_H

#include <QtCore/QStringList>
#include <QtCore/QEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QWidget>

namespace Ui {
    class ShellForm;
}

class ShellController;

/*
 * Class TabKeyEventFilter
 */

// Needed for handle autocomplete
class TabKeyEventFilter : public QObject
{
    Q_OBJECT

public:
    TabKeyEventFilter(QObject* parent = 0)
        : QObject(parent)
    {}

signals:
    void tabPressed();
    void historyUp();
    void historyDown();

protected:
    bool eventFilter(QObject */*obj*/, QEvent *event) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->modifiers() == Qt::NoModifier) {
                switch(keyEvent->key()) {
                case Qt::Key_Tab:
                    emit tabPressed();
                    return true;
                case Qt::Key_Up:
                    emit historyUp();
                    return true;
                case Qt::Key_Down:
                    emit historyDown();
                    return true;
                default:
                    return false;
                }
            } else {
                return false;
            }
        }
        return false;
    }
};

class ShellForm : public QWidget {
    Q_OBJECT
    Q_PROPERTY(bool isActive READ isActive)

public:
    ShellForm(QWidget *parent = 0);
    ~ShellForm();
    bool isActive() const;

public slots:
    void activate();

private slots:
    void executeSentence();
    void write(QString text);
    void complete();
    void reset();
    void useGlobalEngine(bool use);
    void historyUp();
    void historyDown();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ShellForm *ui;
    ShellController *m_controller;
    QString m_sentence;
    bool m_useGlobalEngine;
    QStringList m_history;
    int m_index;
};

#endif // SHELLFORM_H
