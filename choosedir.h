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

#ifndef CHOOSEDIR_H
#define CHOOSEDIR_H

#include <QDialog>
#include <QModelIndex>

namespace Ui {
class chooseDir;
}

class chooseDir : public QDialog
{
    Q_OBJECT

public:
    explicit chooseDir(QWidget *parent = 0);
    ~chooseDir();
    QString seldir;

private slots:
    void on_cmdCancel_clicked();
    void itemExpanded(QModelIndex index);

    void on_cmdok_clicked();

private:
    Ui::chooseDir *ui;
};

#endif // CHOOSEDIR_H
