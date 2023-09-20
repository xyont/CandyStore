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
 * \file point.h
 * \brief A brief file description
 **/
#ifndef POINT_H
 #define POINT_H

 #include <qgl.h>
 /*#include <QGLViewer/qglviewer.h>*/
 #include "./vector3d/vector3d.h"
 #include "plane.h"
//!   Basic point class  
/*! 
	Attributes and methods for a point in space
 */
    class point 
    {

    public:
	   point();
       
	   void setLabel(long l);
	   long getLabel();
	   
	   void setCoordinates(vector3d c);
	   void setCoordinates(double x, double y, double z);
	   void setCoordinates(int dof, double value);
	   vector3d getCoordinates();
	   void clearCoordinates();
	   double distanceToPlane(plane&);
	   double distanceToPlane(vector3d&,vector3d&);
	   double distanceToPoint(point&);
	   double getCoordinate1();
	   double getCoordinate2();
	   double getCoordinate3();
       void move(double,double,double);

	private:
	   long label;
	   vector3d coords;
    };
#endif
