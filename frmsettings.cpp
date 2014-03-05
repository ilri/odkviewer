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

#include "frmsettings.h"
#include "ui_frmsettings.h"
#include <QSettings>
#include <QDir>
#include <QFileDialog>
#include "choosedir.h"

frmSettings::frmSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmSettings)
{
    ui->setupUi(this);


    QSettings settings("ILRI","odktocsv");
    ui->txtforms->setText(settings.value("formdir","Not set").toString());
    ui->txtdata->setText(settings.value("datadir","Not set").toString());

}

frmSettings::~frmSettings()
{
    delete ui;
}

void frmSettings::on_cmdcancel_clicked()
{
    close();
}

void frmSettings::on_cmdforms_clicked()
{
    QDir dir;

#ifdef Q_OS_ANDROID
    chooseDir dialogDir;
    dialogDir.setWindowState(Qt::WindowMaximized);
    dialogDir.seldir = lastDir;
    dialogDir.exec();
    dir.setPath(dialogDir.seldir);
    if (dir.exists())
    {
        ui->txtforms->setText(dialogDir.seldir);
        lastDir = dialogDir.seldir;
    }
#else

    QString sdir;

    sdir = ui->txtforms->text();
    dir.setPath(sdir);
    if (!dir.exists())
    {
        sdir = lastDir;
        dir.setPath(sdir);
        if (!dir.exists())
            sdir = "~/";
    }

    sdir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),sdir,QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    dir.setPath(sdir);
    if (dir.exists())
    {
        ui->txtforms->setText(sdir);
        lastDir = sdir;
    }
#endif


}

void frmSettings::on_cmddata_clicked()
{
    QDir dir;

#ifdef Q_OS_ANDROID
    chooseDir dialogDir;
    dialogDir.setWindowState(Qt::WindowMaximized);
    dialogDir.seldir = lastDir;
    dialogDir.exec();
    dir.setPath(dialogDir.seldir);
    if (dir.exists())
    {
        ui->txtdata->setText(dialogDir.seldir);
        lastDir = dialogDir.seldir;
    }
#else

    QString sdir;

    sdir = ui->txtdata->text();
    dir.setPath(sdir);
    if (!dir.exists())
    {
        sdir = lastDir;
        dir.setPath(sdir);
        if (!dir.exists())
            sdir = "~/";
    }

    sdir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),sdir,QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    dir.setPath(sdir);
    if (dir.exists())
    {
        ui->txtdata->setText(sdir);
        lastDir = sdir;
    }
#endif
}

void frmSettings::on_cmdsave_clicked()
{
    QSettings settings("ILRI","odktocsv");
    QDir dir;
    dir.setPath(ui->txtforms->text());
    if (dir.exists())
        settings.setValue("formdir",ui->txtforms->text());

    dir.setPath(ui->txtdata->text());
    if (dir.exists())
        settings.setValue("datadir",ui->txtdata->text());

    close();

}
