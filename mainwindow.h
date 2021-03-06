/*
    ODK Viewer - A tool to visualise and edit survey data collected in ODK
    Copyright (C) 2014  International Livestock Research Institute
    Author: Carlos Quiros (Research Methods Group)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "formsmodel.h"
#include <QModelIndex>
#include <QMovie>
#include "odkthread.h"


namespace Ui {
class mainWindow;
}

class mainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit mainWindow(QWidget *parent = 0);
    ~mainWindow();

private slots:
    void on_cmdclose_clicked();

    void on_cmdsettings_clicked();

    void formClicked(QModelIndex index);

    void on_cmdabout_clicked();

    void readerFinished();

    void reStart();

private:
    Ui::mainWindow *ui;
    void loadSettings();
    QString dataDir;
    QString formsDir;
    formsModel *m_formModel;
    QMovie *movie;
    odkThread odkReader;
};

#endif // MAINWINDOW_H
