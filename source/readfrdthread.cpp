/***************************************************************************
 *   Copyright (C) 2010 by Patrick Prokopczuk   *
 *   prokopczuk@gmx.de   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "readfrdthread.h"
#include <QFileDialog>
#include <QMessageBox>

readFrdMeshThread::readFrdMeshThread()
{
}
//**************************************************
/*! Constructing the thread with a filename, fe-database specified */
readFrdMeshThread::readFrdMeshThread(const QString& name,femodel* fd)
{
    fileName=name;
    fedata=fd;

}
//**************************************************
/*! Setting the filename */
void readFrdMeshThread::setFilename(const QString& name)
{
    fileName=name;
}
//**************************************************
/*! Setting the fedata-pointer */
void readFrdMeshThread::setFeData(femodel* fd)
{
    fedata=fd;
}
//****************************************************
/*! Running the thread */
void readFrdMeshThread::run()
{

    fedata->readfrd(fileName);
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
readFrdResultInfoThread::readFrdResultInfoThread()
{
}
//**************************************************
/*! Constructing the thread with a filename, fe-database specified */
readFrdResultInfoThread::readFrdResultInfoThread(const QString& name,femodel* fd)
{
    fileName=name;
    fedata=fd;

}
//**************************************************
/*! Setting the filename */
void readFrdResultInfoThread::setFilename(const QString& name)
{
    fileName=name;
}
//**************************************************
/*! Setting the fedata-pointer */
void readFrdResultInfoThread::setFeData(femodel* fd)
{
    fedata=fd;
}
//****************************************************
/*! Running the thread */
void readFrdResultInfoThread::run()
{

    fedata->readfrdResultInfo(fileName);
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
readFrdResultDataThread::readFrdResultDataThread()
{
}
//**************************************************
/*! Constructing the thread with a filename, fe-database specified */
readFrdResultDataThread::readFrdResultDataThread(const QString& name,femodel* fd)
{
    fileName=name;
    fedata=fd;

}
//**************************************************
/*! Setting the filename */
void readFrdResultDataThread::setFilename(const QString& name)
{
    fileName=name;
}
//**************************************************
/*! Setting the fedata-pointer */
void readFrdResultDataThread::setFeData(femodel* fd)
{
    fedata=fd;
}
//****************************************************
/*! Running the thread */
void readFrdResultDataThread::run()
{

    fedata->readfrd(fileName);
}
