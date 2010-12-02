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

#ifndef EDITHANDLER_H
#define EDITHANDLER_H

#include <QObject>

class ExtendedUiMainWindow;
class MainWindow;

class EditHandler : public QObject
{
    Q_OBJECT

public:
    explicit EditHandler(ExtendedUiMainWindow *eui, MainWindow *parent = 0);

    void updateEditActions(bool enabled);
    void updateTextViewActions(bool enabled);
    void startEdit();

public slots:
    void updateLineAndColumn(bool enabled = true);

private slots:
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void delete_();
    void selectAll();
    void find();
    void closeFind();
    void replace();

    void canCut(bool avaiable);
    void canCopy(bool avaiable);
    void canDelete(bool avaiable);
    void canUndo(bool avaiable);
    void canRedo(bool avaiable);
    void clipboardDataChanged();

private:
    void canPaste(bool avaiable);

private:
    ExtendedUiMainWindow *ui;

};

#endif // EDITHANDLER_H
