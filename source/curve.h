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
 * \file curve.h
 * \brief A brief file description
 **/
#ifndef CURVE_H
 #define CURVE_H

 #include <qgl.h>
 /*#include <QGLViewer/qglviewer.h>*/
 #include "./vector3d/vector3d.h"
 #include "point.h"
//!   Basic curve class  
/*! 
	Attributes and methods for a curve in space
 */
    class curve 
    {

    public:
	   curve();
	   curve(long,long,long);
       
	   void setLabel(long l);
	   long getLabel();
	   void setPoints(long,long);
	   void setStartPoint(long);
	   long getStartPoint();
	   void setEndPoint(long);
	   long getEndPoint();
	   void setDivision(long);
	   long getDivision();
            bool connectedTo(curve&);
           void reverse();
           point *sPoint;
           point *ePoint;
           double length();
	   

	private:
	   long label;
	   long startPoint;
	   long endPoint;
	   long division;
	};
#endif
