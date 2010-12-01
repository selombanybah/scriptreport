#ifndef PLAINTEXTEDITWITHLINENUMBER_H
#define PLAINTEXTEDITWITHLINENUMBER_H

/*
 * Based in the Qt Code Editor Example
 */

#include <QPlainTextEdit>
#include <QObject>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class PlainTextEditWithLineNumber : public QPlainTextEdit
{
    Q_OBJECT

public:
    PlainTextEditWithLineNumber(QWidget *parent = 0);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QWidget *m_lineNumberArea;
};

#endif // PLAINTEXTEDITWITHLINENUMBER_H
