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
/**
 * \file plane.cpp
 * \brief A brief file description
 **/
#include "plane.h"
//*****************************************************************************************
/*! Standard Constructor creates a xy-plane at the global origin*/
plane::plane()
{
	setLabel(0);
	
	origin.x=0;
	origin.y=0;
	origin.z=0;
	
	normal.x=0;
	normal.y=0;
	normal.z=1;

}

//*****************************************************************************************
/*! Sets the label of the plane to label*/
void plane::setLabel(long l)
{
	label=l;
}
//*****************************************************************************************
/*! Returns the label of the plane */
long plane::getLabel()
{
	return(label);
}
//*****************************************************************************************
/*! Sets the normal of the plane */
void plane::setNormal(vector3d n)
{
	normal.x=n.x;
	normal.y=n.y;
	normal.z=n.z;
}
//*****************************************************************************************
/*! Sets the normal of the plane */
void plane::setNormal(double x,double y, double z)
{
	normal.x=x;
	normal.y=y;
	normal.z=z;
}
//*****************************************************************************************
/*! Sets the origin of the plane */
void plane::setOrigin(vector3d o)
{
	origin.x=o.x;
	origin.y=o.y;
	origin.z=o.z;
}
//*****************************************************************************************
/*! Sets the origin of the plane */
void plane::setOrigin(double x,double y, double z)
{
	origin.x=x;
	origin.y=y;
	origin.z=z;
}
//*****************************************************************************************
/*! Returns the origin of the plane*/
vector3d plane::getOrigin()
{
	return(origin);
}
//*****************************************************************************************
/*! Returns the normal of the plane*/
vector3d plane::getNormal()
{
	return(normal);
}
//**************************************************************************************************
/*! Moves the plane in normal direction */
void plane::offset(double distance)
{
	const double ZERO=1E-8;
	
	if (normal.norm()-1<ZERO)
		origin=origin+distance*normal;
	else
	{
		normal=normal/normal.norm();
		origin=origin+distance*normal;
	}
	
}
//************************************************************************************************
/*!Returns the parameters of the plane for cutting plane calculations*/
void plane::getParameters(double *params)
{
        //double params[4];
	params[0]=normal.x;
	params[1]=normal.y;
	params[2]=normal.z;
	params[3]=origin.norm();

}
//***************************************************************************************************
/*! Defines wether a plane should be drawn*/ 
void plane::setDrawn(bool d)
{
	drawn=d;
}
//***************************************************************************************************
/*! Returns wether a plane should be drawn */ 
bool plane::isDrawn()
{
	return(drawn);
}
