/***************************************************************************
 *   Copyright (C) 2008 by Patrick Prokopczuk   *
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
#ifndef COORDINATESYSTEM_H
#define COORDINATESYSTEM_H
#include "./vector3d/vector3d.h"
#include <QString>
#include <QObject>


class coordinateSystem
{
public:
    coordinateSystem();

public:
    void setOrigin(double,double,double);
    void setOrigin(vector3d);
    void setVector1(double,double,double);
    void setVector1(vector3d);
    void setVector2(double,double,double);
    void setVector2(vector3d);
    void setType(int);
    void move(double,double,double);
    void setName(const QString&);

public:
    int type;
    QString name;
    vector3d vector1;
    vector3d vector2;
    vector3d origin;
};

#endif // COORDINATESYSTEM_H
