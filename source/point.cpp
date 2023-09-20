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
 * \file point.cpp
 * \brief A brief file description
 **/
#include "point.h"
//*****************************************************************************************
/*! Standard Constructor creates a point at the global origin*/
point::point()
{
	label=0;
	coords.x=0;
	coords.y=0;
	coords.z=0;
	
}
//*****************************************************************************************
/*! Sets the label of the point to l*/
void point::setLabel(long l)
{
	label=l;
}
//*****************************************************************************************
/*! Returns the label of the point */
long point::getLabel()
{
	return(label);
}

//*****************************************************************************************
/*! Returns the coordinates of the point */
vector3d point::getCoordinates()
{
	return(coords);
}
//*****************************************************************************************
/*! Resets the coordinates to 0 0 0 */
void point::clearCoordinates()
{
	coords.x=0;
	coords.y=0;
	coords.z=0;
}
//*****************************************************************************************
/*! Sets the coordinates to c*/
void point::setCoordinates(vector3d c)
{
	coords.x=c.x;
	coords.y=c.y;
	coords.z=c.z;
}
//*****************************************************************************************
/*! Sets the coordinates to x, y,z*/
void point::setCoordinates(double x, double y, double z)
{
	coords.x=x;
	coords.y=y;
	coords.z=z;
}
//*****************************************************************************************
/*! Sets the coordinates of degree of freedom dof to value*/
void point::setCoordinates(int dof, double value)
{
	switch (dof)
	{
	case 1:
		coords.x=value;
	break;
	case 2:
		coords.y=value;
	break;
	case 3:
		coords.z=value;
	break;
	}
}
//********************************************************************
/*!Calculates the distance to the plane p*/
double point::distanceToPlane(plane &p)
{
	//Berechnen des Abstand mit Hilfe der Hesseschen Normalenform für die Ebene
	/*n.normalize();
	double p=o.norm();
	double d=coords*n-p;
	*/
	return(1);

}
//*************************************************************************
/*!Calculates the distance to the plane with origin o and normal n*/
double point::distanceToPlane(vector3d &o, vector3d &n)
{
	//Berechnen des Abstand mit Hilfe der Hesseschen Normalenform für die Ebene
	n.normalize();
	double p=o.norm();
	double d=coords*n-p;
	
	return(d);
	


}
//***********************************************************
/*!Calculates the distance to another point*/
double point::distanceToPoint(point &p)
{
	return((getCoordinates()-p.getCoordinates()).norm());
}
//****************************************************
/*!Returns the value of the first coordinate*/
double point::getCoordinate1()
{
    return(coords.x);
}
//*****************************************************
/*!Returns the value of the second coordinate*/
double point::getCoordinate2()
{
return(coords.y);
}
//****************************************************
/*!Returns the value of the third coordinate*/
double point::getCoordinate3()
{
return(coords.z);
}
//****************************************************
/*!Returns the value of the third coordinate*/
void point::move(double dx, double dy, double dz)
{
    coords.x+=dx;
    coords.y+=dy;
    coords.z+=dz;
}
