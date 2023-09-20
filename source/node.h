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
 * \file node.h
 * \brief A brief file description
 **/
#ifndef NODE_H
 #define NODE_H


 #include "point.h"
 
//!   Basic node class  
/*! 
	The node class holds label, coords,adjacent elements, displacements,
	result value and normalized result value for the node
 */
///*Ist der normalized result value notwendig  ??????
 
    class node : public point
    {

    public:
	   node();
          
	   void setDisplacements(vector3d disp);
	   void setDisplacements(double x, double y, double z);
	   void setDisplacements(int dof, double value);
	   vector3d getDisplacements();
	   void clearDisp();
	   
	   void setResultValue(double value);
	   double getResultValue();
	   void clearResultValue();
	   
	   void setNormalizedResultValue(float value);
	   float getNormalizedResultValue();
	   void clearNormalizedResultValue();
	   
	   void setOutside(bool yn);
	   bool isOutside();
	   
	   QVector<long> adjacentElements;
	   
	   
	   
	  	
	private:
		vector3d displacements;
		double resultValue;
		float normalizedResultValue;
		bool outside;
		
    };

#endif
