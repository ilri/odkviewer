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

#include "odkthread.h"

odkThread::odkThread(QObject *parent) :
    QThread(parent)
{
}

void odkThread::run()
{
    m_reader.processXML(m_surveyFile,m_mainModule);
}

void odkThread::setReader(odkFormReader reader)
{
    m_reader = reader;
}

odkFormReader odkThread::getReader()
{
    return m_reader;
}

void odkThread::setSurveyData(QString surveyFile, QString mainModule)
{
    m_surveyFile = surveyFile;
    m_mainModule = mainModule;
}
