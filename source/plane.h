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
#ifndef PLANE_H
 #define PLANE_H

 #include "./vector3d/vector3d.h"
//!   Basic plane class  
/*! 
	Attributes and methods for a plane in space
	Plane normal and origin is stored
 */
    class plane 
    {

    public:
	   plane();
	   
	   void setLabel(long l);
	   long getLabel(); 
	   void setNormal(double x, double y, double z);
	   void setNormal(vector3d n);
	   vector3d getNormal();
           void getParameters(double*);
	   void setOrigin(double x, double y, double z);
	   void setOrigin(vector3d o);
	   vector3d getOrigin();
	   void setCutting(bool c);
	   void setDrawn(bool d);
	   bool isDrawn();
	   void offset(double distance);
	private:
		
		
		
		long label;
		vector3d origin;
		vector3d normal;
		bool drawn;
		bool cutting;

	};
#endif
