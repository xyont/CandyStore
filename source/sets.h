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
/**
 * \file sets.h
 * \brief A brief file description
 **/
#ifndef SETS_H
#define SETS_H
#include <qstring.h>
#include <qlist.h>

class sets
{

public:
    sets();
    QString name;
    int nodePlotColor;
    int facePlotColor;
    int elementPlotColor;
    QList<long> nodeList;
    QList<long> elementList;
    QList<long> faceIndexList;
    QList<long> pointList;
    QList<long> lineList;
    QList<long> surfaceList;
    QList<long> bodyList;
    bool qCommandFlag;
    int plotColor;
    bool plot;
    bool plotFace;
    long getNodeIndex(long,int*);
    long getFaceIndex(long,int*);
    long getElementIndex(long,int*);
};

#endif // SETS_H
