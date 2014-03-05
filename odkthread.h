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

#ifndef ODKTHREAD_H
#define ODKTHREAD_H

#include <QThread>
#include "odkformreader.h"

// This class reads an ODK Survey in a separate execution thread so
// the application does not hang while reading a survey.

class odkThread : public QThread
{
    Q_OBJECT
public:
    explicit odkThread(QObject *parent = 0);
    void run(); //The main thread process
    void setReader(odkFormReader reader);
    odkFormReader getReader();
    void setSurveyData(QString surveyFile, QString mainModule);

private:
    odkFormReader m_reader;
    QString m_surveyFile;
    QString m_mainModule;


};

#endif // ODKTHREAD_H
