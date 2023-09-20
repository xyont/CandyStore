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
#include "coordinatesystem.h"

/*! Standard constructor creates a rectangular system coincident with the global cos*/
coordinateSystem::coordinateSystem()
{
    type=0;

    origin.x=0;
    origin.y=0;
    origin.z=0;

    vector1.x=1;
    vector1.y=0;
    vector1.z=0;

    vector2.x=0;
    vector2.y=0;
    vector2.z=1;


}
//****************************************************************************
/*!Move the origin of the cos*/
void coordinateSystem::move(double x, double y, double z)
{
    origin.x+=x;
    origin.y+=y;
    origin.z+=z;

}
//****************************************************************************
/*!Set the name of the cos*/
void coordinateSystem::setName(const QString &n)
{
    name=n;
}
//*******************************************************************************
/*!Set the origin of the cos*/
void coordinateSystem::setOrigin(double x, double y, double z)
{
    origin.x=x;
    origin.y=y;
    origin.z=z;
}
//*********************************************************************************
/*!Set the origin of the cos*/
void coordinateSystem::setOrigin(vector3d o)
{
    origin.x=o.x;
    origin.y=o.y;
    origin.z=o.z;
}
//****************************************************************************
/*!Set the name of the cos*/
void coordinateSystem::setVector1(double x, double y, double z)
{
    vector1.x=x;
    vector1.y=y;
    vector1.z=z;
}
//*********************************************************************************
/*!Set the name of the cos*/
void coordinateSystem::setVector2(double x, double y, double z)
{
    vector2.x=x;
    vector2.y=y;
    vector2.z=z;
}
//***********************************************************************************
/*!Set the vector1 of the cos*/
void coordinateSystem::setVector1(vector3d v)
{
    vector1.x=v.x;
    vector1.y=v.y;
    vector1.z=v.z;
}
//************************************************************************************
/*!Set the vector2 of the cos*/
void coordinateSystem::setVector2(vector3d v)
{
    vector2.x=v.x;
    vector2.y=v.y;
    vector2.z=v.z;
}
//************************************************************************************
/*!Set the type of the cos*/
void coordinateSystem::setType(int t)
{
    type=t;
}
