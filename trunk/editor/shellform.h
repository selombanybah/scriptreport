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
